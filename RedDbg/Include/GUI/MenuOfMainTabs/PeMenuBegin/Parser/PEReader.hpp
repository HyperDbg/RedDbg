#pragma once
#define _PEPARSE_WINDOWS_CONFLICTS 1
#include "pe-parse/parse.h"
#include "GUI/MenuDispatcherTabs/GlobalEnums.hpp"

#include <Windows.h>
#include <filesystem>

using ParsedPeRef = peparse::parsed_pe*;

typedef struct _Image
{
	PIMAGE_SECTION_HEADER pImageImportHeader;
	PIMAGE_IMPORT_DESCRIPTOR pImageImportDescriptor = nullptr;

	uint64_t dRawOffset = 0;
} Image, *PImage;

class DllsApi_
{
public:
	std::string Name;
	DWORD dwRvaToName;
	DWORD dwFirstThink;
	DWORD dwOriginalFirstThink;
	DWORD dwTimeDataStamp;
	DWORD dwForwaderChain;

	DllsApi_(std::string LibName, DWORD RvaToName, DWORD FirstThink, DWORD OriginalFirstThink, DWORD TimeDataStamp, DWORD ForwaderChain)
		: Name(LibName), dwRvaToName(RvaToName), dwFirstThink(FirstThink), dwOriginalFirstThink(OriginalFirstThink), dwTimeDataStamp(TimeDataStamp), dwForwaderChain(ForwaderChain) { }
};

class DllsFuncs_
{
public:
	std::string Name;
	FARPROC ptr;
	ULONGLONG qwOrdinal;

	DllsFuncs_(std::string FuncName, FARPROC Ptr, ULONGLONG Ordinal)
		: Name(FuncName), ptr(Ptr), qwOrdinal(Ordinal) { }
};

typedef struct _PeReader
{
	ParsedPeRef Pe = nullptr;
	STARTUPINFOW StartupInfo = { sizeof(StartupInfo) };
	PROCESS_INFORMATION ProcessInfo;
	std::vector<DllsApi_*> Dlls;
	std::vector<std::vector<DllsFuncs_*>*> Functions;
} PeReader, *PPeReader;

double CalcEntropy(uint8_t* ptr, int start, int end);

class PeParser
{
private:
	ParsedPeRef OpenExecutable(const std::string& path) noexcept;
	void ParseX32(ParsedPeRef Pe, const std::filesystem::path& PathToTarget, PPeReader Reader);
	void ParseX64(ParsedPeRef Pe, const std::filesystem::path& PathToTarget, PPeReader Reader);

	void GetImageImportHeader64(ParsedPeRef Pe, PImage ImageDirectory);
	void GetImageImportHeader32(ParsedPeRef Pe, PImage ImageDirectory);

	void InitAndGetImports64(ParsedPeRef Pe, PImage ImageDirectory, const std::filesystem::path& PathToTarget, PPeReader Reader);
	void InitAndGetImports32(ParsedPeRef Pe, PImage ImageDirectory, const std::filesystem::path& PathToTarget, PPeReader Reader);

	HANDLE LoadDll(const std::string& DllName, const std::filesystem::path& PathToTarget);
public:
	const std::shared_ptr<PeReader> Open(const std::filesystem::path& PathToTarget, bool Start);
	//float CalcEntropy(char* ptr, int start, int end);
	void PeIsNull(ParsedPeRef Pe);
};