/*!
    @file HookCommon.hpp

    @brief Kernel mode and VMM shared code.

    @author Satoshi Tanda

    @copyright Copyright (c) 2018-2021, Satoshi Tanda. All rights reserved.
 */
#pragma once
#include <fltKernel.h>
#include "HyperVisor/CommonTypes/PTE.hpp"

//
// The data structure represents a single hook.
//
typedef struct _HOOK_ENTRY
{
    //
    // The address where hook is/will be installed.
    //
    PVOID HookAddress;

    //
    // The handler function to be executed when the hooked function is executed.
    //
    PVOID Handler;

    //
    // The page aligned address of the exec page.
    //
    PVOID PageBaseForExecution;

    //
    // The page aligned physical memory address of HookAddress.
    //
    ULONG64 PhyPageBase;

    //
    // The page aligned physical memory address of PageBaseForExecution.
    //
    ULONG64 PhyPageBaseForExecution;

    //
    // The address of code that does "jump to the next instruction of original
    // code". Must be freed when this structure is freed.
    //
    PVOID OriginalCallStub;
} HOOK_ENTRY, *PHOOK_ENTRY;

//
// The data structure where developer(s) can define a function to be hooked and
// a corresponding handler.
//
typedef struct _HOOK_REGISTRATION_ENTRY
{
    //
    // The name of NT-kernel exported function to hook.
    //
    UNICODE_STRING FunctionName;

    //
    // The handler function to be executed when the hooked function is executed.
    //
    PVOID Handler;

    //
    // The data initialized at runtime.
    //
    HOOK_ENTRY HookEntry;
} HOOK_REGISTRATION_ENTRY, *PHOOK_REGISTRATION_ENTRY;

//
// The list of functions to be hooked.
//
#if (SIMPLESVMHOOK_SINGLE_HOOK == 0)
extern HOOK_REGISTRATION_ENTRY g_HookRegistrationEntries[4];
#else
extern HOOK_REGISTRATION_ENTRY g_HookRegistrationEntries[1];
#endif

//
// State of NPT. See HookVmmCommon.cpp for details.
//
typedef enum _NPT_STATE
{
    NptDefault,
    NptHookEnabledInvisible,
    NptHookEnabledVisible,
} NPT_STATE, *PNPT_STATE;

//
// The per processor data structure for hooking.
//
typedef struct _HOOK_DATA
{
    //
    // The NTP PML4. Its physical address is set to NCr3 of VMCB.
    //
    PML4E* Pml4Table;

    //
    // The pre-allocated NTP entries and how many of them are assigned to
    // NPTs. 50 is a large enough number based on observation. Our hypervisor
    // will trigger bug check when the pre-allocated entries are exhausted.
    //
    PVOID PreAllocatedNptEntries[50];
    volatile LONG UsedPreAllocatedEntriesCount;

    //
    // The highest index of PDPT + 1 to cover all physical memory ranges. This
    // data is not necessary "per processor" since it is a system global thing,
    // but it is stored here for an easy access.
    //
    ULONG MaxNptPdpEntriesUsed;

    //
    // A read only pointer to the hook entry that corresponds to the page the
    // processor is currently executing on. If the processor is executing on
    // a page that does not have a hook, this is NULL.
    //
    const HOOK_ENTRY* ActiveHookEntry;

    //
    // The indicator of NTP state. See HookVmmCommon.cpp for details.
    //
    NPT_STATE NptState;
} HOOK_DATA, *PHOOK_DATA;


// Get the highest 25 bits
static constexpr ULONGLONG k_PxiShift = 39;

// Get the highest 34 bits
static constexpr ULONGLONG k_PpiShift = 30;

// Get the highest 43 bits
static constexpr ULONGLONG k_PdiShift = 21;

// Get the highest 52 bits
static constexpr ULONGLONG k_PtiShift = 12;

// Use 9 bits; 0b0000_0000_0000_0000_0000_0000_0001_1111_1111
static constexpr ULONGLONG k_PtxMask = 0x1ff;

/*!
    @brief Returns the index of PML4 entry for the specified physical address.

    @param[in] PhysicalAddress - The physical address to get the index.

    @return The index of PML4 entry.
 */
inline
_Check_return_
ULONG64
GetPxeIndex (
    _In_ ULONG64 PhysicalAddress
    )
{
    return (PhysicalAddress >> k_PxiShift) & k_PtxMask;
}

/*!
    @brief Returns the index of PDPT entry for the specified physical address.

    @param[in] PhysicalAddress - The physical address to get the index.

    @return The index of PDPT entry.
 */
inline
_Check_return_
ULONG64
GetPpeIndex (
    _In_ ULONG64 PhysicalAddress
    )
{
    return (PhysicalAddress >> k_PpiShift) & k_PtxMask;
}

/*!
    @brief Returns the index of PDT entry for the specified physical address.

    @param[in] PhysicalAddress - The physical address to get the index.

    @return The index of PDT entry.
 */
inline
_Check_return_
ULONG64
GetPdeIndex (
    _In_ ULONG64 PhysicalAddress
    )
{
    return (PhysicalAddress >> k_PdiShift) & k_PtxMask;
}

/*!
    @brief Returns the index of PT entry for the specified physical address.

    @param[in] PhysicalAddress - The physical address to get the index.

    @return The index of PT entry.
 */
inline
_Check_return_
ULONG64
GetPteIndex (
    _In_ ULONG64 PhysicalAddress
    )
{
    return (PhysicalAddress >> k_PtiShift) & k_PtxMask;
}

/*!
    @brief Returns the physical address of the non-pagable virtual address.

    @param[in] VirtualAddress - The non-pagable virtual address.

    @return The physical address of the virtual address.
 */
inline
_Check_return_
ULONG64
GetPaFromVa (
    _In_ PVOID VirtualAddress
    )
{
    PHYSICAL_ADDRESS pa;

    pa = MmGetPhysicalAddress(VirtualAddress);
    return pa.QuadPart;
}

/*!
    @brief Returns the PFN of the physical address.

    @param[in] PhysicalAddress - The physical address.

    @return The PFN of the physical address.
 */
inline
_Check_return_
PFN_NUMBER
GetPfnFromPa (
    _In_ ULONG64 PhysicalAddress
    )
{
    return static_cast<PFN_NUMBER>(PhysicalAddress >> PAGE_SHIFT);
}

/*!
    @brief Returns the PFN of the non-pagable virtual address.

    @param[in] VirtualAddress - The non-pagable virtual address.

    @return The PFN of the physical address.
 */
inline
_Check_return_
PFN_NUMBER
GetPfnFromVa (
    _In_ PVOID VirtualAddress
    )
{
    return GetPfnFromPa(GetPaFromVa(VirtualAddress));
}

/*!
    @brief Returns the virtual address of the physical address.

    @param[in] PhysicalAddress - The physical address.

    @return The virtual address of the physical address.
 */
inline
_Check_return_
PVOID
GetVaFromPa (
    _In_ ULONG64 PhysicalAddress
    )
{
    PHYSICAL_ADDRESS pa;

    pa.QuadPart = PhysicalAddress;
    return MmGetVirtualForPhysical(pa);
}

/*!
    @brief Returns the physical address of the PFN.

    @param[in] PageFrameNumber - The PFN.

    @return The physical address of the PFN.
 */
inline
_Check_return_
ULONG64
GetPaFromPfn (
    _In_ PFN_NUMBER PageFrameNumber
    )
{
    return static_cast<ULONG64>(PageFrameNumber) << PAGE_SHIFT;
}

/*!
    @brief Returns the virtual address of the PFN.

    @param[in] PageFrameNumber - The PFN.

    @return The virtual address of the PFN.
 */
inline
_Check_return_
PVOID
GetVaFromPfn (
    _In_ PFN_NUMBER PageFrameNumber
    )
{
    return GetVaFromPa(GetPaFromPfn(PageFrameNumber));
}

_When_(HookData == nullptr, _Post_maybenull_)
_Post_writable_byte_size_(PAGE_SIZE)
PVOID
AllocateNptEntry (
    _Inout_opt_ PHOOK_DATA HookData
    );

_Check_return_
PTE*
BuildSubTables (
    _Inout_ PML4E* Pml4Table,
    _In_ ULONG64 PhysicalAddress,
    _Inout_opt_ PHOOK_DATA HookData
    );

_Check_return_
PTE*
GetNestedPageTableEntry (
    _Inout_ PML4E* Pml4Table,
    _In_ ULONG64 PhysicalAddress
    );
