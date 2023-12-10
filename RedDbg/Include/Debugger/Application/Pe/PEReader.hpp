#pragma once
#include "PEInformation.hpp"
#include "GUI/UI/ImGui/imgui.h"

#include <vector>
#include <string>
#include <Windows.h>
#include <filesystem> 

enum ErrorPeParseCodes_
{
	AllOk,
	hPeFileContainsInvalidHandleValue,
	pImageDOSHeaderOfPeContainsNullptr,
	pImageNTHeaderOfPeContainsNullptr,
	pImageNTHeader64ContainsNullptr,
	pImageSectionHeaderContainsNullptr,
	pImageImportHeaderContainsNullptr,
	pImageImportDescriptorContainsNullptr,
};

class PeReader
{
private:
	const std::string ExitProcess = "ExitProcess";
	const std::string Kernel32Dll = "kernel32.dll";
private:
	HANDLE hFileContent;
	HANDLE hPeFileContent;
public:
	uint64_t ErrorCode = 0;
private:
	PIMAGE_DOS_HEADER pImageDOSHeaderOfPe;
	PIMAGE_NT_HEADERS pImageNTHeaderOfPe;
private:
	HANDLE inline GetFileContent();
	HANDLE inline GetContentOfDll(
		const std::wstring& DllName,
		uint8_t Method);
	PEInformation ParseImageOfDll64(
		const std::wstring& DllName,
		PEInformation& PEInformation);
	PEInformation SectionCopierOfDll64(
		PEInformation& PEInformation,
		std::vector<LPVOID>& BAs, 
		std::vector<size_t>& BAsSize);
	PEInformation ParseImage64(
		PEInformation& PEInformation);
	PEInformation ParseImageOfDll32(
		const std::wstring& DllName,
		PEInformation& PEInformation);
	PEInformation GetSectionsOfDll64(
		PEInformation& PEInformation);
	PEInformation GetSections64(
		PEInformation& PEInformation);
	PEInformation GetExportSection64(
		PEInformation& PEInformation);
	PEInformation GetImports64(
		PEInformation& PEInformation,
		uint64_t dRawOffset);
	PEInformation GetExports64(
		PEInformation& PEInformation,
		uint64_t dRawOffset);
	PEInformation GetImportsOfDll64(
		PEInformation& PEInformation,
		uint64_t dRawOffset,
		const std::wstring& DllName);
public:
	float CalcEntropy(char* ptr, int start, int end);
	PEInformation Dll(
		const std::wstring DllName,
		PEInformation& PEInformation);
	PEInformation Pe(
		PEInformation& PEInformation);
	std::filesystem::path PathToCurrentDebugging;
	bool Start;
};