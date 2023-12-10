#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    extern PEInformation objPEInformation;
    extern PeReader objPeReader;
}

void PETab_::PeDataDirectoriesTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

    static bool Init = false;

    static std::vector<DWORD*> DataDirectoriesDwordVals{
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_ARCHITECTURE].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size,
    };

    static std::vector<std::string> InputStrings;

    if (!Init)
    {
        InputStrings.resize(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.DataDirectories.DataDirectoriesNames.size());
        Init = true;
    }

    if (ImGui::BeginTable("c", 7, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Directory Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Offset", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        for (int Row = 0; Row < Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.DataDirectories.DataDirectoriesNames.size(); Row++)
        {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
            for (int Column = 0; Column < 7; ++Column)
            {
                Window.SelectAllAndColored(7, Dimensionses.U32GrayColor);

                ImGui::TableSetColumnIndex(Column);

                ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32TransparentColor);
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32TransparentColor);

                if (Column == 0)
                {
                    ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.DataDirectories.DataDirectoriesNames[Row].data());
                }
                else if (Column == 1)
                {
                    std::stringstream Ss;
                    Ss << std::uppercase << std::hex << PeStruct::DataDirectories::DataDirectoriesOffsets[Row];

                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 2)
                {
                    ImGui::Selectable("Dword");
                }
                else if (Column == 3)
                {
                    std::stringstream Ss;
                    if (GlobalVarsOfPeTab::objPEInformation.x64.pImageNTHeader64 != nullptr)
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint64_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation.x64.pImageNTHeader64->OptionalHeader.ImageBase + PeStruct::DataDirectories::DataDirectoriesOffsets[Row];
                    }
                    else
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint32_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation.x32.pImageNTHeader32->OptionalHeader.ImageBase + PeStruct::DataDirectories::DataDirectoriesOffsets[Row];
                    }
                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 4)
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                    EditField<DWORD>(
                        "##" + std::to_string(Row),
                        InputStrings[Row],
                        DataDirectoriesDwordVals[Row], 9);
                    ImGui::PopStyleColor();
                }
                ImGui::PopStyleColor(2);
            }
        }
        ImGui::EndTable();
    }
}