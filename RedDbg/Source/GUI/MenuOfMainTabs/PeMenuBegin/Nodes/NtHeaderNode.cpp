#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
    //extern PEInformation objPEInformation;
    //extern PeReader objPeReader;
}

void PETab_::PeNtHeaderTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
    if (ImGui::BeginTable("c", 7, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Structure Field", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Offset", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        for (int Row = 0; Row < 1; Row++)
        {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
            for (int Column = 0; Column < 7; ++Column)
            {
                Window.SelectAllAndColored(6, Dimensionses.U32GrayColor);

                ImGui::TableSetColumnIndex(Column);

                ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32TransparentColor);
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32TransparentColor);

                if (Column == 0)
                {
                    ImGui::Selectable("Signature");
                }
                else if (Column == 1)
                {
                    std::stringstream Ss;
                    Ss << std::uppercase << std::hex << GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfanew;

                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 2)
                {
                    ImGui::Selectable("Dword");
                }
                else if (Column == 3)
                {
                    std::stringstream Ss;
                    if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint64_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.ImageBase + (uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfanew;
                    }
                    else
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint32_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.ImageBase + (uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfanew;
                    }
                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 4)
                {
                    uint32_t* Nt =
                        (uint32_t*)((uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->fileBuffer->buf + (uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfanew);

                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                    static std::string a;
                    EditField<uint32_t>(
                        "##" + std::to_string(Row),
                        a,
                        Nt, 9);
                    ImGui::PopStyleColor();
                }
                else if (Column == 5)
                {
                    ImGui::Selectable(
                        (char*)((uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->fileBuffer->buf + (uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfanew));
                }

                ImGui::PopStyleColor(2);
            }
        }

        ImGui::EndTable();
    }
}
