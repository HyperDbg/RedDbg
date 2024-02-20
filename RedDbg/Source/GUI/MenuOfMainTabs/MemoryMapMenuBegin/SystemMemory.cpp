#include "GUI/MenuOfMainTabs/MemoryMapMenuBegin/SystemMemory.hpp"


std::filesystem::path Memory::GetModulePath(HMODULE hModule) {
    char szPath[MAX_PATH] = { 0 };
    if (::GetModuleFileNameA(hModule, szPath, sizeof(szPath) / sizeof(wchar_t))) {
        return szPath;
    }
    return std::string();
}

std::unique_ptr<Module> Memory::GetFullModulePath(HANDLE hProcess, LPVOID pAddress) {
    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQueryEx(hProcess, pAddress, &mbi, sizeof(mbi))) {
        HMODULE hModule = (HMODULE)mbi.AllocationBase;
        std::unique_ptr<Module> structModule = std::make_unique<Module>();
        structModule->mbi = mbi;
        structModule->FullpathToModule = Memory::GetModulePath(hModule);
        return structModule;
    }
    return nullptr;
}