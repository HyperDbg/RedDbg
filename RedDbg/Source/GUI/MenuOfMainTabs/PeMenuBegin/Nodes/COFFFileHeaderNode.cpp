#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    extern PEInformation objPEInformation;
    extern PeReader objPeReader;
}

void PETab_::PeFileHeaderTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

    static bool Init = false;

    static std::vector<WORD*> FileHeaderWordVals{
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->FileHeader.Machine,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->FileHeader.NumberOfSections,
        nullptr,
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->FileHeader.SizeOfOptionalHeader,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->FileHeader.Characteristics
    };

    static std::vector<DWORD*> FileHeaderDWordVals{
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->FileHeader.TimeDateStamp,//
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->FileHeader.PointerToSymbolTable,//
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->FileHeader.NumberOfSymbols,//
        nullptr,
        nullptr,
    };

    static std::vector<std::string> InputStrings;

    if (!Init)
    {
        InputStrings.resize(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.FileHeaderNames.size());
        Init = true;
    }

    if (ImGui::BeginTable("d", 7, DefaultTableFlags))
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

        for (int Row = 0; Row < 7; Row++)
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
                    ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.FileHeaderNames[Row].data());
                }
                else if (Column == 1)
                {
                    std::stringstream Ss;
                    Ss << std::uppercase << std::hex << PeStruct::FileHeader::FileHeaderOffsets[Row];

                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 2)
                {
                    if (Row < 2 || Row > 4)
                    {
                        ImGui::Selectable("Word");
                    }
                    if (Row >= 2 && Row <= 4)
                    {
                        ImGui::Selectable("Dword");
                    }
                }
                else if (Column == 3)
                {
                    std::stringstream Ss;
                    if (GlobalVarsOfPeTab::objPEInformation.x64.pImageNTHeader64 != nullptr)
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint64_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation.x64.pImageNTHeader64->OptionalHeader.ImageBase + PeStruct::FileHeader::FileHeaderOffsets[Row];
                    }
                    else
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint32_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation.x32.pImageNTHeader32->OptionalHeader.ImageBase + PeStruct::FileHeader::FileHeaderOffsets[Row];
                    }
                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 4)
                {
                    if (Row != 0 && Row != 2 && Row != 3 && Row != 4 && Row != 6)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        EditField<WORD>(
                            "##" + std::to_string(Row),
                            InputStrings[Row],
                            FileHeaderWordVals[Row], 5);
                        ImGui::PopStyleColor();
                    }
                    else if (Row == 0)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                        EditField<WORD>(
                            "##" + std::to_string(Row),
                            InputStrings[0],
                            FileHeaderWordVals[0], 5);
                        ImGui::PopStyleColor();

                        if (ImGui::BeginPopupContextItem("asdasd", ImGuiPopupFlags_MouseButtonRight))
                        {
                            for (int SubRows = 0; SubRows < Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.MachineNames.size(); ++SubRows)
                            {
                                static std::vector<bool> Opened(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.MachineNames.size());

                                const bool& Opened_ = Opened[SubRows];
                                const std::string CheckNodeId = "##CheckBox" + std::to_string(SubRows);
                                ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                                CheckBoxOpened(&Opened, Opened_, SubRows);

                                ImGui::SameLine();

                                ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.MachineNames[SubRows].data());
                                if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip) && ImGui::BeginTooltip())
                                {
                                    ImGui::SetTooltip(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.MachineToolTips[SubRows].data());
                                    ImGui::EndTooltip();
                                }
                            }
                            ImGui::EndPopup();
                        }
                    }
                    else if (Row >= 2 && Row <= 4)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        EditField<DWORD>(
                            "##" + std::to_string(Row),
                            InputStrings[Row],
                            FileHeaderDWordVals[Row], 9);
                        ImGui::PopStyleColor();
                    }
                    else if (Row == 6)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                        EditField<WORD>(
                            "##" + std::to_string(Row),
                            InputStrings[6],
                            FileHeaderWordVals[6], 5);
                        ImGui::PopStyleColor();

                        if (ImGui::BeginPopupContextItem("as", ImGuiPopupFlags_MouseButtonRight))
                        {
                            for (int SubRows = 0; SubRows < Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.CharateristicsNames.size(); ++SubRows)
                            {
                                static std::vector<bool> Opened(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.CharateristicsNames.size());

                                const bool& Opened_ = Opened[SubRows];
                                const std::string CheckNodeId = "##CheckBox" + std::to_string(SubRows);
                                ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                                Opened[SubRows] = Opened_;

                                ImGui::SameLine();

                                ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileHeader.CharateristicsNames[SubRows].data());
                            }

                            ImGui::EndPopup();
                        }
                    }
                }
                else if (Column == 5)
                {

                }

                ImGui::PopStyleColor(2);
            }
        }
        ImGui::EndTable();
    }
}