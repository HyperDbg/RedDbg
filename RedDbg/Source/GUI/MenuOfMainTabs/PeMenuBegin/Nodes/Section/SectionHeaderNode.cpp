#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    std::vector<double> Entropy;

    extern std::shared_ptr<PeReader> objPEInformation;
    //extern PeReader objPeReader;

    static std::vector<bool> Opened;
}

typedef struct _Section
{
    std::vector<const uint32_t*> SectionDwordPointersSizeVals{};
    std::vector<const uint32_t*> SectionDwordPointersAddressVals{};
    std::vector<const uint32_t*> SectionDwordPointersRawOffsetsVals{};
    std::vector<const uint32_t*> SectionDwordPointersRawSizesVals{};
    std::vector<const uint32_t*> SectionDwordPointersCharateristicsVals{};
    std::vector<uint32_t> SectionDwordSizeVals{};

} Section, *PSection;

void PETab_::PeSectionHeaderTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

    static bool Init = false;

    Section SectionPointers;
    auto L_inspectSection = [](void* N,
        const peparse::VA& secBase,
        const std::string& secName,
        const peparse::image_section_header& SectionHeader,
        const peparse::bounded_buffer* data) -> int {
            static_cast<void>(secBase);
            static_cast<void>(secName);
            static_cast<void>(data);

            auto SectionPointers = static_cast<PSection>(N);
            DWORD Size = (SectionHeader.Misc.VirtualSize + 4095) & ~4095;
            SectionPointers->SectionDwordSizeVals.push_back(Size);


            SectionPointers->SectionDwordPointersSizeVals.push_back(&SectionHeader.Misc.VirtualSize);
            SectionPointers->SectionDwordPointersAddressVals.push_back(&SectionHeader.VirtualAddress);
            SectionPointers->SectionDwordPointersRawOffsetsVals.push_back(&SectionHeader.PointerToRawData);
            SectionPointers->SectionDwordPointersRawSizesVals.push_back(&SectionHeader.SizeOfRawData);
            SectionPointers->SectionDwordPointersCharateristicsVals.push_back(&SectionHeader.Characteristics);
            return 0;
        };
    peparse::IterSec(GlobalVarsOfPeTab::objPEInformation->Pe, L_inspectSection, &SectionPointers);

    static std::vector<std::string> InputSizeStrings;
    static std::vector<std::string> InputAddressStrings;
    static std::vector<std::string> InputAddress64Strings;
    static std::vector<std::string> InputRawOffsetsStrings;
    static std::vector<std::string> InputRawSizesStrings;
    static std::vector<std::string> InputCharateristicsStrings;
    if (!Init)
    {
        InputSizeStrings.resize(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections);
        InputAddressStrings.resize(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections);
        InputAddress64Strings.resize(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections);
        InputRawOffsetsStrings.resize(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections);
        InputRawSizesStrings.resize(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections);
        InputCharateristicsStrings.resize(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections);
        Init = true;
    }

    static PIMAGE_NT_HEADERS pImageNTHeader = (PIMAGE_NT_HEADERS)((uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->fileBuffer->buf + (uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfanew);
    static PIMAGE_SECTION_HEADER pImageSectionHeader = (PIMAGE_SECTION_HEADER)((uint64_t)pImageNTHeader + 4 + sizeof(IMAGE_FILE_HEADER) + GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.SizeOfOptionalHeader);

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

        for (int Row = 0; Row < GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections - 1; ++Row)
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
                    Window.CheckBoxOpened(&GlobalVarsOfPeTab::Opened, Opened_, Row);
                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip) && ImGui::BeginTooltip())
                    {
                        ImGui::SetTooltip("Section Info");
                        ImGui::EndTooltip();
                    }
                }
                else if (Column == 1)
                {
                    ImGui::Selectable((char*)pImageSectionHeader[Row].Name);
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
                    int Offset = pImageSectionHeader[Row].PointerToRawData;
                    int Length = pImageSectionHeader[Row].SizeOfRawData;

                    if (GlobalVarsOfPeTab::Entropy.size() != GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections)
                    {
                        GlobalVarsOfPeTab::Entropy.push_back(CalcEntropy(GlobalVarsOfPeTab::objPEInformation->Pe->fileBuffer->buf, Offset, Offset + Length));
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

                    EditField<uint32_t>(
                        "##" + std::to_string(Row),
                        InputSizeStrings[Row],
                        &SectionPointers.SectionDwordSizeVals[Row],
                        9,
                        false,
                        (uint32_t*)SectionPointers.SectionDwordPointersSizeVals[Row]);

                    ImGui::PopStyleColor();
                }
                else if (Column == 4)
                {
                    static std::vector<bool> Opened(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections - 1);
                    auto Iterator = std::find(Opened.begin(), Opened.end(), true);
                    if (Iterator != Opened.end() && Opened[Row] == true)
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                        DWORD64 Ep;
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            Ep = (DWORD64)pImageSectionHeader[Row].VirtualAddress + GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.ImageBase;
                        }
                        else
                        {
                            Ep = (DWORD64)pImageSectionHeader[Row].VirtualAddress + GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.ImageBase;
                        }

                        std::stringstream Ss;
                        Ss << std::setfill('0') << std::setw(sizeof(uint64_t) * 2) << std::uppercase << std::hex << Ep << " ";
                        InputAddress64Strings[Row] = Ss.str();
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            EditField<uint32_t>(
                                "##" + std::to_string((Row + 20) * 1000),
                                InputAddress64Strings[Row],
                                (uint32_t*)SectionPointers.SectionDwordPointersAddressVals[Row],
                                17,
                                false,
                                nullptr,
                                true,
                                GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.ImageBase,
                                true);
                        }
                        else
                        {
                            EditField<uint32_t>(
                                "##" + std::to_string((Row + 20) * 1000),
                                InputAddress64Strings[Row],
                                (uint32_t*)SectionPointers.SectionDwordPointersAddressVals[Row],
                                17,
                                false,
                                nullptr,
                                true,
                                GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.ImageBase,
                                true);
                        }
                        ImGui::PopStyleColor();
                    }
                    else
                    {
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                        EditField<uint32_t>(
                            "##" + std::to_string((Row + 20) * 1000),
                            InputAddressStrings[Row],
                            (uint32_t*)SectionPointers.SectionDwordPointersAddressVals[Row],
                            9);

                        ImGui::PopStyleColor();
                    }

                    if (ImGui::BeginPopupContextItem())
                    {
                        const bool& Opened_ = Opened[Row];
                        const std::string CheckNodeId = "##CheckBox" + std::to_string(Row);
                        ImGui::Checkbox(CheckNodeId.c_str(), (bool*)&Opened_);
                        Window.CheckBoxOpened(&Opened, Opened_, Row);

                        ImGui::SameLine();

                        ImGui::Selectable("Qword input type");

                        ImGui::EndPopup();
                    }
                }
                else if (Column == 5)
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                    EditField<uint32_t>(
                        "##" + std::to_string((Row + 30) * 2000),
                        InputAddressStrings[Row],
                        (uint32_t*)SectionPointers.SectionDwordPointersRawOffsetsVals[Row],
                        9);

                    ImGui::PopStyleColor();
                }
                else if (Column == 6)
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                    EditField<uint32_t>(
                        "##" + std::to_string((Row + 40) * 3000),
                        InputRawSizesStrings[Row],
                        (uint32_t*)SectionPointers.SectionDwordPointersRawSizesVals[Row],
                        9);

                    ImGui::PopStyleColor();
                }
                else if (Column == 7)
                {
                    static std::vector<bool> Opened(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.SectionHeader.SectionCharateristics.size());
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);

                    EditField<uint32_t>(
                        "##" + std::to_string((Row + 50) * 4000),
                        InputCharateristicsStrings[Row],
                        (uint32_t*)SectionPointers.SectionDwordPointersCharateristicsVals[Row],
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
    GlobalVarsOfPeTab::Opened.resize(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections - 1);
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
