#include "GUI/MenuOfMainTabs/MemoryMapMenuBegin/MemoryMapTab.hpp"

#include <vector>

void MemoryMapTab_::MemoryMapWindowRender()
{
    static float MemoryMapHorizontalSplitterRelPosY = 0.95f;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        MemoryMapHorizontalSplitterRelPosY,
        WindowCodes::MemoryMapTabWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.RightPos.x + WindowGeneralSizes.RightSize.x;
    posMax.y += WindowGeneralSizes.RightPos.y + WindowGeneralSizes.RightSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos,
        posMax,
        &StartPosY))
    {
        MemoryMapHorizontalSplitterRelPosY =
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::MemoryMapWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMapWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::FindInMemoryMapWindowCode,
        *this,
        false);
}

void MemoryMapTab_::MemoryMapTableRender()
{
    if (ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableName.data(), Dimensionses.CountOfMemoryMapColumns, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableAddressColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableSizeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTablePartyColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableInfoColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableContentColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableTypeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableProtectionColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableInitialColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin(0, 17);//TODO: Data transfer
        while (clipper.Step())
        {
            for (int Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfMemoryMapColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfMemoryMapColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);
                    //ImGui::Selectable("23423423423423423423", false);
                    ImGui::PopStyleColor(2);
                }
            }
        }
        ImGui::EndTable();
    }
}

void MemoryMapTab_::FindInMemoryMapWindowRender()
{
    static float FindInMemoryMapVerticalSplitterRelPosX = 0.15f;
    static int StartPosX = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        FindInMemoryMapVerticalSplitterRelPosX,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::FindInMemoryMapWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.FindInMemoryMapVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        FindInMemoryMapVerticalSplitterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.InputFieldInFindMemoryMapWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::InputFieldInFindMemoryMapWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.ComboBoxOfTypesWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::ComboBoxOfTypesInFindMemoryMapWindowCode,
        *this,
        true);
}

void MemoryMapTab_::InputFieldInFindInMemoryMapWindowRender()
{
    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::InputFieldInFindMemoryMapWindowCode);

    ImVec2 TextSize = ImGui::CalcTextSize(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.InputFieldInFindMemoryMapWindowName.data());

    static std::vector<char> InputFindBuffer(256);

    ImGui::PushItemWidth(WindowGeneralSizes.LeftSize.x);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
    ImGui::InputText(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.InputFieldInFindMemoryMapWindowName.data(),
        InputFindBuffer.data(),
        InputFindBuffer.size());
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();
}

void MemoryMapTab_::ComboBoxOfTypesInFindInMemoryMapWindowRender()
{
    const std::vector<std::string_view> Items
    {
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableAddressColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableSizeColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTablePartyColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableInfoColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableContentColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableTypeColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableProtectionColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableInitialColumnName
    };

    static int CurrentTypeId = 4;

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });

    if (ImGui::BeginCombo("Type", Items[CurrentTypeId].data(), ImGuiComboFlags_NoArrowButton))
    {
        for (int n = 0; n < Items.size(); n++)
        {
            const bool is_selected = (CurrentTypeId == n);
            if (ImGui::Selectable(Items[n].data(), is_selected))
                CurrentTypeId = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::PopStyleColor();
}