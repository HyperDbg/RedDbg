#include "GUI/MenuOfMainTabs/CPUMenuBegin/CPU.hpp"

namespace MenuBarGlobalVars
{
    namespace EventTabGlobalVars
    {
        extern std::vector<bool> EventTabCheckbox;
    }
}

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

namespace CPUGlobalVars
{
    uint64_t SelectedThread = 0;
    //int64_t SelectedMemorySection = -1;
}

static const ZydisSymbol SYMBOL_TABLE[3] =
{
    { 0x007FFFFFFF401000, "SomeModule.EntryPoint"   },
    { 0x007FFFFFFF530040, "SomeModule.SomeData"     },
    { 0x007FFFFFFF401100, "SomeModule.SomeFunction" }
};

ZydisFormatterFunc DefaultPrintAddressAbsolute;

ZyanStatus ZydisFormatterPrintAddressAbsolute(const ZydisFormatter* formatter,
    ZydisFormatterBuffer* buffer, ZydisFormatterContext* context)
{
    ZyanU64 address;
    ZYAN_CHECK(ZydisCalcAbsoluteAddress(context->instruction, context->operand,
        context->runtime_address, &address));

    for (ZyanUSize i = 0; i < ZYAN_ARRAY_LENGTH(SYMBOL_TABLE); ++i)
    {
        if (SYMBOL_TABLE[i].address == address)
        {
            ZYAN_CHECK(ZydisFormatterBufferAppend(buffer, ZYDIS_TOKEN_SYMBOL));
            ZyanString* string;
            ZYAN_CHECK(ZydisFormatterBufferGetString(buffer, &string));
            return ZyanStringAppendFormat(string, "<%s>", SYMBOL_TABLE[i].name);
        }
    }

    return DefaultPrintAddressAbsolute(formatter, buffer, context);
}

void CPUParser_::DisassemblyGetter(std::pair<peparse::VA, peparse::VA> RipSectionRange)
{
    //if (MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox_::SystemBreakpoint])
    //{
    //
    //}

    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

    ZydisFormatter formatter;
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
    ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_SEGMENT, ZYAN_TRUE);
    ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_SIZE, ZYAN_TRUE);

    DefaultPrintAddressAbsolute = (ZydisFormatterFunc)&ZydisFormatterPrintAddressAbsolute;
    ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_FUNC_PRINT_ADDRESS_ABS,
        (const void**)&DefaultPrintAddressAbsolute);

    ZydisDecodedInstruction instruction;
    std::vector<ZydisDecodedOperand> operands(ZYDIS_MAX_OPERAND_COUNT);
    std::string buffer; buffer.resize(260);

    ZyanU8* BaseSec = (ZyanU8*)RipSectionRange.first;
    ZyanU64 RuntimeAddr = RipSectionRange.first;
    ZyanUSize length = RipSectionRange.second;
    while (length)
    {
        ZydisDecoderDecodeFull(&decoder, BaseSec, length, &instruction, operands.data());

        //vCpuInfo.Address.push_back(RuntimeAddr);

        ZydisFormatterFormatInstruction(&formatter, &instruction, operands.data(),
            instruction.operand_count_visible, buffer.data(), buffer.size(), RuntimeAddr,
            ZYAN_NULL);
        vCpuInfo.Mnemonics.push_back(buffer);

        std::ostringstream opcodeStream;
        for (ZyanU64 i = 0; i < instruction.length; ++i) {
            opcodeStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(BaseSec[i]) << " ";
        }

        vCpuInfo.Opcodes.push_back(opcodeStream.str());

        BaseSec += instruction.length;
        length -= instruction.length;
        RuntimeAddr += instruction.length;
    }
}

void CPUParser_::CountInstructions(std::pair<peparse::VA, peparse::VA> RipSectionRange)
{
    ZydisDecoder decoder;
    //ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);
    decoder.decoder_mode = ZYDIS_DECODER_MODE_MINIMAL;
    decoder.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
    decoder.stack_width = ZYDIS_STACK_WIDTH_64;

    ZydisDecodedInstruction instruction;
    std::vector<ZydisDecodedOperand> operands(ZYDIS_MAX_OPERAND_COUNT);

    ZyanU8* BaseSec = (ZyanU8*)RipSectionRange.first;
    ZyanUSize length = RipSectionRange.second;

    while (length)
    {
        ZydisDecoderDecodeFull(&decoder, BaseSec, length, &instruction, operands.data());

        vCpuInfo.Address.push_back((uint64_t)BaseSec);

        BaseSec += instruction.length;
        length -= instruction.length;
    }

    printf("%llxn\n", vCpuInfo.Address[vCpuInfo.Address.size() - 1]);

    return;
}

void CPUParser_::GetThreadData(PSYSTEM_PROCESS_INFORMATION spi)
{
    if ((DWORD)spi->UniqueProcessId == GlobalVarsOfPeTab::objPEInformation->ProcessInfo.dwProcessId)
    {
        HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, (DWORD)spi->Threads[CPUGlobalVars::SelectedThread].ClientId.UniqueThread);
        if (hThread != NULL)
        {
            context.ContextFlags = CONTEXT_FULL;
            GetThreadContext(hThread, &context);
            CloseHandle(hThread);
        }
    }
}

ParsedPeRefDll CPUParser_::OpenExecutable(std::string path) noexcept {
    ParsedPeRefDll obj(peparse::ParsePEFromFile(path.data(), false),
        peparse::DestructParsedPE);
    if (!obj) {
        return ParsedPeRefDll(nullptr, peparse::DestructParsedPE);
    }
    return obj;
}

void CPUParser_::ToDisassemble(uint64_t CountOfInstrsToDisasm, uint64_t RelativeRip)
{
    vCpuInfo.Mnemonics.clear(); vCpuInfo.Opcodes.clear();
    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64);

    ZydisFormatter formatter;
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);
    ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_SEGMENT, ZYAN_TRUE);
    ZydisFormatterSetProperty(&formatter, ZYDIS_FORMATTER_PROP_FORCE_SIZE, ZYAN_TRUE);

    DefaultPrintAddressAbsolute = (ZydisFormatterFunc)&ZydisFormatterPrintAddressAbsolute;
    ZydisFormatterSetHook(&formatter, ZYDIS_FORMATTER_FUNC_PRINT_ADDRESS_ABS,
        (const void**)&DefaultPrintAddressAbsolute);

    ZydisDecodedInstruction instruction;
    std::vector<ZydisDecodedOperand> operands(ZYDIS_MAX_OPERAND_COUNT);
    std::string buffer; buffer.resize(260);

    uint64_t Rip = vCpuInfo.Address[RelativeRip];

    ZyanU8* BaseSec = (ZyanU8*)Rip;
    ZyanUSize length = ZYDIS_MAX_INSTRUCTION_LENGTH;

    while (CountOfInstrsToDisasm)
    {
        ZydisDecoderDecodeFull(&decoder, BaseSec, length, &instruction, operands.data());

        ZydisFormatterFormatInstruction(&formatter, &instruction, operands.data(),
            instruction.operand_count_visible, buffer.data(), buffer.size(), (ZyanU64)BaseSec,
            ZYAN_NULL);
        vCpuInfo.Mnemonics.push_back(buffer);

        std::ostringstream opcodeStream;
        for (ZyanU64 i = 0; i < instruction.length; ++i) {
            opcodeStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(BaseSec[i]) << " ";
        }

        vCpuInfo.Opcodes.push_back(opcodeStream.str());

        BaseSec += instruction.length;
        --CountOfInstrsToDisasm;
    }

    Cache.vCpuInfo = vCpuInfo;
}

uint64_t CPUParser_::FindAddrByRip()
{
    auto lRip = context.Rip;
    auto it = std::find_if(vCpuInfo.Address.begin(), vCpuInfo.Address.end(),
        [lRip](const uint64_t Addr) {
            return Addr == lRip;
        });

    if (it != vCpuInfo.Address.end() /*&& (it->second - BackSize) >= 0*/)
    { 
        auto IndexFind = std::distance(vCpuInfo.Address.begin(), it);
        return IndexFind;
    }
    return 0;
}

void CPUParser_::GetCpuData()
{
    Thread::GetThreadsOfUserProcess(std::bind(&CPUParser_::GetThreadData, this, std::placeholders::_1));
    RipRange DataFromSections;
    DataFromSections.ModuleInfo = Memory::GetFullModulePath(GlobalVarsOfPeTab::objPEInformation->ProcessInfo.hProcess, (LPVOID)context.Rip);

    auto AllocationBase = DataFromSections.ModuleInfo->mbi.AllocationBase;
    auto L_inspectSection = [](void* N,
        const peparse::VA& secBase,
        const std::string& secName,
        const peparse::image_section_header& SectionHeader,
        const peparse::bounded_buffer* data) -> int {
            static_cast<void>(secBase);
            static_cast<void>(secName);
            static_cast<void>(data);
            auto DataFromSections = static_cast<RipRange*>(N);

            if (DataFromSections->ModuleInfo->mbi.BaseAddress >= DataFromSections->ModuleInfo->mbi.AllocationBase
                && (uint64_t)DataFromSections->ModuleInfo->mbi.BaseAddress <
                (uint64_t)DataFromSections->ModuleInfo->mbi.AllocationBase + ALIGN_TO_4KB(SectionHeader.Misc.VirtualSize)) {
                DataFromSections->RipSectionRange = std::make_pair((uint64_t)DataFromSections->ModuleInfo->mbi.AllocationBase, ALIGN_TO_4KB(SectionHeader.Misc.VirtualSize));
            }

            DataFromSections->ModuleInfo->mbi.AllocationBase =
                PVOID((uint64_t)DataFromSections->ModuleInfo->mbi.AllocationBase + ALIGN_TO_4KB(SectionHeader.Misc.VirtualSize));

            return 0;
        };

    std::size_t found = DataFromSections.ModuleInfo->FullpathToModule.string().find(".exe");
    if (found != std::string::npos)
    {
        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        { 
            DataFromSections.ModuleInfo->mbi.AllocationBase =
                PVOID((uint64_t)DataFromSections.ModuleInfo->mbi.AllocationBase + GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.BaseOfCode);
        }
        else {
            DataFromSections.ModuleInfo->mbi.AllocationBase =
                PVOID((uint64_t)DataFromSections.ModuleInfo->mbi.AllocationBase + GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.BaseOfCode);
        }
    
        peparse::IterSec(GlobalVarsOfPeTab::objPEInformation->Pe, L_inspectSection, &DataFromSections);
        DataFromSections.ModuleInfo->mbi.AllocationBase = AllocationBase;
    }
    else
    {
        auto Pe = OpenExecutable(DataFromSections.ModuleInfo->FullpathToModule.string());
    
        if (Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        { 
            DataFromSections.ModuleInfo->mbi.AllocationBase =
                PVOID((uint64_t)DataFromSections.ModuleInfo->mbi.AllocationBase + Pe->peHeader.nt.OptionalHeader64.BaseOfCode);
        }
        else { 
            DataFromSections.ModuleInfo->mbi.AllocationBase =
                PVOID((uint64_t)DataFromSections.ModuleInfo->mbi.AllocationBase + Pe->peHeader.nt.OptionalHeader64.BaseOfCode);
        }
    
        peparse::IterSec(Pe.get(), L_inspectSection, &DataFromSections);
        DataFromSections.ModuleInfo->mbi.AllocationBase = AllocationBase;
    }

    return CountInstructions(DataFromSections.RipSectionRange);
	//DisassemblyGetter(DataFromSections.RipSectionRange);

}