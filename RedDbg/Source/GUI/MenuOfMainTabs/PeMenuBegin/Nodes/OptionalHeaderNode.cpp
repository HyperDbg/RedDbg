#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    extern PEInformation objPEInformation;
    extern PeReader objPeReader;
}

void PETab_::PeOptionalHeaderTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

    static bool Init = false;

    static std::vector<WORD*> OptionalHeaderWordVals{
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.Magic,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.MajorOperatingSystemVersion,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.MinorOperatingSystemVersion,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.MajorImageVersion,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.MinorImageVersion,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.MajorSubsystemVersion,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.MinorSubsystemVersion,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.Subsystem,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.DllCharacteristics,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
    };

    static std::vector<DWORD*> FileHeaderDwordVals{
        nullptr,
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfCode,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfInitializedData,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfUninitializedData,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.AddressOfEntryPoint,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.BaseOfCode,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SectionAlignment,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.FileAlignment,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.Win32VersionValue,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfImage,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfHeaders,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.CheckSum,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.LoaderFlags,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.NumberOfRvaAndSizes,
    };

    static std::vector<ULONGLONG*> FileHeaderQwordVals{
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.ImageBase,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfStackReserve,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfStackCommit,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfHeapReserve,
        &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.SizeOfHeapCommit,
        nullptr,
        nullptr,
    };

    static std::vector<std::string> InputStrings;

    if (!Init)
    {
        InputStrings.resize(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.OptionalHeaderNames.size());
        Init = true;
    }

    if (ImGui::BeginTable("g", 7, DefaultTableFlags))
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

        for (int Row = 0; Row < Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.OptionalHeaderNames.size(); Row++)
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
                    ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.OptionalHeaderNames[Row].data());
                }
                else if (Column == 1)
                {
                    std::stringstream Ss;
                    Ss << std::uppercase << std::hex << PeStruct::OptionalHeader::OptionalHeaderOffsets[Row];

                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 2)
                {
                    if (Row == 0 || Row >= 11 && Row <= 16 || Row >= 21 && Row <= 22)
                    {
                        ImGui::Selectable("Word");
                    }
                    else if (Row == 1)
                    {
                        ImGui::Selectable("Byte");
                    }
                    else if (Row == 2)
                    {
                        ImGui::Selectable("Byte");
                    }
                    else if (Row >= 3 && Row <= 7 || Row >= 9 && Row <= 10 || Row >= 17 && Row <= 20 || Row >= 27 && Row <= 28)
                    {
                        ImGui::Selectable("Dword");
                    }
                    else if (Row == 8 || Row >= 23 && Row <= 26)
                    {
                        ImGui::Selectable("Qword");
                    }
                }
                else if (Column == 3)
                {
                    std::stringstream Ss;
                    if (GlobalVarsOfPeTab::objPEInformation.x64.pImageNTHeader64 != nullptr)
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint64_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation.x64.pImageNTHeader64->OptionalHeader.ImageBase + PeStruct::OptionalHeader::OptionalHeaderOffsets[Row];
                    }
                    else
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint32_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation.x32.pImageNTHeader32->OptionalHeader.ImageBase + PeStruct::OptionalHeader::OptionalHeaderOffsets[Row];
                    }
                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 4)
                {
                    if (Row == 0 || Row >= 11 && Row <= 16 || Row >= 21 && Row <= 22)
                    {
                        if (Row == 0)
                        {
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                            EditField<WORD>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                OptionalHeaderWordVals[Row], 5);
                            ImGui::PopStyleColor();

                            if (ImGui::BeginPopupContextItem("as", ImGuiPopupFlags_MouseButtonRight))
                            {
                                for (int SubRows = 0; SubRows < Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.MagicNames.size(); ++SubRows)
                                {
                                    static std::vector<bool> Opened(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.MagicNames.size());

                                    const bool& Opened_ = Opened[SubRows];
                                    const std::string CheckNodeId = "##CheckBox" + std::to_string(SubRows);
                                    ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                                    Window.CheckBoxOpened(&Opened, Opened_, SubRows);

                                    ImGui::SameLine();

                                    ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.MagicNames[SubRows].data());
                                }
                                ImGui::EndPopup();
                            }
                        }
                        else if (Row == 21)
                        {
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                            EditField<WORD>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                OptionalHeaderWordVals[Row], 5);
                            ImGui::PopStyleColor();

                            if (ImGui::BeginPopupContextItem("asas", ImGuiPopupFlags_MouseButtonRight))
                            {
                                for (int SubRows = 0; SubRows < Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.SubsystemNames.size(); ++SubRows)
                                {
                                    static std::vector<bool> Opened(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.SubsystemNames.size());

                                    const bool& Opened_ = Opened[SubRows];
                                    const std::string CheckNodeId = "##CheckBox" + std::to_string(SubRows);
                                    ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                                    Window.CheckBoxOpened(&Opened, Opened_, SubRows);

                                    ImGui::SameLine();

                                    ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.SubsystemNames[SubRows].data());
                                }
                                ImGui::EndPopup();
                            }
                        }
                        else if (Row == 22)
                        {
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                            EditField<WORD>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                OptionalHeaderWordVals[Row], 5);
                            ImGui::PopStyleColor();

                            if (ImGui::BeginPopupContextItem("asasas", ImGuiPopupFlags_MouseButtonRight))
                            {
                                for (int SubRows = 0; SubRows < Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.DllCharateristicsNames.size(); ++SubRows)
                                {
                                    static std::vector<bool> Opened(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.DllCharateristicsNames.size());

                                    const bool& Opened_ = Opened[SubRows];
                                    const std::string CheckNodeId = "##CheckBox" + std::to_string(SubRows);
                                    ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                                    Opened[SubRows] = Opened_;

                                    ImGui::SameLine();

                                    ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.OptionalHeader.DllCharateristicsNames[SubRows].data());
                                }
                                ImGui::EndPopup();
                            }
                        }
                        else
                        {
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                            EditField<WORD>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                OptionalHeaderWordVals[Row], 5);
                            ImGui::PopStyleColor();
                        }
                    }
                    else if (Row == 1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        EditField<BYTE>(
                            "##" + std::to_string(Row),
                            InputStrings[Row],
                            &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.MajorLinkerVersion, 3, true);
                        ImGui::PopStyleColor();
                    }
                    else if (Row == 2)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        EditField<BYTE>(
                            "##" + std::to_string(Row),
                            InputStrings[Row],
                            &GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.MinorLinkerVersion, 3, true);
                        ImGui::PopStyleColor();
                    }
                    else if (Row >= 3 && Row <= 7 || Row >= 9 && Row <= 10 || Row >= 17 && Row <= 20 || Row >= 27 && Row <= 28)
                    {
                        if (Row == 6)
                        {
                            static bool Opened = false;

                            if (Opened != false)
                            {
                                ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                                DWORD64 Ep =
                                    (DWORD64)GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.AddressOfEntryPoint + GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.ImageBase;

                                EditField<DWORD64>(
                                    "##" + std::to_string(Row),
                                    InputStrings[Row],
                                    &Ep, 17);
                                ImGui::PopStyleColor();
                            }
                            else
                            {
                                ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                                EditField<DWORD>(
                                    "##" + std::to_string(Row),
                                    InputStrings[Row],
                                    FileHeaderDwordVals[Row], 9);
                                ImGui::PopStyleColor();
                            }
                            if (ImGui::BeginPopupContextItem("asdasdasdad", ImGuiPopupFlags_MouseButtonRight))
                            {
                                const std::string CheckNodeId = "##CheckBox" + std::to_string(Row);
                                ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened);

                                ImGui::SameLine();

                                ImGui::Selectable("Qword input type");

                                ImGui::EndPopup();
                            }
                        }
                        else
                        {
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                            EditField<DWORD>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                FileHeaderDwordVals[Row], 9);
                            ImGui::PopStyleColor();
                        }
                    }
                    else if (Row == 8 || Row >= 23 && Row <= 26)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        EditField<ULONGLONG>(
                            "##" + std::to_string(Row),
                            InputStrings[Row],
                            FileHeaderQwordVals[Row], 17);
                        ImGui::PopStyleColor();
                    }
                }
                ImGui::PopStyleColor(2);
            }
        }
        ImGui::EndTable();
    }
}