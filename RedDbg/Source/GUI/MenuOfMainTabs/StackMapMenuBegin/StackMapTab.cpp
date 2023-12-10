#include "GUI/MenuOfMainTabs/StackMapMenuBegin/StackMapTab.hpp"

void StackMapTab_::StackMapWindowRender()
{
    static float StackMapVerticalSplitterRelPosX = 0.2f;
    static int StartPosX = 0;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        StackMapVerticalSplitterRelPosX,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::StackMapTabWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.StackMapVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        StackMapVerticalSplitterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.AddressesWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::StackMapAddressesWindowCode,
        *this,
        true);

    
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.ReferencesToAddressesWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::StackMapReferencesToAddressesWindowCode,
        *this,
        true);
        
}

void StackMapTab_::StackMapAddressTableRender()
{
    if (ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.AddressesTableName.data(), Dimensionses.CountOfStackMapAddrsTableColumns, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.AddrsTable.StackMapTableAddressesColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.StackMapTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin(0, 17);//TODO: Data transfer
        while (clipper.Step())
        {
            for (int Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfStackMapAddrsTableColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfStackMapAddrsTableColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);
                    ImGui::Selectable("534453", false);
                    ImGui::PopStyleColor(2);

                    if (ImGui::IsItemHovered())
                    {
                        if (ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
                        {
                            //Open = true;
                        }
                    }
                }
            }
        }
        ImGui::EndTable();
    }
}

void StackMapTab_::StackMapReferncesTableRender()
{
    if (ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.RefsToAddrsTableName.data(), Dimensionses.CountOfStackMapRefsToAddrsTableColumns, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.RefsToAddrsTable.StackMapTableIdColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.RefsToAddrsTable.StackMapTableOpcodeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.RefsToAddrsTable.StackMapTableMnemonicColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.RefsToAddrsTable.StackMapTableActionColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.RefsToAddrsTable.StackMapTableCommentColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.StackMapTab.StackMapTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();


        ImGuiListClipper clipper;
        clipper.Begin(0, 17);//TODO: Data transfer
        while (clipper.Step())
        {
            for (int Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfStackMapAddrsTableColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfStackMapAddrsTableColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);
                    ImGui::Selectable("23423423423423423423", false);
                    ImGui::PopStyleColor(2);
                }
            }
        }
        ImGui::EndTable();
    }
}