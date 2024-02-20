#include "HyperVisor/HyperVisor.hpp"

HyperVisorSvm::NptHook::NptHook()
{
    Pte::PAGE_TABLES_INFO Pti{};

    hooked_page = ExAllocatePoolZero(NonPagedPool, PAGE_SIZE, 'HOOK');
    Pte::GetPageTables(hooked_page, &Pti);

    hooked_pte = Pti.Pte;
}

HyperVisorSvm::NptHook* HyperVisorSvm::NptHooks::ForEachHook(bool(HookCallback)(NptHook* hook_entry, void* data), void* callback_data)
{
    for (int i = 0; i < hook_count; ++i)
    {
        if (HookCallback(&npt_hook_list[i], callback_data))
        {
            return &npt_hook_list[i];
        }
    }
    return 0;
}

PMDL LockPages(void* virtual_address, LOCK_OPERATION operation, KPROCESSOR_MODE access_mode, int size = PAGE_SIZE)
{
    PMDL mdl = IoAllocateMdl(virtual_address, size, FALSE, FALSE, nullptr);

    MmProbeAndLockPages(mdl, KernelMode, operation);

    return mdl;
}

NTSTATUS UnlockPages(PMDL mdl)
{
    MmUnlockPages(mdl);
    IoFreeMdl(mdl);

    return STATUS_SUCCESS;
}

/*	IMPORTANT: if you want to set a hook in a globally mapped DLL such as ntdll.dll, you must trigger copy on write first!	*/
HyperVisorSvm::NptHook* HyperVisorSvm::NptHooks::SetNptHook(
    SVM::PRIVATE_VM_DATA* vcpu,
    void* address,
    unsigned char* patch,
    size_t patch_len,
    int ncr3_id)
{
    NptHook* hook_entry = NULL;

    /*	page in the page if it's not present.	*/
    ULONG PageSize;
    if (!Pte::IsPagePresent(address, &PageSize))
    {
        return NULL;
    }
    else
    {
        bool reused_hook = false;

        /*	I switched from a linked list to an array, because traversing the linked list took too long	(CLOCK_WATCHDOG_TIMEOUT :|)*/
        hook_entry = &npt_hook_list[hook_count];

        hook_count += 1;

        /*	Lock the virtual->physical address translation,
            so that the NPT hook is locked to the same virtual address at all times.
        */
        KPROCESSOR_MODE mode = (uintptr_t)address < 0x7FFFFFFFFFF ? UserMode : KernelMode;

        auto physical_page = PAGE_ALIGN(MmGetPhysicalAddress(address).QuadPart);


        hook_entry->mdl = LockPages(PAGE_ALIGN(address), IoReadAccess, mode);

        hook_entry->ncr3_id = ncr3_id;
        hook_entry->address = address;
        hook_entry->process_cr3 = vcpu->Guest.StateSaveArea.Cr3;


        /*	get the guest pte and physical address of the hooked page	*/
        hook_entry->guest_physical_page = (unsigned char*)physical_page;

        Pte::PAGE_TABLES_INFO Pti;
        Pte::GetPageTables(address, &Pti);
        hook_entry->guest_pte = Pti.Pte;

        hook_entry->original_nx = hook_entry->guest_pte->x64.Page4Kb.NX;

        hook_entry->guest_pte->x64.Page4Kb.NX = 0;
        hook_entry->guest_pte->x64.Page4Kb.RW = 1;


        /*	get the nested pte of the guest physical address, in primary nCR3	*/
        Pte::PAGE_TABLES_INFO GpapPti;
        Pte::GetPageTables(address, &GpapPti, true, Ncr3Dirs[ncr3_id]);
        hook_entry->hookless_npte = GpapPti.Pte;//Utils::GetPte((void*)physical_page, Hypervisor::Get()->ncr3_dirs[ncr3_id]);

        if (hook_entry->hookless_npte->x64.Page4Kb.NX == 1)
        {
            /*	this page was already hooked	*/

            reused_hook = true;
        }

        hook_entry->hookless_npte->x64.Page4Kb.NX = 1;


        /*	get the nested pte of the guest physical address in the shadow NCR3, and map it to our hook page	*/
        Pte::PAGE_TABLES_INFO GpasPti;
        Pte::GetPageTables(address, &GpasPti, true, Ncr3Dirs[Ncr3Types::Shadow]);
        hook_entry->hooked_npte = GpasPti.Pte;//Utils::GetPte((void*)physical_page, Ncr3Dirs[Ncr3Types::Shadow]);

        if (address != patch)
        {
            hook_entry->hooked_npte->x64.Page4Kb.PhysicalPageFrameNumber = hook_entry->hooked_pte->x64.Page4Kb.PhysicalPageFrameNumber; //PageFrameNumber
        }

        hook_entry->hooked_npte->x64.Page4Kb.NX = 0;

        auto hooked_copy = PhysicalMemory::GetVirtualForPhysical((PVOID64)(hook_entry->hooked_npte->x64.Page4Kb.PhysicalPageFrameNumber << PAGE_SHIFT));

        auto page_offset = (uintptr_t)address & (PAGE_SIZE - 1);


        /*	place our hook in the copied page for the 2nd NCR3, and don't overwrite already existing hooks on the page	*/
        if (address != patch)
        {
            if (!reused_hook)
            {
                memcpy(hooked_copy, PAGE_ALIGN(address), PAGE_SIZE);
            }

            memcpy((unsigned char*)hooked_copy + page_offset, patch, patch_len);

            DbgPrint("hooked_copy %p \n\n", hooked_copy);
        }

    }

    return hook_entry;
}

void HyperVisorSvm::NptHooks::UnsetHook(HyperVisorSvm::NptHook* hook_entry)
{
    hook_entry->hookless_npte->x64.Page4Kb.NX = 0;
    hook_entry->hooked_npte->x64.Page4Kb.NX = 1;
    hook_entry->process_cr3 = 0;
    hook_entry->guest_pte->x64.Page4Kb.NX = hook_entry->original_nx;

    UnlockPages(hook_entry->mdl);

    memmove(npt_hook_list + hook_count - 1, npt_hook_list + hook_count, MAXHOOKS - hook_count);

    hook_count -= 1;
}

HyperVisorSvm::NptHooks::NptHooks()
{
    /*	reserve memory for hooks because we can't allocate memory in VM root	*/
    npt_hook_list = (NptHook*)ExAllocatePoolZero(NonPagedPool, sizeof(NptHook) * MAXHOOKS, 'hook');

    for (int i = 0; i < MAXHOOKS; ++i)
    {
        npt_hook_list[i] = NptHook{};
    }

    hook_count = 0;
}

CpuVendor HyperVisorSvm::GetCpuVendor()
{
    static CpuVendor CpuVendor; CPUID_REGS Regs;
    __cpuid(Regs.Raw, CPUID::Generic::CPUID_MAXIMUM_FUNCTION_NUMBER_AND_VENDOR_ID);
    if (Regs.Regs.Ebx == IntelEnc::IEbx && Regs.Regs.Edx == IntelEnc::IEdx && Regs.Regs.Ecx == IntelEnc::IEcx) { CpuVendor = CpuVendor::CpuIntel; }
    else if (Regs.Regs.Ebx == AmdEnc::AEbx && Regs.Regs.Edx == AmdEnc::AEdx && Regs.Regs.Ecx == AmdEnc::AEcx) { CpuVendor = CpuVendor::CpuAmd; }
    else { CpuVendor = CpuVendor::CpuUnknown; }
    return CpuVendor;
}

bool HyperVisorSvm::DevirtualizeProcessor(
    _Out_ PVOID &PrivateVmData)
{
    PrivateVmData = NULL; CPUID_REGS Regs = {};
    __cpuid(Regs.Raw, CPUID_VMM_SHUTDOWN);
    if (Regs.Regs.Ecx != CPUID_VMM_SHUTDOWN) { return false; }// Processor not virtualized!

    // Processor is devirtualized now:
    //  Info.Eax -> PRIVATE_VM_DATA* Private LOW
    //  Info.Ebx -> Vmexit RIP
    //  Info.Ecx -> VMEXIT_SIGNATURE
    //  Info.Edx -> PRIVATE_VM_DATA* Private HIGH

    PrivateVmData = reinterpret_cast<void*>(
        (static_cast<UINT64>(Regs.Regs.Edx) << 32u) |
        (static_cast<UINT64>(Regs.Regs.Eax))
        );

    return true;
}

bool HyperVisorSvm::DevirtualizeAllProcessors()
{
    ULONG ProcessorsCount = KeQueryActiveProcessorCountEx(ALL_PROCESSOR_GROUPS);
    PVOID* PrivateVmDataArray = VirtualMemory::AllocArray<PVOID>(ProcessorsCount);

    struct Callable {
        static ULONG_PTR BroadCast(void* Arg, void* Class)
        {
            PVOID* PrivateVmDataArray = reinterpret_cast<PVOID*>(Arg);
            ULONG CurrentProcessor = KeGetCurrentProcessorNumber();
            PVOID PrivateVmData = NULL;
            bool Status = static_cast<HyperVisorSvm*>(Class)->DevirtualizeProcessor(OUT PrivateVmData);
            PrivateVmDataArray[CurrentProcessor] = PrivateVmData; // Data buffer to free
            return static_cast<ULONG_PTR>(Status);
        }
    };

    KeIpiGenericCall(reinterpret_cast<PKIPI_BROADCAST_WORKER>(Callable::BroadCast), reinterpret_cast<ULONG_PTR>(PrivateVmDataArray));
    
    for (ULONG i = 0; i < ProcessorsCount; ++i)
    {
        if (PrivateVmDataArray[i])
        {
            PhysicalMemory::FreePhysicalMemory(PrivateVmDataArray[i]);
        }
    }

    g_IsVirtualized = false;

    return true;
}

#define IA32_APIC_BASE  0x0000001b
#define IA32_MSR_PAT    0x00000277
#define IA32_MSR_EFER   0xc0000080

void HyperVisorSvm::BuildNestedPagingTables(
    _In_ const PHYSICAL_MEMORY_DESCRIPTOR* MemoryDescriptor,
    _Out_ SVM::NESTED_PAGING_TABLES* Npt,
    _Out_ PHOOK_DATA HookData,
    _In_ PTEAccess Flags)
{
    static constexpr ULONG oneGigabyte = 1024 * 1024 * 1024;

    using namespace PhysicalMemory;

    if (!Npt) return;
    const PHYSICAL_MEMORY_RUN* currentRun;
    APIC_BASE apicBase;
    PTE* entry;
    ULONG64 baseAddr = 0;

    HookData->Pml4Table = &Npt->Pml4e;
    HookData->MaxNptPdpEntriesUsed = 0;

    Npt->Pml4e.x64.Page2Mb.P = Flags.P; // Present
    Npt->Pml4e.x64.Page2Mb.RW = Flags.RW; // Writeable
    Npt->Pml4e.x64.Page2Mb.US = Flags.US; // User
    Npt->Pml4e.x64.Page2Mb.NX = !Flags.NX; // No execute
    Npt->Pml4e.x64.Page2Mb.PDP = PAGE_TO_PFN(reinterpret_cast<UINT64>(GetPhysicalAddress(&Npt->Pdpe[0])));

    for (int i = 0; i < _ARRAYSIZE(Npt->Pdpe); ++i)
    {
        Npt->Pdpe[i].x64.NonPageSize.Page2Mb.P = Flags.P; // Present
        Npt->Pdpe[i].x64.NonPageSize.Page2Mb.RW = Flags.RW; // Writeable
        Npt->Pdpe[i].x64.NonPageSize.Page2Mb.US = Flags.US; // User
        Npt->Pdpe[i].x64.NonPageSize.Page2Mb.NX = !Flags.NX; // No execute
        Npt->Pdpe[i].x64.NonPageSize.Page2Mb.PD = PAGE_TO_PFN(reinterpret_cast<UINT64>(GetPhysicalAddress(&Npt->Pde[i][0])));

        for (int j = 0; j < _ARRAYSIZE(Npt->Pde[i]); ++j)
        {
            Npt->Pde[i][j].x64.Page2Mb.P = Flags.P; // Present
            Npt->Pde[i][j].x64.Page2Mb.RW = Flags.RW; // Writeable
            Npt->Pde[i][j].x64.Page2Mb.US = Flags.US; // User
            Npt->Pde[i][j].x64.Page2Mb.NX = !Flags.NX; // No execute
            Npt->Pde[i][j].x64.Page2Mb.PS = TRUE; // Large page
            Npt->Pde[i][j].x64.Page2Mb.PhysicalPageFrameNumber = i * _ARRAYSIZE(Npt->Pde[i]) + j;
        }
    }

    //
    // Build all NTP entries based on the specified physical memory ranges.
    //
    for (ULONG runIndex = 0; runIndex < MemoryDescriptor->NumberOfRuns; ++runIndex)
    {
        currentRun = &MemoryDescriptor->Run[runIndex];
        baseAddr = currentRun->BasePage * PAGE_SIZE;
        for (PFN_NUMBER pageIndex = 0; pageIndex < currentRun->PageCount; ++pageIndex)
        {
            ULONG64 indexedAddr;

            indexedAddr = baseAddr + pageIndex * PAGE_SIZE;
            entry = BuildSubTables(&Npt->Pml4e, indexedAddr, FALSE);
            if (entry == nullptr) { return; }
        }
    }

    //
    // Create an entry for the APIC base.
    //
    apicBase.AsUInt64 = __readmsr(IA32_APIC_BASE);
    entry = BuildSubTables(&Npt->Pml4e, apicBase.Fields.ApicBase * PAGE_SIZE, FALSE);
    if (entry == nullptr)
    {
        return;
    }

    //
    // Compute the max PDPT index based on the last descriptor entry that
    // describes the address of the highest physical page. The index is rounded
    // up with 1GB since a single PDPT entry manages 1GB. (eg, the index will be
    // 2 if the highest physical address is at 1800MB.)
    //
    currentRun = &MemoryDescriptor->Run[MemoryDescriptor->NumberOfRuns - 1];
    baseAddr = currentRun->BasePage * PAGE_SIZE;
    HookData->MaxNptPdpEntriesUsed = ROUND_TO_SIZE(baseAddr + currentRun->PageCount * PAGE_SIZE,
        oneGigabyte) / oneGigabyte;

    return;
}

void HyperVisorSvm::FillVmcbSegmentAttributes(
    _Out_ SVM::VMCB_STATE_SAVE_AREA::VMCB_SEGMENT_ATTRIBUTE* Attribute,
    _In_ const SEGMENT_SELECTOR* Selector,
    _In_ const DESCRIPTOR_TABLE_REGISTER_LONG* Gdtr
) {
    auto Gdt = reinterpret_cast<SEGMENT_DESCRIPTOR_LONG*>(Gdtr->BaseAddress);
    auto Descriptor = reinterpret_cast<USER_SEGMENT_DESCRIPTOR_LONG*>(&Gdt[Selector->Bitmap.SelectorIndex]);

    Attribute->Value = 0;
    Attribute->Bitmap.Type = Descriptor->Generic.Type;
    Attribute->Bitmap.System = Descriptor->Generic.System;
    Attribute->Bitmap.Dpl = Descriptor->Generic.Dpl;
    Attribute->Bitmap.Present = Descriptor->Generic.Present;
    Attribute->Bitmap.Available = Descriptor->Generic.Available;
    Attribute->Bitmap.LongMode = Descriptor->Generic.LongMode;
    Attribute->Bitmap.DefaultOperandSize = Descriptor->Generic.DefaultOperandSize;
    Attribute->Bitmap.Granularity = Descriptor->Generic.Granularity;
}

extern const PHYSICAL_MEMORY_DESCRIPTOR* g_PhysicalMemoryDescriptor;

bool HyperVisorSvm::VirtualizeProcessor()
{
    static volatile bool IsVirtualized = false;
    IsVirtualized = false;
    CONTEXT Context = {};
    Context.ContextFlags = CONTEXT_ALL;
    RtlCaptureContext(&Context);

    //DbgBreakPoint();
    //CR4 Cr4 = {};
    //Cr4.Value = __readcr4();
    //DbgPrint("%p\n", (PVOID)Cr4.Value);
    //Cr4.x64.Bitmap.LA57 = TRUE;
    //DbgPrint("%p\n", (PVOID)Cr4.Value);
    //__writecr4(Cr4.Value);

    if (IsVirtualized) { return true; }
    AMD::EFER Efer = {};
    Efer.Value = __readmsr(static_cast<unsigned long>(AMD::AMD_MSR::MSR_EFER));
    Efer.Bitmap.SecureVirtualMachineEnable = TRUE;
#if REALPC == 1
    Efer.Bitmap.TranslationCacheExtension = TRUE;
#endif // REALPC
    __writemsr(static_cast<unsigned long>(AMD::AMD_MSR::MSR_EFER), Efer.Value);
    SVM::PRIVATE_VM_DATA* Private = reinterpret_cast<SVM::PRIVATE_VM_DATA*>(PhysicalMemory::AllocZeroedPhys(sizeof(*Private)));

    // Callback
    Interceptions = reinterpret_cast<_Interceptions>(PInterceptions);
    Private = Interceptions(Private);

    // Guest Address Space ID:
    Private->Guest.ControlArea.GuestAsid = 1;

    {
        DbgBreakPoint();
        DbgPrint("Private->HookData: %p", Private->HookData);
        Private->HookData = PHOOK_DATA(ExAllocatePool(NonPagedPool,
            sizeof(*Private->HookData)));
        DbgPrint("Private->HookData: %p", Private->HookData);
        RtlZeroMemory(Private->HookData, sizeof(*Private->HookData));


        // Nested paging:
        BuildNestedPagingTables(g_PhysicalMemoryDescriptor, &Private->Npt, Private->HookData, { true, true, true, true });

        InitializePreAllocatedEntries(
            Private->HookData->PreAllocatedNptEntries,
            RTL_NUMBER_OF(Private->HookData->PreAllocatedNptEntries));
    }
    
    //Ncr3Dirs[Ncr3Types::Primary] = reinterpret_cast<UINT64>(PhysicalMemory::GetPhysicalAddress(&Private->Npt.Pml5e));
    //Npts[Ncr3Types::Primary] = Private->Npt;
    //BuildNestedPagingTables(&Npts[Ncr3Types::Shadow], { true, true, true, false });
    //Ncr3Dirs[Ncr3Types::Shadow] = reinterpret_cast<UINT64>(PhysicalMemory::GetPhysicalAddress(&Npts[Ncr3Types::Shadow].Pml5e));
    //BuildNestedPagingTables(&Npts[Ncr3Types::Sandbox], { true, true, true, false });
    //Ncr3Dirs[Ncr3Types::Sandbox] = reinterpret_cast<UINT64>(PhysicalMemory::GetPhysicalAddress(&Npts[Ncr3Types::Sandbox].Pml5e));
    //BuildNestedPagingTables(&Npts[Ncr3Types::SandboxSingleStep], { true, true, true, true });
    //Ncr3Dirs[Ncr3Types::SandboxSingleStep] = reinterpret_cast<UINT64>(PhysicalMemory::GetPhysicalAddress(&Npts[Ncr3Types::SandboxSingleStep].Pml5e));

    Private->Guest.ControlArea.NpEnable = TRUE;
    //DbgPrint("%p\n", reinterpret_cast<PVOID>(PhysicalMemory::GetPhysicalAddress(&Private->Npt.Pml4e)));
    Private->Guest.ControlArea.NestedPageTableCr3 = reinterpret_cast<UINT64>(PhysicalMemory::GetPhysicalAddress(&Private->Npt.Pml4e));//4
    //Private->Guest.ControlArea.NestedPageTableCr3 = Ncr3Dirs[Ncr3Types::Primary];


    DESCRIPTOR_TABLE_REGISTER_LONG Gdtr = {}, Idtr = {}; _sgdt(&Gdtr); __sidt(&Idtr);

    // Setting up the initial guest state to the current system state:
    Private->Guest.StateSaveArea.Gdtr.Base = Gdtr.BaseAddress;
    Private->Guest.StateSaveArea.Gdtr.Limit = Gdtr.Limit;
    Private->Guest.StateSaveArea.Idtr.Base = Idtr.BaseAddress;
    Private->Guest.StateSaveArea.Idtr.Limit = Idtr.Limit;

    Private->Guest.StateSaveArea.Cs.Limit = GetSegmentLimit(Context.SegCs);
    Private->Guest.StateSaveArea.Ds.Limit = GetSegmentLimit(Context.SegDs);
    Private->Guest.StateSaveArea.Es.Limit = GetSegmentLimit(Context.SegEs);
    Private->Guest.StateSaveArea.Ss.Limit = GetSegmentLimit(Context.SegSs);
    
    Private->Guest.StateSaveArea.Cs.Selector = Context.SegCs;
    Private->Guest.StateSaveArea.Ds.Selector = Context.SegDs;
    Private->Guest.StateSaveArea.Es.Selector = Context.SegEs;
    Private->Guest.StateSaveArea.Ss.Selector = Context.SegSs;

    FillVmcbSegmentAttributes(&Private->Guest.StateSaveArea.Cs.Attrib, reinterpret_cast<const SEGMENT_SELECTOR*>(&Context.SegCs), &Gdtr);
    FillVmcbSegmentAttributes(&Private->Guest.StateSaveArea.Ds.Attrib, reinterpret_cast<const SEGMENT_SELECTOR*>(&Context.SegDs), &Gdtr);
    FillVmcbSegmentAttributes(&Private->Guest.StateSaveArea.Es.Attrib, reinterpret_cast<const SEGMENT_SELECTOR*>(&Context.SegEs), &Gdtr);
    FillVmcbSegmentAttributes(&Private->Guest.StateSaveArea.Ss.Attrib, reinterpret_cast<const SEGMENT_SELECTOR*>(&Context.SegSs), &Gdtr);

    Private->Guest.StateSaveArea.Efer = Efer.Value;
    Private->Guest.StateSaveArea.Cr0 = __readcr0();
    Private->Guest.StateSaveArea.Cr2 = __readcr2();
    Private->Guest.StateSaveArea.Cr3 = __readcr3();
    Private->Guest.StateSaveArea.Cr4 = __readcr4();
    Private->Guest.StateSaveArea.Rflags.Value = Context.EFlags;
    Private->Guest.StateSaveArea.Rsp = Context.Rsp;
    Private->Guest.StateSaveArea.Rip = Context.Rip;
    Private->Guest.StateSaveArea.GuestPat = __readmsr(static_cast<unsigned long>(AMD::AMD_MSR::MSR_PAT));

    PVOID GuestVmcbPa = PhysicalMemory::GetPhysicalAddress(&Private->Guest);
    PVOID HostVmcbPa = PhysicalMemory::GetPhysicalAddress(&Private->Host);

    // Store state to the guest VMCB:
    __svm_vmsave(reinterpret_cast<size_t>(GuestVmcbPa));

    // Store the address of the HostStateArea:
    __writemsr(static_cast<unsigned long>(AMD::AMD_MSR::MSR_VM_HSAVE_PA), reinterpret_cast<UINT64>(PhysicalMemory::GetPhysicalAddress(Private->HostStateArea)));

    // Store state to the host VMCB to load it after the #VMEXIT:
    __svm_vmsave(reinterpret_cast<size_t>(HostVmcbPa));

    IsVirtualized = true;

    Private->VmmStack.Layout.InitialStack.GuestVmcbPa = GuestVmcbPa;
    Private->VmmStack.Layout.InitialStack.HostVmcbPa = HostVmcbPa;
    Private->VmmStack.Layout.InitialStack.Private = Private;

    // Callback
    SvmVmmRun = reinterpret_cast<_SvmVmmRun>(PSvmVmmRun);
    SvmVmmRun(&Private->VmmStack.Layout.InitialStack);

    // If SvmVmmRun returns to here, something went wrong:
    PhysicalMemory::FreePhysicalMemory(Private);
    return false;
}

bool HyperVisorSvm::VirtualizeAllProcessors()
{
    struct Callback_ {
        static bool inForEachCpu(void* Arg, unsigned int ProcessorNumber) {
            UNREFERENCED_PARAMETER(ProcessorNumber);
            return static_cast<HyperVisorSvm*>(Arg)->VirtualizeProcessor();
        }
        static bool inSystemContext(void* Arg) {
            return Callable::ForEachCpu(&Callback_::inForEachCpu, Arg);
        }
    };
    bool Status = Callable::CallInSystemContext(&Callback_::inSystemContext, this);
    if (!Status) { DevirtualizeAllProcessors(); } return Status;
}

bool HyperVisorSvm::IsSvmSupported()
{
    CPUID_REGS Regs = {};

    // Check the 'AuthenticAMD' vendor name:
    __cpuid(Regs.Raw, CPUID::Generic::CPUID_MAXIMUM_FUNCTION_NUMBER_AND_VENDOR_ID);
    if (Regs.Regs.Ebx != AmdEnc::AEbx || Regs.Regs.Edx != AmdEnc::AEdx || Regs.Regs.Ecx != AmdEnc::AEcx) return false;

    // Check the AMD SVM (AMD-V) support:
    constexpr unsigned int CPUID_FN80000001_ECX_SVM = 1 << 2;
    __cpuid(Regs.Raw, CPUID::Generic::CPUID_EXTENDED_FEATURE_INFORMATION);
    if ((Regs.Regs.Ecx & CPUID_FN80000001_ECX_SVM) == 0) return false;

    // Check the Nested Paging support (AMD-RVI):
    constexpr unsigned int CPUID_FN8000000A_EDX_NESTED_PAGING = 1 << 0;
    __cpuid(Regs.Raw, CPUID::AMD::CPUID_SVM_FEATURES);
    if ((Regs.Regs.Edx & CPUID_FN8000000A_EDX_NESTED_PAGING) == 0) return false;

    // Check that the EFER.SVME is writeable (we can enable the SVM):
    AMD::VM_CR VmCr = {};
    VmCr.Value = __readmsr(static_cast<unsigned long>(AMD::AMD_MSR::MSR_VM_CR));
    if (VmCr.Bitmap.SVMDIS) return false;

    return true;
}