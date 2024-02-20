#include "GUI/MenuOfMainTabs/CallStackMenuBegin/CallStack.hpp"

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

void ExternalStackWalker::OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry)
{
    CallstackEntry* newEntry = new CallstackEntry(entry);
    vCallStackInfo.push_back(newEntry);
}

void CallStackParser_::EnsureUniqueOffsets()
{
    auto last = std::unique(vCallStackInfo.begin(), vCallStackInfo.end(),
        [](const CallstackEntry* a, const CallstackEntry* b) {
            return a->stack_frame.AddrFrame.Offset == b->stack_frame.AddrFrame.Offset;
        });

    vCallStackInfo.erase(last, vCallStackInfo.end());
}

void CallStackParser_::GetCallStackOfUserProcess()
{
    static size_t Index = 0;
    ExternalStackWalker walker;

    NTSTATUS Status; ULONG size;
    if (NtQuerySystemInformation(SystemProcessInformation, NULL, 0, &size) != STATUS_INFO_LENGTH_MISMATCH) { return; }

    PSYSTEM_PROCESS_INFORMATION spi = new SYSTEM_PROCESS_INFORMATION[2 * size];//PSYSTEM_PROCESS_INFORMATION(VirtualAlloc(NULL, 2 * size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    RtlZeroMemory(spi, 2 * size);
    PSYSTEM_PROCESS_INFORMATION StartSpi = spi;
    if (!NT_SUCCESS(Status = NtQuerySystemInformation(SystemProcessInformation, spi, 2 * size, NULL)))
    {
        delete[] StartSpi;
        return;
    }

    while (true) // Loop over the list until we reach the last entry.
    {
        if ((DWORD)spi->UniqueProcessId == GlobalVarsOfPeTab::objPEInformation->ProcessInfo.dwProcessId)
        {
            for (int ThreadIndex = 0; ThreadIndex < spi->NumberOfThreads; ++ThreadIndex)
            {
                HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, (DWORD)spi->Threads[ThreadIndex].ClientId.UniqueThread);
                if (hThread != NULL)
                {
                    walker.SetTargetProcess(GlobalVarsOfPeTab::objPEInformation->ProcessInfo.dwProcessId, GlobalVarsOfPeTab::objPEInformation->ProcessInfo.hProcess);
                    walker.ShowCallstack(hThread);
                    
                    for (; Index < walker.vCallStackInfo.size(); ++Index) { walker.vCallStackInfo[Index]->stack_frame.KdHelp.Thread = (DWORD64)spi->Threads[ThreadIndex].ClientId.UniqueThread; }
                    CloseHandle(hThread);
                }
            }
        }
        if (spi->NextEntryOffset == 0) { break; }
        spi = PSYSTEM_PROCESS_INFORMATION((LPBYTE)spi + spi->NextEntryOffset);
    }
    delete[] StartSpi;

    vCallStackInfo = walker.vCallStackInfo;
    EnsureUniqueOffsets();

    Index = 0;

    return;
}