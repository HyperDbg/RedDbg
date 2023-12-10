#include "GUI/MenuOfMainTabs/CPUMenuBegin/CPUTab.hpp"

void CPUTab_::DisassemblyTableRender()
{
    if (ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableName.data(), Dimensionses.CountOfDisasmColumns, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableJmpsMapColumnName.data(), DisassemblyColumnFlags);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableAddressColumnName.data(), DisassemblyColumnFlags);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableOpcodeColumnName.data(), DisassemblyColumnFlags);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableDisassemblyColumnName.data(), DisassemblyColumnFlags);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableCommentColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CPUTab.DisassemblyAndInfo.DisassemblyTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin(0, 17);//TODO: DataTransfer
        while (clipper.Step())
        {
            for (uint64_t Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfDisasmColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfDisasmColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);
                    //ImGui::Selectable("yuiyuiyuiyuiyuiyuiyu", false);// TODO: DataTransfer
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
