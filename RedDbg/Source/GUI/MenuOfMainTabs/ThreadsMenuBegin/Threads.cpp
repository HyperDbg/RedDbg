#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/Threads.hpp"

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

void ThreadParser_::GetThreadsOfUserProcessCallBack(PSYSTEM_PROCESS_INFORMATION spi)
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

                    CONTEXT Context{};
                    Context.ContextFlags = CONTEXT_CONTROL;
                    GetThreadContext(hThread, &Context);
                    ThreadStruct.Rip = Context.Rip;
                    
                    NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &ThreadStruct.Thread.Win32StartAddress, sizeof(ThreadStruct.Thread.Win32StartAddress), NULL);
                    NtQueryInformationThread(hThread, ThreadSuspendCount, &ThreadStruct.SuspendCount, sizeof(ThreadStruct.SuspendCount), NULL);

                    QueryThreadCycleTime(hThread, &ThreadStruct.Cycles);
                    if (ThreadIndex == 0) { ThreadStruct.Name = "Main Thread"; }
                    else { ThreadStruct.Name = ""; }

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
                    Thread::LargeIntegerToFileTime(ThreadStruct.Thread.ThreadInfo.KernelTime, ftKernel);
                    Thread::LargeIntegerToFileTime(ThreadStruct.Thread.ThreadInfo.UserTime, ftUser);
                    Thread::LargeIntegerToFileTime(ThreadStruct.Thread.ThreadInfo.CreateTime, ftCreate);

                    ThreadStruct.szKernelTime = Thread::ThreadFormatTime(ftKernel);
                    ThreadStruct.szUserTime = Thread::ThreadFormatTime(ftUser);
                    ThreadStruct.szCreationTime = Thread::ThreadFormatTime(ftCreate);

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
                    CloseHandle(hThread);
                }
            }
        }
    }
}