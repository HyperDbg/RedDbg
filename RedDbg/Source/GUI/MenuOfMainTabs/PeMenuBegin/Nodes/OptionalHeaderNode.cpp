#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
    //extern PeReader objPeReader;
}

bool PETab_::OptionalBitnessRowModeChooserA(int Row)
{
    if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        return Row >= 3 && Row <= 7 || Row >= 9 && Row <= 10 || Row >= 17 && Row <= 20 || Row >= 27 && Row <= 28;
    }
    else
    {
        return Row >= 3 && Row <= 8 || Row >= 9 && Row <= 10 || Row >= 17 && Row <= 20 || Row >= 23 && Row <= 28;
    }
}

bool PETab_::OptionalBitnessRowModeChooserB(int Row)
{
    if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        return Row == 8 || Row >= 23 && Row <= 26;
    }
    else
    {
        return Row == -1;
    }
}

void PETab_::PeOptionalHeaderTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

    static bool Init = false;

    static std::vector<uint16_t*> OptionalHeaderWordVals;
    static std::vector<uint32_t*> FileHeaderDwordVals;
    static std::vector<uint64_t*> FileHeaderQwordVals;
    if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
    {
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.Magic);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.MajorOperatingSystemVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.MinorOperatingSystemVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.MajorImageVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.MinorImageVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.MajorSubsystemVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.MinorSubsystemVersion);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.Subsystem);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.DllCharacteristics);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);

        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfCode);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfInitializedData);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfUninitializedData);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.AddressOfEntryPoint);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.BaseOfCode);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SectionAlignment);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.FileAlignment);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.Win32VersionValue);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfImage);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfHeaders);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.CheckSum);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.LoaderFlags);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.NumberOfRvaAndSizes);

        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.ImageBase);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfStackReserve);
        FileHeaderQwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfStackCommit);
        FileHeaderQwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfHeapReserve);
        FileHeaderQwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.SizeOfHeapCommit);
        FileHeaderQwordVals.push_back(nullptr);
        FileHeaderQwordVals.push_back(nullptr);
    }
    else
    {
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.Magic);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.MajorOperatingSystemVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.MinorOperatingSystemVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.MajorImageVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.MinorImageVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.MajorSubsystemVersion);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.MinorSubsystemVersion);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.Subsystem);
        OptionalHeaderWordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.DllCharacteristics);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);
        OptionalHeaderWordVals.push_back(nullptr);

        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfCode);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfInitializedData);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfUninitializedData);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.AddressOfEntryPoint);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.BaseOfCode);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.ImageBase);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SectionAlignment);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.FileAlignment);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.Win32VersionValue);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfImage);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfHeaders);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.CheckSum);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(nullptr);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfStackReserve);//
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfStackCommit);//
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfHeapReserve);//
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.SizeOfHeapCommit);//
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.LoaderFlags);
        FileHeaderDwordVals.push_back(&GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.NumberOfRvaAndSizes);
    }

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
                    if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint64_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.ImageBase + PeStruct::OptionalHeader::OptionalHeaderOffsets[Row];
                    }
                    else
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint32_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.ImageBase + PeStruct::OptionalHeader::OptionalHeaderOffsets[Row];
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

                            EditField<uint16_t>(
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

                            EditField<uint16_t>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                OptionalHeaderWordVals[Row], 5);
                            ImGui::PopStyleColor();
                        }
                    }
                    else if (Row == 1)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            EditField<uint8_t>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.MajorLinkerVersion, 3, true);
                        }
                        else
                        {
                            EditField<uint8_t>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.MajorLinkerVersion, 3, true);
                        }
                        ImGui::PopStyleColor();
                    }
                    else if (Row == 2)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            EditField<uint8_t>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.MinorLinkerVersion, 3, true);
                        }
                        else
                        {
                            EditField<uint8_t>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.MinorLinkerVersion, 3, true);
                        }
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
                                DWORD64 Ep;
                                if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                                {
                                    Ep = (DWORD64)GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.AddressOfEntryPoint + GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.ImageBase;
                                    EditField<DWORD64>(
                                        "##" + std::to_string(Row),
                                        InputStrings[Row],
                                        &Ep, 17);
                                }
                                else
                                {
                                    Ep = (DWORD64)GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.AddressOfEntryPoint + GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.ImageBase;
                                    EditField<DWORD64>(
                                        "##" + std::to_string(Row),
                                        InputStrings[Row],
                                        &Ep, 9);
                                }
                                ImGui::PopStyleColor();
                            }
                            else
                            {
                                ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                                EditField<uint32_t>(
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

                            EditField<uint32_t>(
                                "##" + std::to_string(Row),
                                InputStrings[Row],
                                FileHeaderDwordVals[Row], 9);
                            ImGui::PopStyleColor();
                        }
                    }
                    else if (Row == 8 || Row >= 23 && Row <= 26)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        EditField<uint64_t>(
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
