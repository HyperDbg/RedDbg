#include "GUI/MenuOfMainTabs/CPUMenuBegin/CPUTab.hpp"
#include <imgui_internal.h>

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

//{ для реализации нужно писать полноценный дизасм, т.к.StepOver
//применяется для целой кучи инструкций, вот список :
//CALL, INT, LOOP, LOOPZ, LOOPNZ, REP, REPZ, REPNZ, CMPS, CMPSB, CMPSW,
//LODSB, LODSW, MOVS, MOVSB, MOVSW, SCAS, SCASB, SCASW, STOS, STOSB, STOSW
//}

void CPUTab_::GeneralRegister(const std::vector<std::string_view>& vGeneralRegs, std::string_view& Data)
{
    std::stringstream Ss;
    for (int RelativeIndex = 0, Index = 0; RelativeIndex < vGeneralRegs.size(); ++Index)
    {
        if (Index != Registers::RAX && Index != Registers::RFLAGS + 1)
        {
            Ss << vGeneralRegs[RelativeIndex] << ":";
            ImGui::Text(Ss.str().c_str());
            ImGui::SameLine();
            auto Size = ImGui::CalcTextSize(Data.data());
            if (ImGui::Selectable(Data.data(), false, 0, Size))
            {

            }

            ++RelativeIndex;
            Ss.str("");
        }
        else
        {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }
    }
}

void CPUTab_::FlagsRegister(const std::vector<std::string_view>& vBitsOfFlag)
{
    std::stringstream Ss;
    for (int Index = 0; Index < vBitsOfFlag.size(); ++Index)
    {
        if ((Index + 1) % 3)
        {
            Ss << vBitsOfFlag[Index] << ":";
            ImGui::Text(Ss.str().c_str());
            ImGui::SameLine();
            auto Size = ImGui::CalcTextSize("0");
            if (ImGui::Selectable("0", false, 0, Size))
            {

            }

            ImGui::SameLine();
        }
        else
        {
            Ss << vBitsOfFlag[Index] << ":";
            ImGui::Text(Ss.str().c_str());
            ImGui::SameLine();
            auto Size = ImGui::CalcTextSize("0");
            if (ImGui::Selectable("0", false, 0, Size))
            {

            }
        }
        Ss.str("");
    }
}

void CPUTab_::LastError_Status()
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    std::stringstream Ss;
    Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.LastError << ":";

    ImGui::Text(Ss.str().c_str());
    ImGui::SameLine();
    auto Size = ImGui::CalcTextSize("FFFFFFFF");
    if (ImGui::Selectable("FFFFFFFF", false, 0, Size))
    {

    }

    Ss.str("");

    Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.LastStatus << ":";
    ImGui::Text(Ss.str().c_str());
    ImGui::SameLine();
    if (ImGui::Selectable("FFFFFFFF", false, 0, Size))
    {

    }
}

void CPUTab_::DrRegisters()
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    std::stringstream Ss;
    ImVec2 DrSize = ImGui::CalcTextSize("0000000000000000");
    for (int Index = 0; Index < Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.Debug.vDr.size(); ++Index)
    {
        Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.Debug.vDr[Index] << ":";
        ImGui::Text(Ss.str().c_str());
        ImGui::SameLine();
        if (ImGui::Selectable("0000000000000000", false, 0, DrSize))
        {

        }

        Ss.str("");
    }
}

void CPUTab_::SegmentsRegisters()
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    std::stringstream Ss;
    ImVec2 SegSize = ImGui::CalcTextSize("FFFF");
    for (int Index = 0; Index < Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.Segments.vSegments.size(); ++Index)
    {
        Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.Segments.vSegments[Index] << ":";
        ImGui::Text(Ss.str().c_str());
        ImGui::SameLine();
        if (ImGui::Selectable("FFFF", false, 0, SegSize))
        {
        
        }
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

        if ((Index + 1) % 2)  {  ImGui::SameLine(); }
        Ss.str("");
    }
}

void CPUTab_::X87r_MmxRegisters(const TypeOfDisplay_ Type)
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    std::stringstream Ss;
    switch (Type)
    {
    case TypeOfDisplay_::ST_X87:
    {
        for (int Index = 0; Index < Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vST_X87.size(); ++Index)
        {
            Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vST_X87[Index].first << ":";
            ImGui::Text(Ss.str().c_str());
            ImGui::SameLine();
            auto Size = ImGui::CalcTextSize("00000000000000000000");
            if (ImGui::Selectable("00000000000000000000", false, 0, Size))
            {

            }
            Ss.str("");
            if (!(Index % 1)) { ImGui::SameLine(); }
            Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vST_X87[Index].second << ":";
            ImGui::Text(Ss.str().c_str());
            ImGui::SameLine();
            ImGui::Text("Empty");
            ImGui::SameLine();
            ImGui::Text("0.00000000000000000000");
            Ss.str("");
        }
        break;
    }
    case TypeOfDisplay_::Mmx:
    {
        for (int Index = 0; Index < Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.Mmx.vMmx.size(); ++Index)
        {
            Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.Mmx.vMmx[Index] << ":";
            ImGui::Text(Ss.str().c_str());
            ImGui::SameLine();
            auto Size = ImGui::CalcTextSize("0000000000000000");
            if (ImGui::Selectable("0000000000000000", false, 0, Size))
            {

            }
            Ss.str("");
        }
        break;
    }
    case TypeOfDisplay_::X87_ST:
    {
        for (int Index = 0; Index < Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vST_X87.size(); ++Index)
        {
            Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vST_X87[Index].second << ":";
            ImGui::Text(Ss.str().c_str());
            ImGui::SameLine();
            auto Size = ImGui::CalcTextSize("00000000000000000000");
            if (ImGui::Selectable("00000000000000000000", false, 0, Size))
            {

            }
            Ss.str("");
            if (!(Index % 1)) { ImGui::SameLine(); }
            Ss << Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vST_X87[Index].first << ":";
            ImGui::Text(Ss.str().c_str());
            ImGui::SameLine();
            ImGui::Text("Empty");
            ImGui::SameLine();
            ImGui::Text("0.00000000000000000000");
            Ss.str("");
        }
        break;
    }
    }
}

//TO DO: CallBacks
void CPUTab_::X87FlagAndSseFlags(
    const std::string_view& GeneralFpuFlagName,
    const std::vector<std::string_view> vGeneralFpuFlagBits, 
    TypeOfFpuBitFlags_ TypeOfFpuBitFlags)
{
    ImGui::Spacing();
    //if (TypeOfFpuBitFlags == TypeOfFpuBitFlags_::Sse) { ImGui::Separator(); }
    ImGui::Spacing();

    std::stringstream Ss;
    Ss << GeneralFpuFlagName << ":";
    ImGui::Text(Ss.str().c_str());
    ImVec2 TagWordSize;
    if (TypeOfFpuBitFlags != TypeOfFpuBitFlags_::Sse) { TagWordSize = ImGui::CalcTextSize("FFFF"); }
    else { TagWordSize = ImGui::CalcTextSize("FFFFFFFF"); }
    auto TWSize = ImGui::CalcTextSize("0");
    ImGui::SameLine();
    if (ImGui::Selectable("FFFF", false, 0, TagWordSize))
    {

    }
    Ss.str("");

    for (int Index = 0; Index < vGeneralFpuFlagBits.size(); ++Index)
    {
        Ss << vGeneralFpuFlagBits[Index] << ":";
        ImGui::Text(Ss.str().c_str());
        ImGui::SameLine();
        if (ImGui::Selectable("0", false, 0, TWSize))
        {

        }
        switch (TypeOfFpuBitFlags)
        {
        case TypeOfFpuBitFlags_::X87TW:
        {
            ImGui::SameLine();
            ImGui::Text("(Empty)");
            break;
        }
        case TypeOfFpuBitFlags_::X87SW:
        {
            break;
        }
        case TypeOfFpuBitFlags_::X87CW:
        {
            break;
        }
        case TypeOfFpuBitFlags_::Sse:
        {
            break;
        }
        }
        if (!((Index) % 2)) { ImGui::SameLine(); }
        Ss.str("");
    }
}

void CPUTab_::AvxRegisters(
    const std::vector<std::string_view> vAvxRegs)
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    std::stringstream Ss;
    ImVec2 Size = ImGui::CalcTextSize("0000000000000000000000000000000000000000000000000000000000000000");
    for (int Index = 0; Index < vAvxRegs.size(); ++Index)
    {
        Ss << vAvxRegs[Index] << ":";
        ImGui::Text(Ss.str().c_str());
        ImGui::SameLine();
        if (ImGui::Selectable("0000000000000000000000000000000000000000000000000000000000000000", false, 0, Size))
        {

        }
        Ss.str("");
    }
}

void CPUTab_::SseRegisters(
    const std::vector<std::string_view> vSseRegs)
{
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    std::stringstream Ss;
    ImVec2 Size = ImGui::CalcTextSize("00000000000000000000000000000000");
    for (int Index = 0; Index < vSseRegs.size(); ++Index)
    {
        Ss << vSseRegs[Index] << ":";
        ImGui::Text(Ss.str().c_str());
        ImGui::SameLine();
        if (ImGui::Selectable("00000000000000000000000000000000", false, 0, Size))
        {

        }
        Ss.str("");
    }
}

void CPUTab_::RegisterRender()
{
    if (CustomTitleBarGlobalVars::Pid != 0)
    {
        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
        {
            std::string_view GDataX64 = "FFFFFFFFFFFFFFFF";
            GeneralRegister(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x64.x64Regs, GDataX64);

            FlagsRegister(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.BitFlags.vBitFlags);

            DrRegisters();

            LastError_Status();

            SegmentsRegisters();

            AvxRegisters(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.AvxSse.x64YmmRegs);
            SseRegisters(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.AvxSse.x64XmmRegs);

            X87FlagAndSseFlags(
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.AvxSse.MxCsr,
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.AvxSse.vMxCsr,
                TypeOfFpuBitFlags_::Sse);

            X87r_MmxRegisters(TypeOfDisplay_::ST_X87);
            X87FlagAndSseFlags(
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.x87TagWord,
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vX87TW,
                TypeOfFpuBitFlags_::X87TW);
            X87FlagAndSseFlags(
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.x87StatusWord,
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vX87SW,
                TypeOfFpuBitFlags_::X87SW);
            X87FlagAndSseFlags(
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.x87ControlWord,
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vX87CW,
                TypeOfFpuBitFlags_::X87CW);
        }
        else
        {
            std::string_view GDataX32 = "FFFFFFFF";
            GeneralRegister(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x32.x32Regs, GDataX32);

            FlagsRegister(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.BitFlags.vBitFlags);

            DrRegisters();

            LastError_Status();

            SegmentsRegisters();

            AvxRegisters(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.AvxSse.x32YmmRegs);
            SseRegisters(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.AvxSse.x32XmmRegs);

            X87FlagAndSseFlags(
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.AvxSse.MxCsr,
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.AvxSse.vMxCsr,
                TypeOfFpuBitFlags_::Sse);

            X87r_MmxRegisters(TypeOfDisplay_::ST_X87);
            X87FlagAndSseFlags(
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.x87TagWord,
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vX87TW,
                TypeOfFpuBitFlags_::X87TW);
            X87FlagAndSseFlags(
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.x87StatusWord,
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vX87SW,
                TypeOfFpuBitFlags_::X87SW);
            X87FlagAndSseFlags(
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.x87ControlWord,
                Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegistersForWindow.x87.vX87CW,
                TypeOfFpuBitFlags_::X87CW);
        }
    }
}

void CPUTab_::CallConventionRender()
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
    ImGui::PushStyleColor(ImGuiCol_Button, 0);
    const char* items[] = { "thiscall##x64", "stdcall", "stdcall(EBP stack)", "thiscall##x32", "fastcall", "Borland fastcall"};
    static int item_current_idx = 0; // Here we store our selection data as an index.
    const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
    if (ImGui::BeginCombo("##combo 1", combo_preview_value, ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_WidthFitPreview))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            if (n == 0) { ImGui::SeparatorText("x64"); }

            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(items[n], is_selected)) { item_current_idx = n; }
            if (n == 0) { ImGui::SeparatorText("x32"); }
            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) { ImGui::SetItemDefaultFocus(); }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    static int CallStackSize = 0;
    ImGui::SetNextItemWidth(80);
    ImGui::InputInt("##input int", &CallStackSize);
    ImGui::PopStyleColor(2);
}

void CPUTab_::DisassemblyTableRender()
{
    if (CustomTitleBarGlobalVars::Pid != 0 &&
        ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableName.data(), Dimensionses.CountOfDisasmColumns, DefaultTableFlags))
    {
        ImGui::SetItemUsingMouseWheel();

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableJmpsMapColumnName.data(), DisassemblyColumnFlags);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableAddressColumnName.data(), DisassemblyColumnFlags);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableOpcodeColumnName.data(), DisassemblyColumnFlags);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableDisassemblyColumnName.data(), DisassemblyColumnFlags);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableCommentColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        static CPUParser_ Parse;
        static uint64_t lRip = 0;
        ImGuiListClipper clipper;
        clipper.Begin(Parse.Cache.vCpuInfo.Address.size(), 17);

        int index = Parse.FindAddrByRip() - 1;
        clipper.ForceDisplayRangeByIndices(index, index + 1);
        
        while (clipper.Step())
        {
            //{If mouse wheels up - disassembly back | If mouse wheels down - disassembly forward} - Rip Relative
            float CurrentScrollBar = ImGui::GetScrollY();
            uint64_t CurrentTableSize = clipper.DisplayEnd - clipper.DisplayStart;

            static uint64_t lCurrentTableSize = 0;
            static float lCurrentScrollBar = 0.0;
            if (CurrentScrollBar != lCurrentScrollBar || CurrentTableSize != lCurrentTableSize)
            {
                Parse.ToDisassemble(clipper.DisplayEnd - clipper.DisplayStart, clipper.DisplayStart);
                lCurrentScrollBar = CurrentScrollBar;
                lCurrentTableSize = CurrentTableSize;
            }
            else if (CurrentScrollBar == 0)
            {
                Parse.ToDisassemble(clipper.DisplayEnd - clipper.DisplayStart, Parse.FindAddrByRip());
                lCurrentScrollBar = CurrentScrollBar;
                lCurrentTableSize = CurrentTableSize;
            }
            for (uint64_t RelativeRow = 0, Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row, ++RelativeRow)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfDisasmColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfDisasmColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);

                    if (Parse.context.Rip != lRip && Row == index) { ImGui::SetScrollHereY(0.0f); lRip = Parse.context.Rip; }

                    switch (Column)
                    {
                    case 0:
                    {
                        break;
                    }
                    case 1:
                    {
                        std::stringstream Ss;
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint64_t) * 2) << std::hex << Parse.Cache.vCpuInfo.Address[Row];
                        }
                        else
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint32_t) * 2) << std::hex << Parse.Cache.vCpuInfo.Address[Row];
                        }
                        ImGui::Selectable(Ss.str().c_str(), false);
                        break;
                    }
                    case 2:
                    {
                        ImGui::Selectable(Parse.Cache.vCpuInfo.Opcodes[RelativeRow].data(), false);
                        break;
                    }
                    case 3:
                    {
                        ImGui::Selectable(Parse.Cache.vCpuInfo.Mnemonics[RelativeRow].data(), false);
                        break;
                    }
                    case 4:
                    {
                        break;
                    }
                    }
                    ImGui::PopStyleColor(2);
                }
            }
        }
        ImGui::EndTable();
    }
}

void CPUTab_::CPUDisasmAndInfoWindowRender()
{
    static float DisasmAndInfoVerticalSplitterRelPosX = 0.85f;
    static float DisasmAndInfoHorizontalSplitterRelPosY = 0.9f;
    static int StartPosX = 0;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        DisasmAndInfoVerticalSplitterRelPosX,
        DisasmAndInfoHorizontalSplitterRelPosY,
        WindowCodes::DisasmAndInfoWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.LeftPos.x + WindowGeneralSizes.LeftSize.x;
    posMax.y += WindowGeneralSizes.LeftPos.y + WindowGeneralSizes.LeftSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisasmAndInfoSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos,
        posMax,
        &StartPosY))
    {
        DisasmAndInfoHorizontalSplitterRelPosY =
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::DisassemblyWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.InfoWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::InfoWindowCode,
        *this,
        true);
}

void CPUTab_::CPURegisterAndCallConventionWindowRender()
{
    static float RegisterAndCallConventionVerticalSplitterRelPosX = 0.85f;
    static float RegisterAndCallConventionHorizontalSplitterRelPosY = 0.8f;
    static int StartPosX = 0;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        RegisterAndCallConventionVerticalSplitterRelPosX,
        RegisterAndCallConventionHorizontalSplitterRelPosY,
        WindowCodes::RegisterAndCallConventionWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.LeftPos.x + WindowGeneralSizes.LeftSize.x;
    posMax.y += WindowGeneralSizes.LeftPos.y + WindowGeneralSizes.LeftSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegisterAndCallConventionSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos,
        posMax,
        &StartPosY))
    {
        RegisterAndCallConventionHorizontalSplitterRelPosY = 
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.RegisterWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::RegisterWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConvention.CallConventionWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::CallConventionWindowCode,
        *this,
        true);
}

void CPUTab_::StackTableRender()
{
    if (ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.StackWindowTableName.data(), Dimensionses.CountOfStackColumns, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.StackTableAddressColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.StackTableDataColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.StackTableCommentColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.StackTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin(0, 17);//43534 is count. TODO: DataTransfer
        while (clipper.Step())
        {
            for (uint64_t Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfStackColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfStackColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);
                    //ImGui::Selectable("Addr", false); TODO: DataTransfer
                    ImGui::PopStyleColor(2);
                }
            }
        }

        ImGui::EndTable();
    }
}

void CPUTab_::TabDispatcherDump(DumpTab_ TabCode, bool* open)
{
    static HexEditor objHexEditor;
    switch (TabCode)
    {
    case Dump1:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.Dump.Dump1.data(), open))
        {
            objHexEditor.DrawEditor();
            ImGui::EndTabItem();
        }
        return;
    }
    case Dump2:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.Dump.Dump2.data(), open))
        {
            objHexEditor.DrawEditor();
            ImGui::EndTabItem();
        }
        return;
    }
    case Dump3:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.Dump.Dump3.data(), open))
        {
            objHexEditor.DrawEditor();
            ImGui::EndTabItem();
        }
        return;
    }
    case Dump4:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.Dump.Dump4.data(), open))
        {
            objHexEditor.DrawEditor();
            ImGui::EndTabItem();
        }
        return;
    }
    case Dump5:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.Dump.Dump5.data(), open))
        {
            objHexEditor.DrawEditor();
            ImGui::EndTabItem();
        }
        return;
    }
    case Watch1:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.Dump.Watch1.data(), open))
        {
            ImGui::EndTabItem();
        }
        return;
    }
    case Locals:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.Dump.Locals.data(), open))
        {
            ImGui::EndTabItem();
        }
        return;
    }
    case Struct:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.Dump.Struct.data(), open))
        {
            ImGui::EndTabItem();
        }
        return;
    }
    default:
    {
        if (ImGui::BeginTabItem((Names.Windowses.MainDebuggerInterface.MainTabs.MainNewTabName.data() + std::to_string((unsigned long long)TabCode)).data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    }
}

void CPUTab_::DumpRender()
{
    static std::vector<uint64_t> ActiveTabs;
    static uint64_t NextTabId = 0;
    if (NextTabId == 0)
    {
        for (int Tab = 0; Tab < 8; Tab++)
        {
            ActiveTabs.push_back(NextTabId++);
        }
    }

    if (ImGui::BeginTabBar(
        Names.Windowses.MainDebuggerInterface.MainTabBarName.data(),
        ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton))
    {
        ImGui::SetCursorPosX(0);

        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) { ActiveTabs.push_back(NextTabId++); }

        for (uint64_t Tab = 0; Tab < ActiveTabs.size();)
        {
            bool open = true;

            TabDispatcherDump((DumpTab_)ActiveTabs[Tab], &open);

            if (!open) { ActiveTabs.erase(ActiveTabs.begin() + Tab); }
            else { Tab++; }
        }

        ImGui::EndTabBar();
    }
}

void CPUTab_::StackAndDumpWindowRender()
{
    static float DumpAndStackVerticalSplitterRelPosX = 0.5f;
    static float DumpAndStackHorizontalSplitterRelPosY = 0.8f;
    static int StartPosX = 0;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        DumpAndStackVerticalSplitterRelPosX,
        DumpAndStackHorizontalSplitterRelPosY,
        WindowCodes::StackAndDumpWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.StackAndDumpVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        DumpAndStackVerticalSplitterRelPosX = 
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.StackWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::StackWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDump.DumpWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::DumpWindowCode,
        *this,
        true);
}

void CPUTab_::CPUWindowRender()
{
    static float CPUVerticalSplitterRelPosX = 0.85f;
    static float CPUHorizontalSplitterRelPosY = 0.8f;
    static int StartPosX = 0;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight, 
        Dimensionses.SplitterSize,
        CPUVerticalSplitterRelPosX,
        CPUHorizontalSplitterRelPosY, 
        WindowCodes::CPUTabWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.CPUVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX, 
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos, 
        posMax, 
        &StartPosX))
    {
        CPUVerticalSplitterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    ImVec2 posMax2{};
    posMax2.x += WindowGeneralSizes.RightPos.x + WindowGeneralSizes.RightSize.x;
    posMax2.y += WindowGeneralSizes.RightPos.y + WindowGeneralSizes.RightSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.CPUHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos, 
        posMax2, 
        &StartPosY))
    {
        CPUHorizontalSplitterRelPosY = 
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisasmAndInfoWindowName.data(), 
        WindowGeneralSizes.LeftTopPosition, 
        WindowGeneralSizes.LeftTopSize, 
        WindowCodes::DisasmAndInfoWindowCode,
        *this,
        false);
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.RegisterAndCallConventionWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::RegisterAndCallConventionWindowCode,
        *this,
        false);
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.StackAndDumpWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize, 
        WindowCodes::StackAndDumpWindowCode,
        *this,
        false);
}
