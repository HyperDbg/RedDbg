#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <filesystem>
#include <utility>
#include <TlHelp32.h>

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

class Apis_
{
public:
	std::vector<DllsApi_*> Dlls;
	std::vector<std::vector<DllsFuncs_*>*> Functions;
	//std::vector<std::pair<std::pair<const dwName, const dwOriginalFirstThink>, std::pair<const dwNameRva, const dwFirstThink>>> Dlls;
	//std::vector<std::pair<std::pair<const dwName, const dwOriginalFirstThink>, const dwFirstThink>> Functions;
};

class PEInformation
{
public:
	//std::vector<std::pair<ULONG_PTR, const std::string>> Funcs;
	Apis_ Apis;
	uint64_t FirstSize = 0, SizeOfAll = 0;
private:
	class x64
	{
	public:
		PIMAGE_NT_HEADERS64 pImageNTHeader64;
		IMAGE_OPTIONAL_HEADER64 ImageOptionalHeader64;
		PIMAGE_THUNK_DATA64 pOriginalFirstThrunk;
	};

	class x32
	{
	public:
		PIMAGE_NT_HEADERS32 pImageNTHeader32;
		IMAGE_OPTIONAL_HEADER32 ImageOptionalHeader32;
		PIMAGE_THUNK_DATA32 pOriginalFirstThrunk;
	};

	class Dllx64
	{
	public:
		PIMAGE_NT_HEADERS64 pImageNTHeader64;
		IMAGE_OPTIONAL_HEADER64 ImageOptionalHeader64;
		PIMAGE_THUNK_DATA64 pOriginalFirstThrunk;
	};

	class Dllx32
	{
	public:
		PIMAGE_NT_HEADERS32 pImageNTHeader32;
		IMAGE_OPTIONAL_HEADER32 ImageOptionalHeader32;
		PIMAGE_THUNK_DATA32 pOriginalFirstThrunk;
	};

	class Dll
	{
	public:
		Dllx64 x64;
		Dllx32 x32;

		PIMAGE_DOS_HEADER pImageDOSHeaderOfPe;
		PIMAGE_NT_HEADERS pImageNTHeaderOfPe;

		PIMAGE_SECTION_HEADER pImageExportHeader;
		PIMAGE_EXPORT_DIRECTORY pImageExportDirectory;

		PIMAGE_SECTION_HEADER pImageImportHeader;
		PIMAGE_IMPORT_DESCRIPTOR pImageImportDescriptor;

		IMAGE_FILE_HEADER ImageFileHeader;
		PIMAGE_SECTION_HEADER pImageSectionHeader;
	};
public:
	x64 x64;
	x32 x32;
	Dll Dll;
public:
	uint64_t SizeOfStack = 0x1000;
public:
	PIMAGE_DOS_HEADER pImageDOSHeaderOfPe;
	PIMAGE_NT_HEADERS pImageNTHeaderOfPe;

	PIMAGE_SECTION_HEADER pImageExportHeader;
	PIMAGE_EXPORT_DIRECTORY pImageExportDirectory;

	PIMAGE_SECTION_HEADER pImageImportHeader;
	PIMAGE_IMPORT_DESCRIPTOR pImageImportDescriptor;

	IMAGE_FILE_HEADER ImageFileHeader;
	PIMAGE_SECTION_HEADER pImageSectionHeader;
public:
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFOW StartupInfo = { sizeof(StartupInfo) };
	THREADENTRY32 TE32;
	PROCESSENTRY32W entry;
public:
	uint32_t ThreadMainGetter()
	{
		HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		TE32.dwSize = sizeof(THREADENTRY32);
		BOOL bSuccess = Thread32First(hThreadSnap, &TE32);
		uint32_t mainThreadId = 0;
		while (bSuccess)
		{
			if (TE32.th32OwnerProcessID == entry.th32ProcessID)
			{
				mainThreadId = TE32.th32ThreadID;
				break;
			}
			bSuccess = Thread32Next(hThreadSnap, &TE32);
		}
		CloseHandle(hThreadSnap);

		return mainThreadId;
	}
};