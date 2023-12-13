#define IMGUI_DEFINE_MATH_OPERATORS
#include "GUI/MenuBarDispatcher.hpp"

#include <iostream>
#include <imgui_internal.h>

namespace MenuBarGlobalVars
{
    std::map<std::size_t, MenuBarDispatcher_*> MapOfDialogs;
    std::vector<std::string> Titles;
    namespace DriverTabGlobalVars
    {
        std::vector<bool> DriverTabCheckbox(2);
        std::string DriverPath;
    }
    namespace EventTabGlobalVars
    {
        std::vector<bool> EventTabCheckbox(24);
    }
    namespace EngineTabGlobalVars
    {
        std::vector<bool> EngineTabCheckbox(6);
        std::string TraceCount = "50000 ";
        std::string AnimationPerStepInterval = "20 ";
    }
    namespace DisassemlyTabGlobalVars
    {
        std::vector<bool> DisassemblyTabCheckbox(14);
    }
    namespace GuiTabGlobalVars
    {
        std::vector<bool> GuiTabCheckbox(7);
    }
    namespace MiscTabGlobalVars
    {
        std::vector<bool> MiscTabCheckBox(6);
        std::string SymbolStore = "https://msdl.microsoft.com/download/symbols ";
        std::string SymbolPath = ".\\symbols ";
    }
}

MenuBarDispatcher_* MenuBarDispatcher_::OpenInstance(std::string Id, std::string Title)
{
    std::size_t str_hash = std::hash<std::string>{}(Id);
    auto Iter = MenuBarGlobalVars::MapOfDialogs.find(str_hash);
    MenuBarDispatcher_* Instance = new MenuBarDispatcher_();
    if (Iter == MenuBarGlobalVars::MapOfDialogs.end())
    {
        MenuBarGlobalVars::MapOfDialogs.insert(std::make_pair(str_hash, Instance));
        MenuBarGlobalVars::Titles.push_back(Title);
    }
    return Instance;
}

void MenuBarDispatcher_::DriverWindowRender()
{
    ImGui::InputText(
        Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DriverTab.DriverPath.data(),
        MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.data(),
        MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &MenuBarGlobalVars::DriverTabGlobalVars::DriverPath);

    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DriverTab.Names.size(); ++StringIndex)
    {
        const bool& Opened_ = MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[StringIndex];
        ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DriverTab.Names[StringIndex].data(), (bool*)&Opened_);
        MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[StringIndex] = Opened_;
    }
}

void MenuBarDispatcher_::EventsWindowRender()
{
    ImGui::Text(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EventsTab.BreakOn.data());
    ImGui::SeparatorText(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EventsTab.User.data());
    
    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EventsTab.Names.size(); ++StringIndex )
    {
        const bool& Opened_ = MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[StringIndex];
        ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EventsTab.Names[StringIndex].data(), (bool*)&Opened_);
        MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[StringIndex] = Opened_;

        if (StringIndex == 14)
        {
            ImGui::SeparatorText(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EventsTab.Kernel.data());
        }
    }
    //Checkboxes
    //
    //"Break on: " Text
    //Text separator "User level"
    //"System Breakpoint"
    //"Entry Breakpoint"
    //"Exit Breakpoint"
    //"Debug strings"
    //"User TLS Callbacks"
    //"User DLL Entry"
    //"User DLL Load"
    //"User DLL Unload"
    //The same line, that all lines above
    //"Thread entry"
    //"Thread create"
    //"Thread exit"
    //"SetThreadName exceptions"
    //"System TLS CallBack"
    //"System DLL Entry"
    //"System DLL Load"
    //"System DLL Unload"
    // 
    //Text separator "Kernel level"
    //"Syscall table breakpoint"
    //"IDT Table breakpoint"
    //"Launch"
    //"Load driver"
    //"Service start"
    //"Service create"
    //"Thread entry"
    //"Thread create"
    //"Thread exit"
}

void MenuBarDispatcher_::EngineWindowRender()
{
    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names.size(); ++StringIndex)
    {
        if (StringIndex < 6)
        {
            const bool& Opened_ = MenuBarGlobalVars::EngineTabGlobalVars::EngineTabCheckbox[StringIndex];
            ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(), (bool*)&Opened_);
            MenuBarGlobalVars::EngineTabGlobalVars::EngineTabCheckbox[StringIndex] = Opened_;
            if (StringIndex == 1 && ImGui::IsItemHovered() && ImGui::BeginTooltip())
            {
                ImGui::Text(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Ring3App.data());
                ImGui::EndTooltip();
            }
        }
        else if (StringIndex < 8)
        {
            ImGui::PushItemWidth(150);
            ImGui::InputText(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(),
                MenuBarGlobalVars::EngineTabGlobalVars::TraceCount.data(),
                MenuBarGlobalVars::EngineTabGlobalVars::TraceCount.size(),
                ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::EngineTabGlobalVars::TraceCount);

            ++StringIndex;
            ImGui::InputText(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(),
                MenuBarGlobalVars::EngineTabGlobalVars::AnimationPerStepInterval.data(),
                MenuBarGlobalVars::EngineTabGlobalVars::AnimationPerStepInterval.size(),
                ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::EngineTabGlobalVars::AnimationPerStepInterval);
            ImGui::PopItemWidth();
        }
        else
        {
            const std::vector<std::string_view> DSBT
            {
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.DSBTypeList.Int3,
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.DSBTypeList.LongInt3,
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.DSBTypeList.UD2,
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.DSBTypeList.CLI,
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.DSBTypeList.JMP,
            };

            const std::vector<std::string_view> CT
            {
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.CalculationType.Signed,
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.CalculationType.Unsigned,
            };

            static int CurrentTypeId1 = 0;

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
            ImGui::PushItemWidth(100);
            if (ImGui::BeginCombo(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(), DSBT[CurrentTypeId1].data(), ImGuiComboFlags_NoArrowButton))
            {
                for (int n = 0; n < DSBT.size(); n++)
                {
                    const bool is_selected = (CurrentTypeId1 == n);
                    if (ImGui::Selectable(DSBT[n].data(), is_selected))
                        CurrentTypeId1 = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            //ImGui::PopStyleColor();

            ++StringIndex;

            static int CurrentTypeId2 = 0;

            //ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
            if (ImGui::BeginCombo(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(), CT[CurrentTypeId2].data(), ImGuiComboFlags_NoArrowButton))
            {
                for (int n = 0; n < CT.size(); n++)
                {
                    const bool is_selected = (CurrentTypeId2 == n);
                    if (ImGui::Selectable(CT[n].data(), is_selected))
                        CurrentTypeId2 = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::PopStyleColor();
            ImGui::PopItemWidth();
        }
    }
    //Checkboxes
    //
    //"Undecorated Symbol Names"
    //"Disable ASLR"//On ring3 app
    //"Log if thread has switched"
    //"Enable source debugging"
    //"Save Database in Program Directory"
    //"Ignore incosistent breakpoints"
    //Changeable fields
    // 
    //"Default Maximum trace count"
    //"Animation per-step interval (ms)"
    //Lists
    // 
    //"Default Software Breakpoint Type"
    //"Calculation Type"
}

void MenuBarDispatcher_::DisassemblyWindowRender()
{
    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DisaasemblyTab.Names.size(); ++StringIndex)
    {
        const bool& Opened_ = MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox[StringIndex];
        ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DisaasemblyTab.Names[StringIndex].data(), (bool*)&Opened_);
        MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox[StringIndex] = Opened_;
    }
    //Checkboxes
    //
    //"Argument Spaces"
    //"Tab between mnemonic and arguments"
    //"Hide pointer sizes"
    //"Only show FS/GS segments"
    //"Memory spaces"
    //"Uppercase"
    //"Autocomments only on CIP"
    //"Don't highlight operands"
    //"Hide module name for local memory addresses"
    //"Permanent highlighting mode"
    //"Disable branch disassembly preview"
    //"0x prefix for hex values"
    //"Don't show source lines in comments"
    //"Assemble intruction on double-click"
}

void MenuBarDispatcher_::GuiWindowRender()
{
    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.GuiTab.Names.size(); ++StringIndex)
    {
        const bool& Opened_ = MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox[StringIndex];
        ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.GuiTab.Names[StringIndex].data(), (bool*)&Opened_);
        MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox[StringIndex] = Opened_;
    }
    //Checkboxes
    //
    //"Show FPU registers as little endian"
    //"Don't show close dialog"
    //"Show PID/TID in HEX"
    //"Graph zoom mode"
    //"Show RVA addresses in graph view"
    //"Auto follow operand in stack"
    //"Show exit confirmation dialog"
}

void MenuBarDispatcher_::MiscWindowRender()
{
    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names.size(); ++StringIndex)
    {
        if (StringIndex < 2)
        {
            ImGui::InputText(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names[StringIndex].data(),
                MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore.data(),
                MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore);

            ++StringIndex;

            ImGui::InputText(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names[StringIndex].data(),
                MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath.data(),
                MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath);
        }
        else
        {
            const bool& Opened_ = MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox[StringIndex];
            ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names[StringIndex].data(), (bool*)&Opened_);
            MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox[StringIndex] = Opened_;
        }
    }
    //Edit fields
    //
    //"Symbol store"
    //"Symbol path"
    //Checkboxes
    //
    //"Use local help file"//Reddbg.chm || HyperDbg.chm
    //"Parse winapi and show arguments for them"
    //"Query process cookie"
    //"Query working set before reading memory"
}
bool was = false;
void MenuBarDispatcher_::Display(std::string Id)
{
    std::size_t str_hash = std::hash<std::string>{}(Id);
    static bool opened = true;
    //bool lastopened = opened;
    if (!opened)
    {
        MenuBarGlobalVars::MapOfDialogs.clear();
    //    was = false;
    }

    auto Iter = MenuBarGlobalVars::MapOfDialogs.find(str_hash);
    if (Iter != MenuBarGlobalVars::MapOfDialogs.end() && Iter->first == 0x2274AD5D845C5920)//"OptionsId"
    {
        ptrdiff_t Index = std::distance(MenuBarGlobalVars::MapOfDialogs.begin(), Iter);
        //MenuBarGlobalVars::MapOfDialogs.clear();
        ImGui::OpenPopup(MenuBarGlobalVars::Titles[Index].c_str());
        if (ImGui::BeginPopupModal(MenuBarGlobalVars::Titles[Index].c_str(), &opened, ImGuiWindowFlags_NoCollapse))
        {
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });
            ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });

            ImGui::SetCursorPosY(20);
            if (ImGui::BeginTabBar(
                Names.Windowses.MainDebuggerInterface.MainTabBarName.data(),
                ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton))
            {
                if (ImGui::BeginTabItem("Driver"))
                {
                    DriverWindowRender();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Events"))
                {
                    EventsWindowRender();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Engine"))
                {
                    EngineWindowRender();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Disassembly"))
                {
                    DisassemblyWindowRender();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Gui"))
                {
                    GuiWindowRender();
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Misc"))
                {
                    MiscWindowRender();
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::PopStyleColor(2);

            //if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); MenuBarGlobalVars::MapOfDialogs.clear(); }

            ImGui::EndPopup();
        }
        //else
        //{
        //    was = true;
        //}
    }
    else
    {
        opened = true;
    }
}