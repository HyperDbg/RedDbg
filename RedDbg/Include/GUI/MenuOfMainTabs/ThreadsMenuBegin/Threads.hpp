#pragma once
#include <functional>
#include "GUI/MenuOfMainTabs/PeMenuBegin/Parser/PEReader.hpp"
#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/SystemThread.hpp"
#include <windows.h>
#include <psapi.h>
#include <filesystem>
#include <TlHelp32.h>
#include <intsafe.h>
#include "Userntdll.h"

typedef struct _ThreadInfo {
	SYSTEM_EXTENDED_THREAD_INFORMATION Thread{};
	uint64_t Cycles = 0;
	uint64_t SuspendCount = 0;
	uint64_t TebBase = 0;
    uint64_t Rip = 0;
	std::string szPriority;
	std::string szWaitReason;
	std::string Name;
    std::string szKernelTime;
    std::string szUserTime;
    std::string szCreationTime;
} ThreadInfo, * PThreadInfo;

typedef struct _ThreadCache {
	std::chrono::steady_clock::time_point LastUpdated;
    std::vector<PTEB> PTebs;
	std::vector<ThreadInfo> vThreadInfo;
} ThreadCache, * PThreadCache;

typedef enum
{
	_PriorityIdle = -15,
	_PriorityAboveNormal = 1,
	_PriorityBelowNormal = -1,
	_PriorityHighest = 2,
	_PriorityLowest = -2,
	_PriorityNormal = 0,
	_PriorityTimeCritical = 15,
	_PriorityUnknown = 0x7FFFFFFF
} THREADPRIORITY;

typedef enum
{
    _Executive = 0,
    _FreePage = 1,
    _PageIn = 2,
    _PoolAllocation = 3,
    _DelayExecution = 4,
    _Suspended = 5,
    _UserRequest = 6,
    _WrExecutive = 7,
    _WrFreePage = 8,
    _WrPageIn = 9,
    _WrPoolAllocation = 10,
    _WrDelayExecution = 11,
    _WrSuspended = 12,
    _WrUserRequest = 13,
    _WrEventPair = 14,
    _WrQueue = 15,
    _WrLpcReceive = 16,
    _WrLpcReply = 17,
    _WrVirtualMemory = 18,
    _WrPageOut = 19,
    _WrRendezvous = 20,
    _Spare2 = 21,
    _Spare3 = 22,
    _Spare4 = 23,
    _Spare5 = 24,
    _WrCalloutStack = 25,
    _WrKernel = 26,
    _WrResource = 27,
    _WrPushLock = 28,
    _WrMutex = 29,
    _WrQuantumEnd = 30,
    _WrDispatchInt = 31,
    _WrPreempted = 32,
    _WrYieldExecution = 33,
    _WrFastMutex = 34,
    _WrGuardedMutex = 35,
    _WrRundown = 36,
} THREADWAITREASON;

class ThreadParser_
{
private:
	std::vector<ThreadInfo> vThreadInfo;
private:
    void GetThreadsOfUserProcessCallBack(PSYSTEM_PROCESS_INFORMATION spi);
    //std::string FormatTime(const FILETIME& time);
    //void LargeIntegerToFileTime(const LARGE_INTEGER& li, FILETIME& ft);
public:
    ThreadCache Cache;

	ThreadParser_() {
		Cache.LastUpdated = std::chrono::steady_clock::now();
        Thread::GetThreadsOfUserProcess(std::bind(&ThreadParser_::GetThreadsOfUserProcessCallBack, this, std::placeholders::_1));
        Cache.vThreadInfo = vThreadInfo;
	}

    void UpdateThreadCache(std::shared_ptr<std::atomic<bool>> Active) {
        std::vector<PTEB> PTebs;
        Cache.LastUpdated = std::chrono::steady_clock::now();
        for (auto& Thread : vThreadInfo) { PTebs.push_back(Thread.Thread.TebBase); }
        vThreadInfo.clear();
        Thread::GetThreadsOfUserProcess(std::bind(&ThreadParser_::GetThreadsOfUserProcessCallBack, this, std::placeholders::_1));
        while (Active->load()) { continue;/*std::this_thread::sleep_for(std::chrono::milliseconds(5)); Sleep for a short duration to avoid busy waiting*/ }
        Cache.vThreadInfo = vThreadInfo;
        for (auto& PTeb : PTebs)
        {
            VirtualFree(PTeb, 0, MEM_RELEASE);
        }
		return;
	}
};