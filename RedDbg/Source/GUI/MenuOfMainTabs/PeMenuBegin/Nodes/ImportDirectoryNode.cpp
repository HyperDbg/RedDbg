#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    extern PEInformation objPEInformation;
    extern PeReader objPeReader;

    static std::vector<bool> Opened;
}

void PETab_::PeInfoOfImportDllWindowRender()
{

}

void PETab_::PeImportDirectoryTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

    if (ImGui::BeginTable("z", 10, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("##Info", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Library Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Imports", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("ApiSet Host", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("OFTs", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Time data stamp", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Forwarder chain", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Name RVA", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("FTs (IAT)", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        //ImGuiListClipper clipper;
        //clipper.Begin(GlobalVarsOfPeTab::objPEInformation.Apis.Dlls.size(), 17);//TODO: DataTransfer
        //while (clipper.Step())
        //{
            for (uint64_t Row =0; Row < GlobalVarsOfPeTab::objPEInformation.Apis.Dlls.size(); ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < 10; ++Column)
                {
                    Window.SelectAllAndColored(10, Dimensionses.U32GrayColor);

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
                            ImGui::SetTooltip("APIs");
                            ImGui::EndTooltip();
                        }
                    }
                    else if (Column == 1)
                    {
                        ImGui::Selectable(GlobalVarsOfPeTab::objPEInformation.Apis.Dlls[Row]->Name.c_str());
                    }
                    else if (Column == 2)
                    {
                        ImGui::Selectable(std::to_string(GlobalVarsOfPeTab::objPEInformation.Apis.Functions[Row]->size()).c_str());
                    }
                    else if (Column == 3)
                    {

                    }
                    else if (Column == 4)
                    {

                    }
                    else if (Column == 5)
                    {

                    }
                    else if (Column == 6)
                    {

                    }
                    else if (Column == 8)
                    {

                    }
                    else if (Column == 9)
                    {

                    }
                    ImGui::PopStyleColor(2);
                }
            }
       // }
        ImGui::EndTable();
    }
}

void PETab_::PeImportDirectoryInternalWindowRender()
{
    GlobalVarsOfPeTab::Opened.resize(GlobalVarsOfPeTab::objPEInformation.Apis.Dlls.size());
    auto Iterator = std::find(GlobalVarsOfPeTab::Opened.begin(), GlobalVarsOfPeTab::Opened.end(), true);
    if (Iterator != GlobalVarsOfPeTab::Opened.end())
    {
        static float PEHorizontalSplitterRelPosY = 0.75f;
        static int StartPosY = 0;

        Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
            Dimensionses.TitleBarHeight,
            Dimensionses.SplitterSize,
            Dimensionses.SplitterIsDisabled,
            PEHorizontalSplitterRelPosY,
            WindowCodes::PeImportDirectoryTwoWindowInternalWindowCode);


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
            WindowCodes::PeImportDirectoryTableNodeCode,
            *this,
            false);

        Window.ChildWindowRender(
            Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.SectionHeader.InfoSectionWindowName.data(),
            WindowGeneralSizes.CenterBottomPosition,
            WindowGeneralSizes.CenterBottomSize,
            WindowCodes::PeInfoOfImportDllWindowCode,
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
            WindowCodes::PeImportDirectoryTableNodeCode,
            *this,
            false);
    }
}