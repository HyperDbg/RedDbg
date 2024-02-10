#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/Threads.hpp"

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

// Helper function to convert LARGE_INTEGER to milliseconds
void ThreadParser_::LargeIntegerToFileTime(const LARGE_INTEGER& li, FILETIME& ft) {
    ft.dwLowDateTime = li.LowPart;
    ft.dwHighDateTime = li.HighPart;
}

// Format : d:hh:mm:ss.1234567
std::string ThreadParser_::FormatTime(const FILETIME& time) {
    // FILETIME is in  100ns intervals
    uint64_t time100ns = ((uint64_t)time.dwHighDateTime << 32) | (uint64_t)time.dwLowDateTime;
    uint64_t milliseconds = time100ns / 10000; // Convert to milliseconds

    // Calculate hours, minutes, seconds, and milliseconds
    milliseconds %= (1000 * 60 * 60 * 24);
    uint64_t hours = milliseconds / (1000 * 60 * 60);
    milliseconds %= (1000 * 60 * 60);
    uint64_t minutes = milliseconds / (1000 * 60);
    milliseconds %= (1000 * 60);
    uint64_t seconds = milliseconds / 1000;
    uint64_t remainingMilliseconds = milliseconds % 1000;

    SYSTEMTIME st;
    GetLocalTime(&st);

    long diff_hours = st.wHour - (hours % 24);
    if (diff_hours > 12) diff_hours -= 24;
    else if (diff_hours < -12) diff_hours += 24;

    if (hours > 0) { hours += diff_hours; }

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << hours << ":"
        << std::setw(2) << minutes << ":"
        << std::setw(2) << seconds << "."
        << std::setw(7) << remainingMilliseconds;

    return oss.str();
}

void ThreadParser_::GetThreadsOfUserProcess(bool Cache)
{
	if (Cache) { vThreadInfo.clear(); }

    NTSTATUS Status; ULONG size;
    if (NtQuerySystemInformation(SystemProcessInformation, NULL, 0, &size) != STATUS_INFO_LENGTH_MISMATCH) { return; }

    PSYSTEM_PROCESS_INFORMATION spi = PSYSTEM_PROCESS_INFORMATION(VirtualAlloc(NULL, 2 * size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
    if (!NT_SUCCESS(Status = NtQuerySystemInformation(SystemProcessInformation, spi, 2 * size, NULL)))
    {
        printf("\nError: Unable to query process list (%#x)\n", Status);
        VirtualFree(spi, 0, MEM_RELEASE);
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
                    NT_TIB tib = { 0 };
                    THREAD_BASIC_INFORMATION tbi = { 0 };

                    NTSTATUS status = NtQueryInformationThread(hThread, (THREADINFOCLASS)ThreadBasicInformation, &tbi, sizeof(tbi), NULL);
                    if (status == STATUS_SUCCESS) 
                    {
                        ReadProcessMemory(GlobalVarsOfPeTab::objPEInformation->ProcessInfo.hProcess, tbi.TebBaseAddress, &tib, sizeof(tbi), nullptr);

                        ThreadInfo ThreadStruct;
                        ThreadStruct.Thread.ThreadInfo = spi->Threads[ThreadIndex];
                        ThreadStruct.TebBase = (uint64_t)tbi.TebBaseAddress;
                        ThreadStruct.Thread.TebBase = PTEB(VirtualAlloc(NULL, sizeof(*ThreadStruct.Thread.TebBase), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
                        ReadProcessMemory(
                            GlobalVarsOfPeTab::objPEInformation->ProcessInfo.hProcess, 
                            tbi.TebBaseAddress,
                            ThreadStruct.Thread.TebBase, 
                            sizeof(*ThreadStruct.Thread.TebBase), nullptr);
                        ThreadStruct.Thread.StackBase = tib.StackBase;
                        ThreadStruct.Thread.StackLimit = tib.StackLimit;

                        NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &ThreadStruct.Thread.Win32StartAddress, sizeof(ThreadStruct.Thread.Win32StartAddress), NULL);
                        NtQueryInformationThread(hThread, ThreadSuspendCount, &ThreadStruct.SuspendCount, sizeof(ThreadStruct.SuspendCount), NULL);
                        
                        QueryThreadCycleTime(hThread, &ThreadStruct.Cycles);
                        if (ThreadIndex == 0) { ThreadStruct.Name = "Main Thread"; } else { ThreadStruct.Name = ""; }
                      
                        {
                        if (ThreadStruct.Thread.ThreadInfo.Priority >= _PriorityIdle && ThreadStruct.Thread.ThreadInfo.Priority < _PriorityAboveNormal)
                            ThreadStruct.szPriority = "Idle";
                        else if (ThreadStruct.Thread.ThreadInfo.Priority >= _PriorityAboveNormal && ThreadStruct.Thread.ThreadInfo.Priority < _PriorityBelowNormal)
                            ThreadStruct.szPriority = "AboveNormal";
                        else if (ThreadStruct.Thread.ThreadInfo.Priority >= _PriorityBelowNormal && ThreadStruct.Thread.ThreadInfo.Priority < _PriorityNormal)
                            ThreadStruct.szPriority = "BelowNormal";
                        else if (ThreadStruct.Thread.ThreadInfo.Priority >= _PriorityNormal && ThreadStruct.Thread.ThreadInfo.Priority < _PriorityHighest)
                            ThreadStruct.szPriority = "Normal";
                        else if (ThreadStruct.Thread.ThreadInfo.Priority >= _PriorityHighest && ThreadStruct.Thread.ThreadInfo.Priority < _PriorityLowest)
                            ThreadStruct.szPriority = "Highest";
                        else if (ThreadStruct.Thread.ThreadInfo.Priority >= _PriorityLowest && ThreadStruct.Thread.ThreadInfo.Priority < _PriorityNormal)
                            ThreadStruct.szPriority = "Lowest";
                        else if (ThreadStruct.Thread.ThreadInfo.Priority >= _PriorityNormal && ThreadStruct.Thread.ThreadInfo.Priority <= _PriorityTimeCritical)
                            ThreadStruct.szPriority = "Normal";
                        else if (ThreadStruct.Thread.ThreadInfo.Priority > _PriorityTimeCritical)
                            ThreadStruct.szPriority = "TimeCritical";
                        else
                            ThreadStruct.szPriority = "Unknown";
                        }
                        FILETIME ftKernel, ftUser, ftCreate;
                        LargeIntegerToFileTime(ThreadStruct.Thread.ThreadInfo.KernelTime, ftKernel);
                        LargeIntegerToFileTime(ThreadStruct.Thread.ThreadInfo.UserTime, ftUser);
                        LargeIntegerToFileTime(ThreadStruct.Thread.ThreadInfo.CreateTime, ftCreate);

                        ThreadStruct.szKernelTime = FormatTime(ftKernel); 
                        ThreadStruct.szUserTime = FormatTime(ftUser);
                        ThreadStruct.szCreationTime = FormatTime(ftCreate);

                        switch (ThreadStruct.Thread.ThreadInfo.WaitReason)
                        {
                        case _Executive: ThreadStruct.szWaitReason = "Executive"; break;
                        case _FreePage: ThreadStruct.szWaitReason = "FreePage"; break;
                        case _PageIn: ThreadStruct.szWaitReason = "PageIn"; break;
                        case _PoolAllocation: ThreadStruct.szWaitReason = "PoolAllocation"; break;
                        case _DelayExecution: ThreadStruct.szWaitReason = "DelayExecution"; break;
                        case _Suspended: ThreadStruct.szWaitReason = "Suspended"; break;
                        case _UserRequest: ThreadStruct.szWaitReason = "UserRequest"; break;
                        case _WrExecutive: ThreadStruct.szWaitReason = "WrExecutive"; break;
                        case _WrFreePage: ThreadStruct.szWaitReason = "WrFreePage"; break;
                        case _WrPageIn: ThreadStruct.szWaitReason = "WrPageIn"; break;
                        case _WrPoolAllocation: ThreadStruct.szWaitReason = "WrPoolAllocation"; break;
                        case _WrDelayExecution: ThreadStruct.szWaitReason = "WrDelayExecution"; break;
                        case _WrSuspended: ThreadStruct.szWaitReason = "WrSuspended"; break;
                        case _WrUserRequest: ThreadStruct.szWaitReason = "WrUserRequest"; break;
                        case _WrEventPair: ThreadStruct.szWaitReason = "WrEventPair"; break;
                        case _WrQueue: ThreadStruct.szWaitReason = "WrQueue"; break;
                        case _WrLpcReceive: ThreadStruct.szWaitReason = "WrLpcReceive"; break;
                        case _WrLpcReply: ThreadStruct.szWaitReason = "WrLpcReply"; break;
                        case _WrVirtualMemory: ThreadStruct.szWaitReason = "WrVirtualMemory"; break;
                        case _WrPageOut: ThreadStruct.szWaitReason = "WrPageOut"; break;
                        case _WrRendezvous: ThreadStruct.szWaitReason = "WrRendezvous"; break;
                        case _Spare2: ThreadStruct.szWaitReason = "Spare2"; break;
                        case _Spare3: ThreadStruct.szWaitReason = "Spare3"; break;
                        case _Spare4: ThreadStruct.szWaitReason = "Spare4"; break;
                        case _Spare5: ThreadStruct.szWaitReason = "Spare5"; break;
                        case _WrCalloutStack: ThreadStruct.szWaitReason = "WrCalloutStack"; break;
                        case _WrKernel: ThreadStruct.szWaitReason = "WrKernel"; break;
                        case _WrResource: ThreadStruct.szWaitReason = "WrResource"; break;
                        case _WrPushLock: ThreadStruct.szWaitReason = "WrPushLock"; break;
                        case _WrMutex: ThreadStruct.szWaitReason = "WrMutex"; break;
                        case _WrQuantumEnd: ThreadStruct.szWaitReason = "WrQuantumEnd"; break;
                        case _WrDispatchInt: ThreadStruct.szWaitReason = "WrDispatchInt"; break;
                        case _WrPreempted: ThreadStruct.szWaitReason = "WrPreempted"; break;
                        case _WrYieldExecution: ThreadStruct.szWaitReason = "WrYieldExecution"; break;
                        case _WrFastMutex: ThreadStruct.szWaitReason = "WrFastMutex"; break;
                        case _WrGuardedMutex: ThreadStruct.szWaitReason = "WrGuardedMutex"; break;
                        case _WrRundown: ThreadStruct.szWaitReason = "WrRundown"; break;
                        default: ThreadStruct.szWaitReason = "Unknown"; break;
                        }

                        vThreadInfo.push_back(ThreadStruct);
                    }
                }
                CloseHandle(hThread);
            }
        }
        if (spi->NextEntryOffset == 0) { break; }
        spi = PSYSTEM_PROCESS_INFORMATION((LPBYTE)spi + spi->NextEntryOffset);
    }
    VirtualFree(spi, 0, MEM_RELEASE);

    //for (size_t Index = 0; Index < vThreadInfo.size(); ++Index)
    //{
    //    FILETIME ftKernel, ftUser, ftCreate;
    //    LargeIntegerToFileTime(vThreadInfo[Index].Thread.ThreadInfo.KernelTime, ftKernel);
    //    LargeIntegerToFileTime(vThreadInfo[Index].Thread.ThreadInfo.UserTime, ftUser);
    //    LargeIntegerToFileTime(vThreadInfo[Index].Thread.ThreadInfo.CreateTime, ftCreate);
    //    printf("Number: %X | ID: %X | Entry: %#p && Win32StartAddr: %#p | TEB: %#p | Suspend Count: %X | Priority: %s | Wait Reason: %s | Last Error: %X | CPU Cycles: %#p | Name: %s\n",
    //        Index, vThreadInfo[Index].Thread.ThreadInfo.ClientId.UniqueThread, 
    //        vThreadInfo[Index].Thread.ThreadInfo.StartAddress, vThreadInfo[Index].Thread.Win32StartAddress, 
    //        vThreadInfo[Index].TebBase, vThreadInfo[Index].SuspendCount, vThreadInfo[Index].szPriority.c_str(), vThreadInfo[Index].szWaitReason.c_str(), vThreadInfo[Index].Thread.TebBase->LastErrorValue,
    //        vThreadInfo[Index].Cycles, vThreadInfo[Index].Name.c_str());
    //    printf("Kt: %s | Ut: %s | Ct: %s\n", FormatTime(ftKernel).c_str(), FormatTime(ftUser).c_str(), FormatTime(ftCreate).c_str());
    //}

	return;
}