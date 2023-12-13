#include "GUI/MenuOfMainTabs/MenuGeneral.hpp"

#include <vector>

void MenuGeneral_::TabDispatcher(GeneralTab_ TabCode, bool* open)
{
    switch (TabCode)
    {
    case GeneralCPUTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainCPUTabName.data(), open))
        {
            CPUTab.CPUWindowRender();
            ImGui::EndTabItem();
        }

        return;
    }
    case GeneralStackMapTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainStackMapTabName.data(), open))
        {
            StackMapTab.StackMapWindowRender();
            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralNotesTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainNotesTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralThreadsTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainThreadsTabName.data(), open))
        {
            ThreadsTab.ThreadsWindowRender();
            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralCallStackTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainCallStackTabName.data(), open))
        {
            CallStackTab.CallStackWindowRender();
            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralLogTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainLogTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralBreakpointsTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainBreakpointsTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralMemoryMapTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainMemoryMapTabName.data(), open))
        {
            MemoryMapTab.MemoryMapWindowRender();
            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralSEHTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainSEHTabName.data(), open))
        {
            SEHTab.SEHWindowRender();
            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralScriptTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainScriptTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralSymbolsTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainSymbolsTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralSourceTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainSourceTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralHandlesTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainHandlesTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralTraceTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainTraceTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralPETab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainPETabName.data(), open))
        {
            PETab.PeWindowRender();
            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralLiftersTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainLiftersTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralDevirtualizersTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainDevirtualizersTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    case GeneralDeobfuscatorsTab:
    {
        if (ImGui::BeginTabItem(Names.Windowses.MainDebuggerInterface.MainTabs.MainDeobfuscatorsTabName.data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    default:
    {
        if (ImGui::BeginTabItem((Names.Windowses.MainDebuggerInterface.MainTabs.MainNewTabName.data() + std::to_string((unsigned long long)TabCode)).data(), open))
        {

            ImGui::EndTabItem();
        }
        return;
    }
    }
}

void MenuGeneral_::MainInterfaceRender()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImVec2 DebuggerPos{};
    DebuggerPos.x = viewport->WorkPos.x;
    DebuggerPos.y = viewport->WorkPos.y + Dimensionses.TitleBarHeight + 10;//Default Title Bar pos by y

    ImVec2 DebuggerSize{};
    DebuggerSize.x = viewport->WorkSize.x;
    DebuggerSize.y = viewport->WorkSize.y + Dimensionses.TitleBarHeight + 10;//Default Title Bar size by y

    ImGui::SetNextWindowPos(DebuggerPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });
    ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });

    ImGui::Begin(
        Names.Windowses.MainDebuggerInterfaceName.data(),
        nullptr,
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);

    static std::vector<uint64_t> ActiveTabs;
    static uint64_t NextTabId = 0;
    if (NextTabId == 0)
    {
        for (int Tab = 0; Tab < DefaultCountOfMainTabs; Tab++)
        {
            ActiveTabs.push_back(NextTabId++);
        }
    }

    ImGui::SetCursorPos(ImVec2(5, 0));//By left side

    if (ImGui::BeginTabBar(
        Names.Windowses.MainDebuggerInterface.MainTabBarName.data(),
        ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton))
    {
        ImGui::SetCursorPosX(0);

        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) { ActiveTabs.push_back(NextTabId++); }

        for (uint64_t Tab = 0; Tab < ActiveTabs.size();)
        {
            bool open = true;

            TabDispatcher((GeneralTab_)ActiveTabs[Tab], &open);

            if (!open) { ActiveTabs.erase(ActiveTabs.begin() + Tab); }
            else { Tab++; }
        }

        ImGui::EndTabBar();
    }
    ImGui::PopStyleColor(2);
    ImGui::End();
}