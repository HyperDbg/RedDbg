#pragma once
#include "StackWalker.h"
#include "GUI/MenuOfMainTabs/PeMenuBegin/Parser/PEReader.hpp"

#include <windows.h>
#include <psapi.h>
#include <filesystem>
#include <memory>
#include <TlHelp32.h>
#include <intsafe.h>

#include "Userntdll.h"

enum
{
    STACKWALK_MAX_NAMELEN = 1024
};

typedef struct CallstackEntry
{
    DWORD64 offset; // if 0, we have no valid entry
    STACKFRAME64 stack_frame;
    CHAR    name[STACKWALK_MAX_NAMELEN];
    CHAR    undName[STACKWALK_MAX_NAMELEN];
    CHAR    undFullName[STACKWALK_MAX_NAMELEN];
    DWORD64 offsetFromSmybol;
    DWORD   offsetFromLine;
    DWORD   lineNumber;
    CHAR    lineFileName[STACKWALK_MAX_NAMELEN];
    DWORD   symType;
    LPCSTR  symTypeString;
    CHAR    moduleName[STACKWALK_MAX_NAMELEN];
    DWORD64 baseOfImage;
    CHAR    loadedImageName[STACKWALK_MAX_NAMELEN];
} CallstackEntry;

class CallStackCache : private StackWalker {
public:
    std::chrono::steady_clock::time_point LastUpdated;
    std::vector<CallstackEntry*> vCallStackInfo;
};

class ExternalStackWalker : public StackWalker
{
public:
    std::vector<CallstackEntry*> vCallStackInfo;
protected:
    virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry);
};

class CallStackParser_ : private StackWalker
{
private:
    std::vector<CallstackEntry*> vCallStackInfo;
private:
    void GetCallStackOfUserProcess(bool Cache);
    void EnsureUniqueOffsets();
public:
    CallStackCache Cache;

    CallStackParser_() {
        Cache.LastUpdated = std::chrono::steady_clock::now();
        GetCallStackOfUserProcess(false);
        Cache.vCallStackInfo.resize(vCallStackInfo.size());
        std::copy(vCallStackInfo.begin(), vCallStackInfo.end(), Cache.vCallStackInfo.begin());
    }

    void UpdateCallStackCache(std::shared_ptr<std::atomic<bool>> Active) {
        Cache.LastUpdated = std::chrono::steady_clock::now();
        GetCallStackOfUserProcess(true);
        while (Active->load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Sleep for a short duration to avoid busy waiting
        }
        Cache.vCallStackInfo.resize(vCallStackInfo.size());
        std::copy(vCallStackInfo.begin(), vCallStackInfo.end(), Cache.vCallStackInfo.begin());
        return;
    }
};