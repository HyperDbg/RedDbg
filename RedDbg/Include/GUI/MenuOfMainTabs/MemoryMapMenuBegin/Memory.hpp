#pragma once
#include "AppToDrv/MiddleStructs.hpp"
#include "AppToDrv/IoCtlCodes.hpp"
#include "GUI/MenuOfMainTabs/PeMenuBegin/Parser/PEReader.hpp"
#include <windows.h>
#include <psapi.h>
#include <filesystem>
#include <TlHelp32.h>
#include <omp.h>
#include <intsafe.h>
#include "Userntdll.h"

#define ALIGN_TO_4KB(size) (((size) + 4095) / 4096) * 4096
#define KUSER_SHARED_DATA 0x7FFE0000
#define PAGE_NO_CHANGE 0x00400000

typedef NTSTATUS(NTAPI* NTQUERYINFORMATIONTHREAD)(
    HANDLE ThreadHandle,
    THREADINFOCLASS ThreadInformationClass,
    PVOID ThreadInformation,
    ULONG ThreadInformationLength,
    PULONG ReturnLength OPTIONAL
    );

using ParsedPeRefDll =
std::unique_ptr<peparse::parsed_pe, void (*)(peparse::parsed_pe*)>;

namespace nsMemoryParser
{
    typedef enum _Party { User, System } Party;

    typedef enum _Type { Image, Mapped, Private } Type;

    typedef enum _State { Commit, Free, Reserve } State;

    typedef enum class _Error {
        hNtDllIsNull,
        NtQueryInformationThreadIsNull,
        hThreadSnapHasInvalidHandleValue,
        FailedToGetFirstThread,
        HeapDoesntExist,
        ResultIsntS_OK,
        hDefaultProcessHeapIsNull,
        AllOk
    } Error;
}

typedef struct _MemoryInfo {
    std::vector<uint64_t> CustomAddress;
    uint64_t BaseAddress = 0;
    size_t Size = 0;
    bool Party = false;
    std::string Info;
    std::filesystem::path FullExePath;
    std::string Content;
    uint32_t Type = 0;
    std::string szType;
    uint32_t Protect = 0;
    std::string szProtect;
    uint32_t State = 0;
    uint32_t InitialProtect = 0;
    std::string szInitialProtect;

    bool Stack = false;
    bool Heap = false;
    bool Peb = false;
    bool Teb = false;
    bool Reserved = false;

} MemoryInfo, * PMemoryInfo;

typedef struct _Found {
    bool Found = false;
    std::vector<MemoryInfo>::const_iterator it;
} Found;

typedef struct _MemoryCache {
    std::chrono::steady_clock::time_point LastUpdated;
    std::vector<MemoryInfo> vMemoryInfo;
} MemoryCache, *PMemoryCache;

struct _DataFromSections final {
    uint64_t CountOfSections = 0;
    std::vector<size_t> vVirtualSizes;
    std::vector<std::string> vNamesOfSections;
    //std::vector<uint32_t> SectionCharacteristics;
};

class MemoryParser_
{
private:
    std::vector<std::string> InformationContent = 
    { "Uninitialized data",
        "Initialized data",
        "Export tables",
        "Import tables",
        "Exception information",
        "Read-only initialized data",
        "Base relocations",
        "Resources",
        "Executable code",
        "Thread-local storage",
        "Exception information"
    };
    std::vector<std::string> SectionNames =
    {
        ".bss",
        ".data",
        ".edata",
        ".idata",
        ".pdata",
        ".rdata",
        ".reloc",
        ".rsrc",
        ".text",
        ".tls",
        ".xdata"
    };
    std::pair<std::vector<std::string>, std::vector<std::string>> Contents = { SectionNames, InformationContent };

    std::string Ntdll = "ntdll.dll";

    std::vector<MemoryInfo> vTebInfo;
    std::vector<MemoryInfo> vStackInfo;
    std::vector<MemoryInfo> vHeapInfo;
    std::vector<uint64_t> vHeapAddrs;
private:
    ParsedPeRefDll OpenExecutable(std::string path) noexcept;
    void GetMemoryMapOfUserProcess(bool Cache);
    nsMemoryParser::Error GetTebAndStackForEachThread();
    nsMemoryParser::Error GetHeap();

public:
    std::vector<MemoryInfo> vMemoryInfo;
    MemoryCache Cache;

    MemoryParser_() {
        Cache.LastUpdated = std::chrono::steady_clock::now();
        GetMemoryMapOfUserProcess(false);
        Cache.vMemoryInfo.resize(vMemoryInfo.size());
        std::copy(vMemoryInfo.begin(), vMemoryInfo.end(), Cache.vMemoryInfo.begin());
        //Cache.vMemoryInfo = vMemoryInfo;
    }

    void UpdateMemoryCache(std::shared_ptr<std::atomic<bool>> Active) {
        Cache.LastUpdated = std::chrono::steady_clock::now();
        GetMemoryMapOfUserProcess(true);
        while (Active->load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Sleep for a short duration to avoid busy waiting
        }
        Cache.vMemoryInfo.resize(vMemoryInfo.size());
        std::copy(vMemoryInfo.begin(), vMemoryInfo.end(), Cache.vMemoryInfo.begin());
        //Cache.vMemoryInfo = vMemoryInfo;
        return;
    }
};