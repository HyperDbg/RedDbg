#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

#include <string>
#include <sstream>

namespace GlobalVarsOfPeTab {
    static std::vector<bool> Nodes{ false };
    static bool Init = false;

    extern PEInformation objPEInformation;
    extern PeReader objPeReader;
}

void PETab_::PeInfoNodesDispatcher(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    auto it = std::find(GlobalVarsOfPeTab::Nodes.begin(), GlobalVarsOfPeTab::Nodes.end(), true);
    if (it != GlobalVarsOfPeTab::Nodes.end())
    {
        auto Code = std::distance(GlobalVarsOfPeTab::Nodes.begin(), it);

        //printf("%llu\n", Code);

        switch (Code)
        {
        case ClickCodesOfTreeNodes_::FileTreeNodeClicked: return PeFileInfoWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::DoSHeaderSelectableClicked: return PeDoSHeaderWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::RichHeaderSelectableClicked: return PeRichHeaderWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::NtHeaderTreeNodeClicked: return PeNtHeaderWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::FileHeaderClicked: return PeFileHeaderWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::OptionalHeaderTreeNodeClicked: return PeOptionalHeaderWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::DataDirectoriesSelectableClicked: return PeDataDirectoriesWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::SectionHeaderSelectableClicked: return PeSectionHeaderWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::ImportDirectorySelectableClicked: return PeImportDirectoryWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::ResourceDirectorySelectableClicked: return PeResourceDirectoryWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::ExceptionDirectorySelectableClicked: return PeExceptionDirectoryWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::DependencyWalkerSelectableClicked: return PeDependencyWalkerWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::ImportAdderSelectableClicked: return PeImportAdderWindowRender(WindowGeneralSizes);
        case ClickCodesOfTreeNodes_::PeStructRebuilderSelectableClicked: return PeStructRebuilderWindowRender(WindowGeneralSizes);
        }
    }
}

void PETab_::PeWindowRender()
{
    if (GlobalVarsOfPeTab::objPEInformation.pImageDOSHeaderOfPe == nullptr || GlobalVarsOfPeTab::objPEInformation.pImageNTHeaderOfPe == nullptr) { return; }
    if (!GlobalVarsOfPeTab::Init) { GlobalVarsOfPeTab::Nodes.resize(Dimensionses.CountOfAllNodes); GlobalVarsOfPeTab::Init = true; }

    static float PEVerticalSpliiterRelPosX = 0.2f;
    static int StartPosX = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        PEVerticalSpliiterRelPosX,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::PETabWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PEHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        PEVerticalSpliiterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PEWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::PETreeNodeWindowCode,
        *this,
        true);

    PeInfoNodesDispatcher(&WindowGeneralSizes);
}

void PETab_::PeTreeNodeRender()
{
    bool Open = false;
    if (ImGui::TreeNodeEx(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(), 
        ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth))
    {
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::FileTreeNodeClicked, &GlobalVarsOfPeTab::Nodes);

        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEDoSHeaderNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::DoSHeaderSelectableClicked, &GlobalVarsOfPeTab::Nodes);

        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PERichHeaderNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::RichHeaderSelectableClicked, &GlobalVarsOfPeTab::Nodes);

        if (ImGui::TreeNodeEx(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PENtHeaderNodeName.data(),
            ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth))
        {
            Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::NtHeaderTreeNodeClicked, &GlobalVarsOfPeTab::Nodes);

            ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileHeaderNodeName.data());
            Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::FileHeaderClicked, &GlobalVarsOfPeTab::Nodes);


            if (ImGui::TreeNodeEx(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEOptionalHeaderNodeName.data(), 
                ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_SpanFullWidth))
            {
                Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::OptionalHeaderTreeNodeClicked, &GlobalVarsOfPeTab::Nodes);

                ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEDataDirectoriesNodeName.data());
                Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::DataDirectoriesSelectableClicked, &GlobalVarsOfPeTab::Nodes);

                ImGui::TreePop();
            }
            Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::OptionalHeaderTreeNodeClicked, &GlobalVarsOfPeTab::Nodes);

            ImGui::TreePop();
        }
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::NtHeaderTreeNodeClicked, &GlobalVarsOfPeTab::Nodes);

        ImGui::Separator();
        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PESectionHeadersNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::SectionHeaderSelectableClicked, &GlobalVarsOfPeTab::Nodes);
        ImGui::Separator();

        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEImportDirectoryNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::ImportDirectorySelectableClicked, &GlobalVarsOfPeTab::Nodes);

        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEResourceDirectoryNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::ResourceDirectorySelectableClicked, &GlobalVarsOfPeTab::Nodes);

        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEExceptionDirectoryNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::ExceptionDirectorySelectableClicked, &GlobalVarsOfPeTab::Nodes);
        ImGui::Separator();

        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEDependencyWalkerNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::DependencyWalkerSelectableClicked, &GlobalVarsOfPeTab::Nodes);

        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEImportAdderNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::ImportAdderSelectableClicked, &GlobalVarsOfPeTab::Nodes);

        ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEStructRebuilderNodeName.data());
        Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::PeStructRebuilderSelectableClicked, &GlobalVarsOfPeTab::Nodes);

        ImGui::TreePop();
    }
    Open = Window.ClickOnTreeNode(Open, ClickCodesOfTreeNodes_::FileTreeNodeClicked, &GlobalVarsOfPeTab::Nodes);
}

void PETab_::PeFileInfoWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEFileInfoNodeCode,
        *this,
        false);
}

void PETab_::PeDoSHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEDoSHeaderNodeCode,
        *this,
        false);
}

void PETab_::CheckBoxOpened(std::vector<bool>* Openeds, bool Opened, const unsigned char Index)
{
    auto CheckedBox = std::find(Openeds->begin(), Openeds->end(), true);
    ptrdiff_t OpendesIndex = std::distance(Openeds->begin(), CheckedBox);
    if (CheckedBox != Openeds->end())
    {
        if (Opened && OpendesIndex != Index)
        {
            (*Openeds)[OpendesIndex] = false;
            (*Openeds)[Index] = Opened;

        }
        else
        {
            (*Openeds)[Index] = Opened;
        }
    }
    else
    {
        (*Openeds)[Index] = Opened;
    }
}

void PETab_::PeRichHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PERichHeaderNodeCode,
        *this,
        false);
}

void PETab_::PeNtHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PENtHeaderNodeCode,
        *this,
        false);
}

void PETab_::PeFileHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEFileHeaderNodeCode,
        *this,
        false);
}

void PETab_::PeOptionalHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEOptionalHeaderNodeCode,
        *this,
        false);
}

void PETab_::PeDataDirectoriesWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEDataDirectoriesNodeCode,
        *this,
        false);
}

void PETab_::PeSectionHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PESectionHeaderNodeCode,
        *this,
        false);
}

void PETab_::PeImportDirectoryWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEImportDirectoryNodeCode,
        *this,
        false);
}

void PETab_::PeResourceDirectoryWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEResourceDirectoryNodeCode,
        *this,
        true);
}

void PETab_::PeExceptionDirectoryWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEExceptionDirectoryNodeCode,
        *this,
        true);
}

void PETab_::PeDependencyWalkerWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEDependencyWalkerNodeCode,
        *this,
        true);
}

void PETab_::PeImportAdderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEImportAdderNodeCode,
        *this,
        true);
}

void PETab_::PeStructRebuilderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes)
{
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PEFileNodeName.data(),
        WindowGeneralSizes->RightTopPosition,
        WindowGeneralSizes->RightTopSize,
        WindowCodes::PEStructRebuilderWalkerNodeCode,
        *this,
        true);
}