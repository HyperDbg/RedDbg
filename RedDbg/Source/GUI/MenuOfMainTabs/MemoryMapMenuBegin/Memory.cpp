#include "GUI/MenuOfMainTabs/MemoryMapMenuBegin/Memory.hpp"

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

ParsedPeRefDll MemoryParser_::OpenExecutable(std::string path) noexcept {
    ParsedPeRefDll obj(peparse::ParsePEFromFile(path.data(), true),
        peparse::DestructParsedPE);
    if (!obj) {
        return ParsedPeRefDll(nullptr, peparse::DestructParsedPE);
    }
    return obj;
}

nsMemoryParser::Error MemoryParser_::GetHeap()
{
    USERMODE_HEAP_DETAILS HeapCountRequest = { 0 };
    DWORD ReturnedLength = 0;

    //printf("%x\n", CustomTitleBarGlobalVars::Pid);
    HeapCountRequest.ProcessId = CustomTitleBarGlobalVars::Pid;
    HeapCountRequest.NumberOfHeapsToReturn = 0;

    bool Status = DeviceIoControl(
        CustomTitleBarGlobalVars::hDriver,
        IoCtlCode::GetProcessHeaps,
        &HeapCountRequest,
        sizeof(USERMODE_HEAP_DETAILS),
        &HeapCountRequest,
        sizeof(USERMODE_HEAP_DETAILS),
        &ReturnedLength,
        NULL);

    //printf("HeapCountRequest.TotalHeaps: %x\n", HeapCountRequest.TotalHeaps);
    if (!Status) { return nsMemoryParser::Error::HeapDoesntExist; }

    PHANDLE aHeaps = nullptr; size_t BytesToAllocate;
    HRESULT Result = SIZETMult(HeapCountRequest.TotalHeaps, sizeof(*aHeaps), &BytesToAllocate);
    if (Result != S_OK) { return nsMemoryParser::Error::ResultIsntS_OK; }

    aHeaps = (PHANDLE)VirtualAlloc(NULL, BytesToAllocate, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE); //(PHANDLE)HeapAlloc(HHandle, 0, BytesToAllocate);
    size_t HeapsLength = HeapCountRequest.TotalHeaps;
    HeapCountRequest.NumberOfHeapsToReturn = HeapCountRequest.TotalHeaps;
    HeapCountRequest.ProcessHeap = aHeaps;
    Status = DeviceIoControl(
        CustomTitleBarGlobalVars::hDriver,
        IoCtlCode::GetProcessHeaps,
        &HeapCountRequest,
        sizeof(USERMODE_HEAP_DETAILS),
        &HeapCountRequest,
        sizeof(USERMODE_HEAP_DETAILS),
        &ReturnedLength,
        NULL);

    for (int Index = 0; Index < HeapCountRequest.TotalHeaps; ++Index)
    {
        vHeapAddrs.push_back((uint64_t)HeapCountRequest.ProcessHeap[Index]);
        //printf("Heap at address: %#p.\n", HeapCountRequest.ProcessHeap[Index]);
    }

    return nsMemoryParser::Error::AllOk;
}

nsMemoryParser::Error MemoryParser_::GetTebAndStackForEachThread()
{
    static HMODULE hNtDll = LoadLibraryA(Ntdll.c_str());
    if (hNtDll == NULL) { return nsMemoryParser::Error::hNtDllIsNull; }

    static NTQUERYINFORMATIONTHREAD NtQueryInformationThread = (NTQUERYINFORMATIONTHREAD)GetProcAddress(hNtDll, "NtQueryInformationThread");
    if (NtQueryInformationThread == NULL) { FreeLibrary(hNtDll); return nsMemoryParser::Error::NtQueryInformationThreadIsNull; }

    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, CustomTitleBarGlobalVars::Pid);
    if (hThreadSnap == INVALID_HANDLE_VALUE) { return nsMemoryParser::Error::hThreadSnapHasInvalidHandleValue; }

    THREADENTRY32 te32; te32.dwSize = sizeof(THREADENTRY32);
    if (!Thread32First(hThreadSnap, &te32)) { CloseHandle(hThreadSnap); return nsMemoryParser::Error::FailedToGetFirstThread; }

    do {
        if (te32.th32OwnerProcessID == CustomTitleBarGlobalVars::Pid) {
            HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te32.th32ThreadID);
            if (hThread != NULL) {
                CLIENT_ID clientId;
                clientId.UniqueProcess = 0;
                clientId.UniqueThread = (HANDLE)te32.th32ThreadID;

                NT_TIB tib = { 0 };
                THREAD_BASIC_INFORMATION tbi = { 0 };

                NTSTATUS status = NtQueryInformationThread(hThread, (THREADINFOCLASS)ThreadBasicInformation, &tbi, sizeof(tbi), NULL);
                if (status == STATUS_SUCCESS) {
                    USERMODE_READ_PROCESS_MEMORY ReadMemory{};
                    DWORD ReturnedLength = 0;

                    ReadMemory.Pid = CustomTitleBarGlobalVars::Pid;
                    ReadMemory.Src = tbi.TebBaseAddress;
                    ReadMemory.Dest = &tib;
                    ReadMemory.Size = sizeof(tib);

                    /*bool Status = DeviceIoControl(
                        CustomTitleBarGlobalVars::hDriver,
                        IoCtlCode::ReadProcessMemory,
                        &ReadMemory,
                        sizeof(USERMODE_READ_PROCESS_MEMORY),
                        &ReadMemory,
                        sizeof(USERMODE_READ_PROCESS_MEMORY),
                        &ReturnedLength,
                        NULL);*/
                    ReadProcessMemory(GlobalVarsOfPeTab::objPEInformation->ProcessInfo.hProcess, tbi.TebBaseAddress, &tib, sizeof(tbi), nullptr);//Replace with CALL TO DRIVER 

                    MemoryInfo StackStruct{};
                    StackStruct.BaseAddress = (uint64_t)tib.StackLimit;
                    std::stringstream ss;
                    ss << "Stack (" << std::uppercase << std::hex << te32.th32ThreadID << ")";
                    StackStruct.Info += ss.str();
                    StackStruct.Party = false;
                    StackStruct.Stack = true;
                    //printf("StackStruct.Address: %#p | ThreadId: %#x | StackLimit: %#p\n", StackStruct.BaseAddress, te32.th32ThreadID, tib.StackLimit);

                    vStackInfo.push_back(StackStruct);

                    MemoryInfo TebStruct;
                    TebStruct.CustomAddress.push_back((uint64_t)tbi.TebBaseAddress);
                    std::stringstream ss1;
                    ss1 << "TEB(" << std::uppercase << std::hex << te32.th32ThreadID << ")";
                    TebStruct.Info += ss1.str();
                    TebStruct.Party = false;
                    TebStruct.Teb = true;
                    //printf("TebStruct.Address: %#p | ThreadId: %#x\n", TebStruct.CustomAddress[0], te32.th32ThreadID);

                    vTebInfo.push_back(TebStruct);
                }
                CloseHandle(hThread);
            }
        }
    } while (Thread32Next(hThreadSnap, &te32));
    CloseHandle(hThreadSnap);
    FreeLibrary(hNtDll);

    return nsMemoryParser::Error::AllOk;
}

bool MemPageRightsToString(DWORD Protect, std::string& Memory)
{
    if (!Protect) //reserved pages don't have a protection (https://goo.gl/Izkk0c)
    {
        Memory = '\0';
        return true;
    }
    switch (Protect & 0xFF)
    {
    case PAGE_NOACCESS:
        Memory = "----"; break;
    case PAGE_READONLY:
        Memory = "-R--"; break;
    case PAGE_READWRITE:
        Memory = "-RW-"; break;
    case PAGE_WRITECOPY:
        Memory = "-RWC"; break;
    case PAGE_EXECUTE:
        Memory = "E---"; break;
    case PAGE_EXECUTE_READ:
        Memory = "ER--"; break;
    case PAGE_EXECUTE_READWRITE:
        Memory = "ERW-"; break;
    case PAGE_EXECUTE_WRITECOPY:
        Memory = "ERWC"; break;
    }

    ((Protect & PAGE_GUARD) == PAGE_GUARD) ? Memory += "G" : Memory += "-";
    ((Protect & PAGE_NO_CHANGE) == PAGE_NO_CHANGE) ? Memory += "N" : Memory += "-";

    return true;
}

void MemoryParser_::GetMemoryMapOfUserProcess(bool Cache)
{
    if (Cache) { vMemoryInfo.clear(); }

    SYSTEM_INFO systemInfo; GetSystemInfo(&systemInfo);

    std::vector<MEMORY_BASIC_INFORMATION> memoryRegions;
    size_t address = reinterpret_cast<size_t>(systemInfo.lpMinimumApplicationAddress);
    size_t maxAddress = reinterpret_cast<size_t>(systemInfo.lpMaximumApplicationAddress);

    GetTebAndStackForEachThread();
    //GetHeap();

    uint64_t CounterOfRegions = 0;
    while (address < maxAddress)
    {
        MEMORY_BASIC_INFORMATION memInfo;
        memset(&memInfo, 0, sizeof(MEMORY_BASIC_INFORMATION));
        SIZE_T bytesReturned = VirtualQueryEx(GlobalVarsOfPeTab::objPEInformation->ProcessInfo.hProcess, reinterpret_cast<LPCVOID>(address), &memInfo, sizeof(memInfo));
        
        if (bytesReturned == sizeof(memInfo))
        {
            MemoryInfo Memory;
            Memory.BaseAddress = (uint64_t)memInfo.BaseAddress;
            Memory.Size = memInfo.RegionSize;
            Memory.Type = memInfo.Type;
            switch (memInfo.Type)
            {
            case MEM_PRIVATE: { Memory.szType = "PRV"; break; }
            case MEM_MAPPED: { Memory.szType = "MAP";  break; }
            case MEM_IMAGE: { Memory.szType = "IMG"; break; }
            }
            Memory.Protect = memInfo.Protect;
            MemPageRightsToString(Memory.Protect, Memory.szProtect);
            Memory.State = memInfo.State;
            Memory.InitialProtect = memInfo.AllocationProtect;
            MemPageRightsToString(Memory.InitialProtect, Memory.szInitialProtect);

            for (int StackIndex = 0; StackIndex < vStackInfo.size(); ++StackIndex)
            {
                if (vStackInfo[StackIndex].BaseAddress >= Memory.BaseAddress && vStackInfo[StackIndex].BaseAddress < Memory.BaseAddress + Memory.Size)
                {
                    Memory.CustomAddress.push_back(vStackInfo[StackIndex].BaseAddress);
                    Memory.Info.size() == 0 ? Memory.Info += vStackInfo[StackIndex].Info : Memory.Info += ", " + vStackInfo[StackIndex].Info;
                }
            }

            for (int TebIndex = 0; TebIndex < vTebInfo.size(); ++TebIndex)
            {
                if (vTebInfo[TebIndex].CustomAddress[0] >= Memory.BaseAddress && vTebInfo[TebIndex].CustomAddress[0] < Memory.BaseAddress + Memory.Size)
                {
                    Memory.CustomAddress.push_back(vTebInfo[TebIndex].CustomAddress[0]);
                    Memory.Info.size() == 0 ? Memory.Info += vTebInfo[TebIndex].Info : Memory.Info += ", " + vTebInfo[TebIndex].Info;
                }
            }

            if (memInfo.Type == MEM_MAPPED)
            {
                char szName[MAX_PATH];
                if (K32GetMappedFileNameA(GlobalVarsOfPeTab::objPEInformation->ProcessInfo.hProcess, (PVOID)memInfo.BaseAddress, szName, sizeof(szName)))
                {
                    Memory.Info = szName;
                }
            }

            for (int AddrsHeapIndex = 0; AddrsHeapIndex < vHeapAddrs.size(); ++AddrsHeapIndex)
            {
                if (vHeapAddrs[AddrsHeapIndex] >= Memory.BaseAddress && vHeapAddrs[AddrsHeapIndex] < Memory.BaseAddress + Memory.Size)
                {
                    Memory.Info += "Heap";
                    vHeapInfo.push_back(Memory);
                }
            }
            if (Memory.BaseAddress == KUSER_SHARED_DATA)
            {
                Memory.Info += "KUSER_SHARED_DATA";
            }
            if (memInfo.State == MEM_RESERVE && memInfo.Type == MEM_PRIVATE && !memInfo.Protect)
            {
                if (memInfo.AllocationBase != memInfo.BaseAddress)
                {
                    std::stringstream ss;
                    ss << "Reserved(" << std::uppercase << std::hex << memInfo.AllocationBase << ")";
                    Memory.Info += ss.str();
                }
                else
                {
                    Memory.Info += "Reserved";
                }
            }

            if (memInfo.Type == MEM_IMAGE) {
                HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, CustomTitleBarGlobalVars::Pid);
                MODULEENTRY32 moduleEntry;
                if (snapshot != INVALID_HANDLE_VALUE) {
                    moduleEntry.dwSize = sizeof(moduleEntry);
                    if (Module32First(snapshot, &moduleEntry)) {
                        do {
                            if (memInfo.BaseAddress == moduleEntry.modBaseAddr) {
                                Memory.FullExePath = moduleEntry.szExePath;
                                Memory.Info = Memory.FullExePath.filename().string();
                                std::size_t found = Memory.FullExePath.string().find(".exe");
                                if (found == std::string::npos)
                                {
                                    HMODULE hFile = LoadLibraryW(Memory.FullExePath.wstring().c_str());
                                    if (hFile != NULL) { Memory.Party = true; }
                                    else { Memory.Party = false; }
                                }
                                else { Memory.Party = false; }
                                vMemoryInfo.push_back(Memory);

                                break;
                            }
                        } while (Module32Next(snapshot, &moduleEntry));

                        static uint64_t lCounterOfRegionsIndex = 0;
                        static _DataFromSections DataFromSections;
                        if (Memory.FullExePath != "")
                        {
                            lCounterOfRegionsIndex = CounterOfRegions + 1;

                            DataFromSections.vNamesOfSections.clear();
                            DataFromSections.vVirtualSizes.clear();

                            auto L_inspectSection = [](void* N,
                                const peparse::VA& secBase,
                                const std::string& secName,
                                const peparse::image_section_header& SectionHeader,
                                const peparse::bounded_buffer* data) -> int {
                                    static_cast<void>(secBase);
                                    static_cast<void>(secName);
                                    static_cast<void>(data);
                                    auto DataFromSections = static_cast<_DataFromSections*>(N);

                                    std::string sectionName((char*)SectionHeader.Name);
                                    if (sectionName.size() > 8) { sectionName = sectionName.substr(0, 8); }

                                    DataFromSections->vNamesOfSections.push_back(sectionName);
                                    DataFromSections->vVirtualSizes.push_back(ALIGN_TO_4KB(SectionHeader.Misc.VirtualSize));

                                    return 0;
                                };

                            std::size_t found = Memory.FullExePath.string().find(".exe");
                            if (found != std::string::npos)
                            {
                                if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                                { DataFromSections.vVirtualSizes.push_back(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.BaseOfCode); }
                                else { DataFromSections.vVirtualSizes.push_back(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.BaseOfCode); }

                                peparse::IterSec(GlobalVarsOfPeTab::objPEInformation->Pe, L_inspectSection, &DataFromSections);
                                DataFromSections.CountOfSections = GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections;
                            }
                            else
                            {
                                auto Pe = OpenExecutable(Memory.FullExePath.string());

                                if (Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) 
                                { DataFromSections.vVirtualSizes.push_back(Pe->peHeader.nt.OptionalHeader64.BaseOfCode); }
                                else { DataFromSections.vVirtualSizes.push_back(Pe->peHeader.nt.OptionalHeader.BaseOfCode); }

                                peparse::IterSec(Pe.get(), L_inspectSection, &DataFromSections);
                                DataFromSections.CountOfSections = Pe->peHeader.nt.FileHeader.NumberOfSections;
                            }
                        }

                        static uint64_t lAllocationBase = 0;
                        if (Memory.FullExePath != "" && lAllocationBase != (uint64_t)memInfo.AllocationBase)
                        {
                            lAllocationBase = (uint64_t)memInfo.AllocationBase;
                            for (int Index = 0; Index < DataFromSections.CountOfSections; ++Index)
                            {
                                Memory.szInitialProtect.clear();
                                Memory.szProtect.clear();
                                for (auto it = vMemoryInfo.rbegin(); it != vMemoryInfo.rend(); ++it)
                                {
                                    if (it->FullExePath.string().size() > 0) { Memory.Party = it->Party; break; }
                                }
                                Memory.BaseAddress = (uint64_t)Memory.BaseAddress + DataFromSections.vVirtualSizes[Index];
                                Memory.Size = DataFromSections.vVirtualSizes[Index];
                                Memory.Info = DataFromSections.vNamesOfSections[Index];

                                MEMORY_BASIC_INFORMATION TempMemInfo;
                                memset(&TempMemInfo, 0, sizeof(MEMORY_BASIC_INFORMATION));
                                VirtualQueryEx(GlobalVarsOfPeTab::objPEInformation->ProcessInfo.hProcess, reinterpret_cast<LPCVOID>(Memory.BaseAddress), &TempMemInfo, sizeof(TempMemInfo));

                                Memory.Type = TempMemInfo.Type;
                                Memory.Protect = TempMemInfo.Protect;
                                MemPageRightsToString(Memory.Protect, Memory.szProtect);
                                Memory.InitialProtect = TempMemInfo.AllocationProtect;
                                MemPageRightsToString(Memory.InitialProtect, Memory.szInitialProtect);
                                Memory.State = TempMemInfo.State;

                                auto it = std::find(Contents.first.begin(), Contents.first.end(), DataFromSections.vNamesOfSections[Index]);
                                if (it != std::end(Contents.first)) { Memory.Content = Contents.second[it - Contents.first.begin()]; }
                                else { Memory.Content = ""; }

                                vMemoryInfo.push_back(Memory);
                            }
                        }
                    }
                }
            }
            else if (memInfo.Type != NULL && memInfo.State != NULL) { vMemoryInfo.push_back(Memory); }
            address = reinterpret_cast<size_t>(memInfo.BaseAddress) + memInfo.RegionSize;
            ++CounterOfRegions;
        }
        else { break; }
    }

    vHeapAddrs.clear();
    vHeapInfo.clear();
    vStackInfo.clear();
    vTebInfo.clear();
    //for (int MemIndex = 0; MemIndex < vMemoryInfo.size(); ++MemIndex)
    //{
    //    printf("Type: %s | Party: %#p | Addr: %#p | Size: %#p | Info: %s | Content: %s | Protect: %s | Initial protection: %s\n", vMemoryInfo[MemIndex].szType.c_str(), vMemoryInfo[MemIndex].Party, vMemoryInfo[MemIndex].BaseAddress, vMemoryInfo[MemIndex].Size, vMemoryInfo[MemIndex].Info.c_str(), vMemoryInfo[MemIndex].Content.c_str(), vMemoryInfo[MemIndex].szProtect.c_str(), vMemoryInfo[MemIndex].szInitialProtect.c_str());
    //}

    return;
}