#pragma once
#include "GUI/MenuDispatcherTabs/GlobalEnums.hpp"
#include "GUI/MenuOfMainTabs/PeMenuBegin/Parser/PEReader.hpp"
#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/SystemThread.hpp"
#include "GUI/MenuOfMainTabs/MemoryMapMenuBegin/SystemMemory.hpp"

#include "Zydis/Zydis.h"
#include "Zycore/Format.h"
#include "Zycore/LibC.h"

#include <windows.h>
#include <psapi.h>
#include <filesystem>
#include <TlHelp32.h>
#include <inttypes.h>
#include <intsafe.h>
#include <memory>
#include "Userntdll.h"

#define ALIGN_TO_4KB(size) (((size) + 4095) / 4096) * 4096

using ParsedPeRefDll =
std::unique_ptr<peparse::parsed_pe, void (*)(peparse::parsed_pe*)>;

typedef struct ZydisSymbol_
{
	/**
	 * The symbol address.
	 */
	ZyanU64 address;
	/**
	 * The symbol name.
	 */
	const char* name;
} ZydisSymbol;

typedef struct _CpuInfo {
	std::vector<uint64_t> Address;
	std::vector<std::string> Opcodes;
	std::vector<std::string> Mnemonics;
	std::vector<std::string> Comments;
} CpuInfo, *PCpuInfo;

typedef struct _CpuCache {
	std::chrono::steady_clock::time_point LastUpdated;
	CpuInfo vCpuInfo;
} CpuCache, * PCpuCache;

struct RipRange final {
	//uint64_t CountOfSections = 0;
	//std::vector<size_t> vVirtualSizes;
	//std::vector<std::string> vNamesOfSections;
	std::pair<peparse::VA, peparse::VA> RipSectionRange;
	std::unique_ptr<Module> ModuleInfo;
};

class CPUParser_
{
private:
	CpuInfo vCpuInfo;
private:
	void GetThreadData(PSYSTEM_PROCESS_INFORMATION spi);
	void CountInstructions(std::pair<peparse::VA, peparse::VA> RipSectionRange);
	void DisassemblyGetter(std::pair<peparse::VA, peparse::VA> RipSectionRange);
	//void GetCpuData();

	//ZyanStatus ZydisFormatterPrintAddressAbsolute(const ZydisFormatter* formatter,
	//	ZydisFormatterBuffer* buffer, ZydisFormatterContext* context);

	ParsedPeRefDll OpenExecutable(std::string path) noexcept;
public:
	CONTEXT context;

	CpuCache Cache;
public:
	void GetCpuData();
	uint64_t FindAddrByRip();
	void ToDisassemble(uint64_t CountOfInstrsToDisasm, uint64_t RelativeRip);

	CPUParser_() {
		Cache.LastUpdated = std::chrono::steady_clock::now();
		GetCpuData();
		Cache.vCpuInfo = vCpuInfo;
		return;
	}

	void UpdateCPUCache(std::shared_ptr<std::atomic<bool>> Active) {
		Cache.LastUpdated = std::chrono::steady_clock::now();
		vCpuInfo.Address.clear();
		vCpuInfo.Opcodes.clear();
		vCpuInfo.Comments.clear();
		vCpuInfo.Mnemonics.clear();
		GetCpuData();
		while (Active->load()) { continue; }
		Cache.vCpuInfo = vCpuInfo;
		return;
	}
};