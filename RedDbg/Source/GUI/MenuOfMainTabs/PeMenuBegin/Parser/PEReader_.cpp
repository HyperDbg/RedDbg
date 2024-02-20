#include "GUI/MenuOfMainTabs/PeMenuBegin/Parser/PEReader.hpp"

namespace MenuBarGlobalVars
{
    namespace EventTabGlobalVars
    {
        extern std::vector<bool> EventTabCheckbox;
    }
}

HANDLE PeParser::LoadDll(const std::string& DllName, const std::filesystem::path& PathToTarget)
{
    HMODULE hFile = LoadLibraryA(DllName.c_str());
    if (hFile == 0)
    {
        //std::wstring DllPath = PathToCurrentDebugging.parent_path().wstring() + PathToCurrentDebugging.root_directory().wstring() + DllName;
        hFile = LoadLibraryA((PathToTarget.parent_path().string() + PathToTarget.root_directory().string() + DllName).c_str());
        if (hFile == 0)
        {
            puts("[-] An error occured when trying to open the PE file !");
            return INVALID_HANDLE_VALUE;
        }
    }
    return hFile;
}

void PeParser::PeIsNull(ParsedPeRef Pe)
{
    if (!Pe) {
        //std::cout << "Failed to open the executable\n\n";

        //std::cout << "Error: " << peparse::GetPEErr() << " ("
        //    << peparse::GetPEErrString() << ")\n";

        //std::cout << "Location: " << peparse::GetPEErrLoc() << "\n";
        return;
    }
}

ParsedPeRef PeParser::OpenExecutable(const std::string& path) noexcept
{
    ParsedPeRef obj = peparse::ParsePEFromFile(path.data(), true);
    if (!obj) { peparse::DestructParsedPE(obj); return nullptr; }

    return obj;
}

void PeParser::GetImageImportHeader64(ParsedPeRef Pe, PImage ImageDirectory)
{
    PIMAGE_NT_HEADERS32 pImageNTHeader = (PIMAGE_NT_HEADERS32)((uint64_t)Pe->fileBuffer->buf + (uint64_t)Pe->peHeader.dos.e_lfanew);
    PIMAGE_SECTION_HEADER pImageSectionHeader = (PIMAGE_SECTION_HEADER)((uint64_t)pImageNTHeader + 4 + sizeof(IMAGE_FILE_HEADER) + Pe->peHeader.nt.FileHeader.SizeOfOptionalHeader);

    for (unsigned char Section = 0; Section < Pe->peHeader.nt.FileHeader.NumberOfSections; ++Section)
    {
        const PIMAGE_SECTION_HEADER pCurrentSectionHeader = (PIMAGE_SECTION_HEADER)((uint64_t)pImageSectionHeader + Section * sizeof(IMAGE_SECTION_HEADER));

        if (Pe->peHeader.nt.OptionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress >= pCurrentSectionHeader->VirtualAddress &&
            Pe->peHeader.nt.OptionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress < pCurrentSectionHeader->VirtualAddress + pCurrentSectionHeader->Misc.VirtualSize)
        {
            ImageDirectory->pImageImportHeader = pCurrentSectionHeader;
        }
    }

    uint64_t dRawOffset = (uint64_t)Pe->fileBuffer->buf + ImageDirectory->pImageImportHeader->PointerToRawData;
    ImageDirectory->dRawOffset = dRawOffset;

    ImageDirectory->pImageImportDescriptor = PIMAGE_IMPORT_DESCRIPTOR(uint64_t(dRawOffset +
        Pe->peHeader.nt.OptionalHeader64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress - ImageDirectory->pImageImportHeader->VirtualAddress));
}

void PeParser::GetImageImportHeader32(ParsedPeRef Pe, PImage ImageDirectory)
{
    PIMAGE_NT_HEADERS32 pImageNTHeader = (PIMAGE_NT_HEADERS32)((uint64_t)Pe->fileBuffer->buf + (uint64_t)Pe->peHeader.dos.e_lfanew);
    PIMAGE_SECTION_HEADER pImageSectionHeader = (PIMAGE_SECTION_HEADER)((uint64_t)pImageNTHeader + 4 + sizeof(IMAGE_FILE_HEADER) + Pe->peHeader.nt.FileHeader.SizeOfOptionalHeader);

    for (unsigned char Section = 0; Section < Pe->peHeader.nt.FileHeader.NumberOfSections; ++Section)
    {
        const PIMAGE_SECTION_HEADER pCurrentSectionHeader = (PIMAGE_SECTION_HEADER)((uint64_t)pImageSectionHeader + Section * sizeof(IMAGE_SECTION_HEADER));

        if (Pe->peHeader.nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress >= pCurrentSectionHeader->VirtualAddress &&
            Pe->peHeader.nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress < pCurrentSectionHeader->VirtualAddress + pCurrentSectionHeader->Misc.VirtualSize)
        {
            ImageDirectory->pImageImportHeader = pCurrentSectionHeader;
        }
    }

    uint64_t dRawOffset = (uint64_t)Pe->fileBuffer->buf + ImageDirectory->pImageImportHeader->PointerToRawData;
    ImageDirectory->dRawOffset = dRawOffset;

    ImageDirectory->pImageImportDescriptor = PIMAGE_IMPORT_DESCRIPTOR(uint64_t(dRawOffset +
        Pe->peHeader.nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress - ImageDirectory->pImageImportHeader->VirtualAddress));
}

void PeParser::InitAndGetImports64(ParsedPeRef Pe, PImage ImageDirectory, const std::filesystem::path& PathToTarget, PPeReader Reader)
{
    Reader->Functions.clear();
    Reader->Dlls.clear();
    while (ImageDirectory->pImageImportDescriptor->Name != 0)
    {
        std::string DllName = (char*)(ImageDirectory->dRawOffset + (ImageDirectory->pImageImportDescriptor->Name - ImageDirectory->pImageImportHeader->VirtualAddress));
        HANDLE hFileContent = LoadDll(DllName, PathToTarget);
        if (hFileContent == INVALID_HANDLE_VALUE) { return; }

        PIMAGE_THUNK_DATA64 pFirstThrunk = (PIMAGE_THUNK_DATA64)(ImageDirectory->dRawOffset + 
            (ImageDirectory->pImageImportDescriptor->FirstThunk - ImageDirectory->pImageImportHeader->VirtualAddress));

        Reader->Functions.push_back(new std::vector<DllsFuncs_*>);
        Reader->Dlls.push_back(new DllsApi_
            { DllName,
            ImageDirectory->pImageImportDescriptor->Name,
            ImageDirectory->pImageImportDescriptor->FirstThunk,
            ImageDirectory->pImageImportDescriptor->OriginalFirstThunk,
            ImageDirectory->pImageImportDescriptor->TimeDateStamp,
            ImageDirectory->pImageImportDescriptor->ForwarderChain });

        while (pFirstThrunk->u1.AddressOfData != 0)
        {
            if (pFirstThrunk->u1.AddressOfData >= IMAGE_ORDINAL_FLAG64) { ++pFirstThrunk; continue; }

            const PIMAGE_IMPORT_BY_NAME pImageImportByName = (PIMAGE_IMPORT_BY_NAME)pFirstThrunk->u1.AddressOfData;
            if (pImageImportByName == nullptr) { continue; }

            if (IMAGE_SNAP_BY_ORDINAL(pFirstThrunk->u1.Ordinal))
            {
                FARPROC pfnProc = GetProcAddress((HMODULE)hFileContent, (LPCSTR)pFirstThrunk->u1.Ordinal);
                if (pfnProc == NULL) { puts("\t\tpfnProc is null\n"); return; }

                pFirstThrunk->u1.Function = (ULONGLONG)pfnProc;
            }
            else
            {
                const std::string FunctionName = ImageDirectory->dRawOffset + (pImageImportByName->Name - ImageDirectory->pImageImportHeader->VirtualAddress);
                FARPROC pfnProc = GetProcAddress((HMODULE)hFileContent, FunctionName.c_str());
                if (pfnProc == NULL) { puts("\t\tpfnProc is null\n"); return; }
                pFirstThrunk->u1.Function = (ULONG_PTR)pfnProc;
            }

            ++pFirstThrunk;
        }

        FreeLibrary((HMODULE)hFileContent);
        ++ImageDirectory->pImageImportDescriptor;
    }
}

void PeParser::InitAndGetImports32(ParsedPeRef Pe, PImage ImageDirectory, const std::filesystem::path& PathToTarget, PPeReader Reader)
{
    Reader->Functions.clear();
    Reader->Dlls.clear();
    while (ImageDirectory->pImageImportDescriptor->Name != 0)
    {
        std::string DllName = (char*)(ImageDirectory->dRawOffset + (ImageDirectory->pImageImportDescriptor->Name - ImageDirectory->pImageImportHeader->VirtualAddress));
        HANDLE hFileContent = LoadDll(DllName, PathToTarget);
        if (hFileContent == INVALID_HANDLE_VALUE) { return; }

        PIMAGE_THUNK_DATA32 pFirstThrunk = (PIMAGE_THUNK_DATA32)(ImageDirectory->dRawOffset +
            (ImageDirectory->pImageImportDescriptor->FirstThunk - ImageDirectory->pImageImportHeader->VirtualAddress));

        Reader->Functions.push_back(new std::vector<DllsFuncs_*>);
        Reader->Dlls.push_back(new DllsApi_
            { DllName,
            ImageDirectory->pImageImportDescriptor->Name,
            ImageDirectory->pImageImportDescriptor->FirstThunk,
            ImageDirectory->pImageImportDescriptor->OriginalFirstThunk,
            ImageDirectory->pImageImportDescriptor->TimeDateStamp,
            ImageDirectory->pImageImportDescriptor->ForwarderChain });

        while (pFirstThrunk->u1.AddressOfData != 0)
        {
            if (pFirstThrunk->u1.AddressOfData >= IMAGE_ORDINAL_FLAG32) { ++pFirstThrunk; continue; }

            const PIMAGE_IMPORT_BY_NAME pImageImportByName = (PIMAGE_IMPORT_BY_NAME)pFirstThrunk->u1.AddressOfData;
            if (pImageImportByName == nullptr) { continue; }

            if (IMAGE_SNAP_BY_ORDINAL(pFirstThrunk->u1.Ordinal))
            {
                FARPROC pfnProc = GetProcAddress((HMODULE)hFileContent, (LPCSTR)pFirstThrunk->u1.Ordinal);
                if (pfnProc == NULL) { puts("\t\tpfnProc is null\n"); return; }

                pFirstThrunk->u1.Function = (ULONGLONG)pfnProc;
            }
            else
            {
                const std::string FunctionName = ImageDirectory->dRawOffset + (pImageImportByName->Name - ImageDirectory->pImageImportHeader->VirtualAddress);
                FARPROC pfnProc = GetProcAddress((HMODULE)hFileContent, FunctionName.c_str());
                if (pfnProc == NULL) { puts("\t\tpfnProc is null\n"); return; }
                pFirstThrunk->u1.Function = (ULONG_PTR)pfnProc;
            }

            ++pFirstThrunk;
        }

        FreeLibrary((HMODULE)hFileContent);
        ++ImageDirectory->pImageImportDescriptor;
    }
}

void PeParser::ParseX64(ParsedPeRef Pe, const std::filesystem::path& PathToTarget, PPeReader Reader)
{
    Image ImageDirectory;
    GetImageImportHeader64(Pe, &ImageDirectory);
    InitAndGetImports64(Pe, &ImageDirectory, PathToTarget, Reader);
}

void PeParser::ParseX32(ParsedPeRef Pe, const std::filesystem::path& PathToTarget, PPeReader Reader)
{
    Image ImageDirectory;
    GetImageImportHeader32(Pe, &ImageDirectory);
    InitAndGetImports32(Pe, &ImageDirectory, PathToTarget, Reader);
}

const std::shared_ptr<PeReader> PeParser::Open(const std::filesystem::path& PathToTarget, bool Start)
{
    auto Pe = OpenExecutable(PathToTarget.string());
    PeIsNull(Pe);

    auto Reader = std::make_shared<PeReader>();

    if (Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) { ParseX64(Pe, PathToTarget, Reader.get()); }
    else { ParseX32(Pe, PathToTarget, Reader.get()); }

    Reader->Pe = Pe;

    if (MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox_::SystemBreakpoint])
    {
        CreateProcessW(PathToTarget.wstring().c_str(),
            NULL, NULL, NULL, FALSE, CREATE_SUSPENDED,
            NULL, NULL, &Reader->StartupInfo, &Reader->ProcessInfo);
    }
    else
    {
        CreateProcessW(PathToTarget.wstring().c_str(),
            NULL, NULL, NULL, FALSE, NULL,
            NULL, NULL, &Reader->StartupInfo, &Reader->ProcessInfo);
    }



    return Reader;
}