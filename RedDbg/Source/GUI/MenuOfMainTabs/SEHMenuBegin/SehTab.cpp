#include "GUI/MenuOfMainTabs/SEHMenuBegin/SehTab.hpp"

#include <vector>

void SEHTab_::SEHWindowRender()
{
    static float SEHHorizontalSplitterRelPosY = 0.95f;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        SEHHorizontalSplitterRelPosY,
        WindowCodes::SEHTabWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.RightPos.x + WindowGeneralSizes.RightSize.x;
    posMax.y += WindowGeneralSizes.RightPos.y + WindowGeneralSizes.RightSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos,
        posMax,
        &StartPosY))
    {
        SEHHorizontalSplitterRelPosY =
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::SEHWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.FindInSEHWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::FindInSEHWindowCode,
        *this,
        false);
}

void SEHTab_::SEHTableRender()
{
    if (ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableName.data(), Dimensionses.CountOfSEHColumns, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableAddressColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableHandlerColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableModuleLabelColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableCommentColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin(0, 17);//TODO: Data transfer
        while (clipper.Step())
        {
            for (int Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfSEHColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfSEHColumns, Dimensionses.U32GrayColor);

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

void SEHTab_::FindInSEHWindowRender()
{
    static float FindInSEHVerticalSplitterRelPosX = 0.15f;
    static int StartPosX = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        FindInSEHVerticalSplitterRelPosX,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::FindInSEHWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.FindInSEH.FindInSEHVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        FindInSEHVerticalSplitterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.FindInSEH.InputFieldInFindSEHWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::InputFieldInFindSEHWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.FindInSEH.ComboBoxOfTypesWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::ComboBoxOfTypesInFindSEHWindowCode,
        *this,
        true);
}

void SEHTab_::InputFieldInFindInSEHWindowRender()
{
    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::InputFieldInFindSEHWindowCode);

    ImVec2 TextSize = ImGui::CalcTextSize(Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.FindInSEH.InputFieldInFindSEHWindowName.data());

    //static std::vector<char> InputFindBuffer(256);
    static std::string InputFindBuffer = "";

    ImGui::PushItemWidth(WindowGeneralSizes.LeftSize.x);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
    ImGui::InputText(
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.FindInSEH.InputFieldInFindSEHWindowName.data(),
        InputFindBuffer.data(),
        InputFindBuffer.size(), 
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFindBuffer);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();
}

void SEHTab_::ComboBoxOfTypesInFindInSEHWindowRender()
{
    const std::vector<std::string_view> Items
    {
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableAddressColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableHandlerColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableModuleLabelColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.SEHTab.SEHTableCommentColumnName
    };

    static int CurrentTypeId = 2;

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