#pragma once
#include <string>
#include <Windows.h>
#include <filesystem>

class Module
{
public:
	MEMORY_BASIC_INFORMATION mbi{};
	std::filesystem::path FullpathToModule;
};

namespace Memory
{
	std::filesystem::path GetModulePath(HMODULE hModule);
	std::unique_ptr<Module> GetFullModulePath(HANDLE hProcess, LPVOID pAddress);
}