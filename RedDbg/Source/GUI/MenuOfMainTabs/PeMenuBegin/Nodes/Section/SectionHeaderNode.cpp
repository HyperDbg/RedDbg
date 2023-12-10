#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    std::vector<float> Entropy;

    extern PEInformation objPEInformation;
    extern PeReader objPeReader;

    static std::vector<bool> Opened;
}

void PETab_::PeSectionHeaderTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

    static bool Init = false;

    std::vector<DWORD> SectionDwordSizeVals{};
    for (int Section = 0; Section < GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections - 1; ++Section)
    {
        DWORD Size =
            (GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Section].Misc.VirtualSize +4095) & ~4095;
        SectionDwordSizeVals.push_back(Size);
    }

    static std::vector<DWORD*> SectionDwordPointersSizeVals{};
    static std::vector<DWORD*> SectionDwordPointersAddressVals{};
    static std::vector<DWORD*> SectionDwordPointersRawOffsetsVals{};
    static std::vector<DWORD*> SectionDwordPointersRawSizesVals{};
    static std::vector<DWORD*> SectionDwordPointersCharateristicsVals{};
    if (!SectionDwordPointersAddressVals.size())
    {
        for (int Section = 0; Section < GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections - 1; ++Section)
        {
            SectionDwordPointersSizeVals.push_back(&GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Section].Misc.VirtualSize);
            SectionDwordPointersAddressVals.push_back(&GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Section].VirtualAddress);
            SectionDwordPointersRawOffsetsVals.push_back(&GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Section].PointerToRawData);
            SectionDwordPointersRawSizesVals.push_back(&GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Section].SizeOfRawData);
            SectionDwordPointersCharateristicsVals.push_back(&GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Section].Characteristics);
        }
    }

    static std::vector<std::string> InputSizeStrings;
    static std::vector<std::string> InputAddressStrings;
    static std::vector<std::string> InputAddress64Strings;
    static std::vector<std::string> InputRawOffsetsStrings;
    static std::vector<std::string> InputRawSizesStrings;
    static std::vector<std::string> InputCharateristicsStrings;
    if (!Init)
    {
        InputSizeStrings.resize(GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections);
        InputAddressStrings.resize(GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections);
        InputAddress64Strings.resize(GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections);
        InputRawOffsetsStrings.resize(GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections);
        InputRawSizesStrings.resize(GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections);
        InputCharateristicsStrings.resize(GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections);
        Init = true;
    }

    if (ImGui::BeginTable("l", 9, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("##Info", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Section Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Entropy", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Virtual Size", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Virtual Address", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Raw Offset", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Raw Size", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Flags", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        for (int Row = 0; Row < GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections - 1; ++Row)
        {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
            for (int Column = 0; Column < 9; ++Column)
            {
                Window.SelectAllAndColored(9, Dimensionses.U32GrayColor);

                ImGui::TableSetColumnIndex(Column);

                ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32TransparentColor);
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32TransparentColor);
                if (Column == 0)
                {
                    const bool& Opened_ = GlobalVarsOfPeTab::Opened[Row];
                    const std::string CheckNodeId = "##CheckBox" + std::to_string(Row);
                    ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                    CheckBoxOpened(&GlobalVarsOfPeTab::Opened, Opened_, Row);
                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip) && ImGui::BeginTooltip())
                    {
                        ImGui::SetTooltip("Section Info");
                        ImGui::EndTooltip();
                    }
                }
                else if (Column == 1)
                {
                    ImGui::Selectable((char*)GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Row].Name);
                    if (ImGui::BeginPopupContextItem())
                    {
                        ImGui::PushStyleColor(ImGuiCol_Button, Dimensionses.U32TransparentColor);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Dimensionses.U32GrayColor);
                        if (ImGui::Button("Rename"))
                        {

                        }
                        ImGui::PopStyleColor(2);
                        ImGui::EndPopup();
                    }
                }
                else if (Column == 2)
                {
                    int Offset = GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Row].PointerToRawData;
                    int Length = GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Row].SizeOfRawData;

                    if (GlobalVarsOfPeTab::Entropy.size() != GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections)
                    {
                        GlobalVarsOfPeTab::Entropy.push_back(GlobalVarsOfPeTab::objPeReader.CalcEntropy((char*)GlobalVarsOfPeTab::objPEInformation.pImageDOSHeaderOfPe, Offset, Offset + Length));
                    }

                    ImGui::Selectable(std::to_string(GlobalVarsOfPeTab::Entropy[Row]).c_str());
                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip) && ImGui::BeginTooltip())
                    {
                        if (GlobalVarsOfPeTab::Entropy[Row] > 6.75)
                        {
                            ImGui::SetTooltip("Packed");
                        }
                        if (GlobalVarsOfPeTab::Entropy[Row] < 6.75 && GlobalVarsOfPeTab::Entropy[Row] > 6.5)
                        {
                            ImGui::SetTooltip("May be packed");
                        }
                        if (GlobalVarsOfPeTab::Entropy[Row] < 6.5)
                        {
                            ImGui::SetTooltip("Not packed");
                        }
                        ImGui::EndTooltip();
                    }
                }
                else if (Column == 3)
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                    EditField<DWORD>(
                        "##" + std::to_string(Row),
                        InputSizeStrings[Row],
                        &SectionDwordSizeVals[Row],
                        9, 
                        false,
                        SectionDwordPointersSizeVals[Row]);

                    ImGui::PopStyleColor();
                }
                else if (Column == 4)
                {
                    static std::vector<bool> Opened(GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections - 1);
                    auto Iterator = std::find(Opened.begin(), Opened.end(), true);
                    if (Iterator != Opened.end() && Opened[Row] == true)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                        DWORD64 Ep =
                            (DWORD64)GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Row].VirtualAddress + GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.ImageBase;
                        std::stringstream Ss;
                        Ss << std::setfill('0') << std::setw(sizeof(uint64_t) * 2) << std::uppercase << std::hex << Ep << " ";
                        InputAddress64Strings[Row] = Ss.str();
                        EditField<DWORD>(
                            "##" + std::to_string((Row + 20) * 1000),
                            InputAddress64Strings[Row],
                            SectionDwordPointersAddressVals[Row],
                            17,
                            false,
                            nullptr,
                            true,
                            GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe->OptionalHeader.ImageBase,
                            true);
                        ImGui::PopStyleColor();
                    }
                    else
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        EditField<DWORD>(
                            "##" + std::to_string((Row + 20) * 1000),
                            InputAddressStrings[Row],
                            SectionDwordPointersAddressVals[Row],
                            9);

                        ImGui::PopStyleColor();
                    }

                    if (ImGui::BeginPopupContextItem())
                    {
                        const bool& Opened_ = Opened[Row];
                        const std::string CheckNodeId = "##CheckBox" + std::to_string(Row);
                        ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                        CheckBoxOpened(&Opened, Opened_, Row);

                        ImGui::SameLine();

                        ImGui::Selectable("Qword input type");

                        ImGui::EndPopup();
                    }
                }
                else if (Column == 5)
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                    EditField<DWORD>(
                        "##" + std::to_string((Row + 30) * 2000),
                        InputAddressStrings[Row],
                        SectionDwordPointersRawOffsetsVals[Row],
                        9);

                    ImGui::PopStyleColor();
                }
                else if (Column == 6)
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                    EditField<DWORD>(
                        "##" + std::to_string((Row + 40) * 3000),
                        InputRawSizesStrings[Row],
                        SectionDwordPointersRawSizesVals[Row],
                        9);

                    ImGui::PopStyleColor();
                }
                else if (Column == 7)
                {
                    static std::vector<bool> Opened(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.SectionHeader.SectionCharateristics.size());
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                    EditField<DWORD>(
                        "##" + std::to_string((Row + 50) * 4000),
                        InputCharateristicsStrings[Row],
                        SectionDwordPointersCharateristicsVals[Row],
                        9);

                    ImGui::PopStyleColor();

                    if (ImGui::BeginPopupContextItem())
                    {
                        for (int SubRows = 0; SubRows < Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.SectionHeader.SectionCharateristics.size(); ++SubRows)
                        {
                            const bool& Opened_ = Opened[SubRows];
                            const std::string CheckNodeId = "##CheckBox" + std::to_string(SubRows);
                            ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                            Opened[SubRows] = Opened_;

                            ImGui::SameLine();

                            ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.SectionHeader.SectionCharateristics[SubRows].data());
                        }
                        ImGui::EndPopup();
                    }
                }
                ImGui::PopStyleColor(2);
            }
        }

        ImGui::EndTable();
    }
}

void PETab_::PeInfoOfSectionInSectionHeaderWindowRender()
{

}

void PETab_::PeSectionHeaderInternalWindowRender()
{
    GlobalVarsOfPeTab::Opened.resize(GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections - 1);
    auto Iterator = std::find(GlobalVarsOfPeTab::Opened.begin(), GlobalVarsOfPeTab::Opened.end(), true);
    if (Iterator != GlobalVarsOfPeTab::Opened.end())
    {
        static float PEHorizontalSplitterRelPosY = 0.8f;
        static int StartPosY = 0;

        Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
            Dimensionses.TitleBarHeight,
            Dimensionses.SplitterSize,
            Dimensionses.SplitterIsDisabled,
            PEHorizontalSplitterRelPosY,
            WindowCodes::PeSectionHeaderTwoWindowInternalWindowCode);

        ImVec2 posMax{};
        posMax.x += WindowGeneralSizes.RightPos.x + WindowGeneralSizes.RightSize.x;
        posMax.y += WindowGeneralSizes.RightPos.y + WindowGeneralSizes.RightSize.y;
        if (Window.SplitterHorizontal(
            Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PEHorizontalSplitterName.data(),
            &WindowGeneralSizes.SplitterHPosY,
            Dimensionses.SplitterSize,
            WindowGeneralSizes.LeftPos,
            posMax,
            &StartPosY))
        {
            PEHorizontalSplitterRelPosY =
                ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
        }

        Window.ChildWindowRender(
            Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.SectionHeader.TableWindowName.data(),
            WindowGeneralSizes.LeftTopPosition,
            WindowGeneralSizes.LeftTopSize,
            WindowCodes::PeSectionHeaderTableNodeCode,
            *this,
            false);

        Window.ChildWindowRender(
            Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.SectionHeader.InfoSectionWindowName.data(),
            WindowGeneralSizes.CenterBottomPosition,
            WindowGeneralSizes.CenterBottomSize,
            WindowCodes::PeInfoOfSectionInHeaderWindowCode,
            *this,
            true);
    }
    else
    {
        Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
            Dimensionses.TitleBarHeight,
            Dimensionses.SplitterSize,
            Dimensionses.SplitterIsDisabled,
            Dimensionses.SplitterIsDisabled,
            WindowCodes::PeSectionHeaderTwoWindowInternalWindowCode);

        Window.ChildWindowRender(
            Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.SectionHeader.TableWindowName.data(),
            WindowGeneralSizes.LeftTopPosition,
            WindowGeneralSizes.LeftTopSize,
            WindowCodes::PeSectionHeaderTableNodeCode,
            *this,
            false);
    }
}