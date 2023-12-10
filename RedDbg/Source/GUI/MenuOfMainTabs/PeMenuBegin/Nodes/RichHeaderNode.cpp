#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    extern PEInformation objPEInformation;
    extern PeReader objPeReader;
}

void PETab_::PeRichHeaderTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
    if (ImGui::BeginTable("b", 6, DefaultTableFlags))
    {

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("@comp.id", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Counter", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Version", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Tool", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Toolset", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();


        for (int Row = 0; Row < 5; Row++)
        {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
            for (int Column = 0; Column < 6; ++Column)
            {
                Window.SelectAllAndColored(6, Dimensionses.U32GrayColor);

                ImGui::TableSetColumnIndex(Column);

                ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);

                if (Column == 3)
                {
                    ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.RichHeader.RichHeaderNames[Row].data());
                }
                ImGui::PopStyleColor(2);
            }
        }

        ImGui::EndTable();
    }
}