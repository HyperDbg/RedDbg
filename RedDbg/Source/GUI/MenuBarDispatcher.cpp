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

        std::vector<std::string> ConfNames{
            "AutomaticallyLoadAfterOpenPEFile",
            "ModulesIsEditable"
        };
    }
    namespace EventTabGlobalVars
    {
        std::vector<bool> EventTabCheckbox(25);
        std::vector<std::string> ConfNames{
            "SystemBreakpoint",
            "EntryBreakpoint",
            "ExitBreakpoint",
            "DebugStrings",
            "UserTlsCallbacks",
            "UserDllEntry",
            "UserDllLoad",
            "UserDllUnload",
            "ThreadEntry",
            "ThreadCreate",
            "ThreadExit",
            "SetThreadNameExceptions",
            "SystemTlsCallback",
            "SystemDllEntry",
            "SystemDllLoad",
            "SyScallTableBreakpoint",
            "IDTTableBreakpoint",
            "LaunchBreakpoint",
            "LoadDriverBreakpoint",
            "ServiceStart",
            "ServiceCreate",
            "ServiceDelete",
            "KernelThreadEntry",
            "KernelThreadCreate",
            "KernelThreadExit"
        };
    }
    namespace EngineTabGlobalVars
    {
        int64_t BreakpointType;
        int64_t CalculationType;


        std::vector<bool> EngineTabCheckbox(6);
        uint64_t TraceCount;
        uint64_t AnimationPerStepInterval;
        std::string szTraceCount;
        std::string szAnimationPerStepInterval;

        std::vector<std::string> ConfNames{
            "UndecoratedSymbolNames",
            "DisableASLR",
            "LogIfThreadHasSwitched",
            "EnableSourceDebugging",
            "SaveDatabaseInProgramDirectory",
            "IgnoreIncosistentBreakpoints",
            "DefaultMaximumTraceCount",
            "AnimationPerStepInterval",
            "DefaultSoftwareBreakpointType",
            "CalculationType"
        };
    }
    namespace DisassemlyTabGlobalVars
    {
        std::vector<bool> DisassemblyTabCheckbox(14);
        std::vector<std::string> ConfNames{
            "ArgumentSpaces",
            "TabBetweenMnemonicAndArguments",
            "HidePointerSizes",
            "OnlyShowFSAndGSSegments",
            "MemorySpaces",
            "UpperCase",
            "AutocommentsOnlyOnCip",
            "DoNotHighlightOperands",
            "HideModuleNameForLocalMemoryAddresses",
            "PermanentHighlightingMode",
            "DisableBranchDisasemblyPreview",
            "Prefix0xForHexVals",
            "DoNotShowSourceLinesInComments",
            "AssembleInstructionOnDoubleClick"
        };
    }
    namespace GuiTabGlobalVars
    {
        std::vector<bool> GuiTabCheckbox(7);
        std::vector<std::string> ConfNames{
            "ShowFpuRegistersAsLittleEndian",
            "DoNotShowCloseDialog",
            "ShowPidAndTidInHex",
            "GraphZoomMode",
            "ShowRvaAddressesInGraphView",
            "AutoFollowOperandInStack",
            "ShowExitConfirmationDialog"
        };
    }
    namespace MiscTabGlobalVars
    {
        std::vector<bool> MiscTabCheckBox(6);
        std::vector<std::string> ConfNames{
            "",
            "",
            "UseLocalHelpFile",
            "ParseWinApiAndShowArgumentsForThem",
            "QueryProcessCookie", 
            "QueryWorkingSetBeforeReadingMemory"};
        std::string SymbolStore;
        std::string SymbolPath;
    }

    namespace TraceInto
    {
        bool TraceToFunction = false;

        std::string BreakCondition;
        std::string LogText;
        std::string LogCondition;
        std::string CommandText;
        std::string CommandCondition;
        std::string ToFunction;
        std::string MaximumTraceCount;
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

void MenuBarDispatcher_::ParseDriverOptions(toml::table& Tbl)
{
    MenuBarGlobalVars::DriverTabGlobalVars::DriverPath = (std::string)*Tbl["DriverPath"].as_string();

    MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.append(" ");

    for (int StringIndex = 0; StringIndex < MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox.size(); ++StringIndex)
    {
        MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[StringIndex] =
            (bool)*Tbl[MenuBarGlobalVars::DriverTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
    }
}

void MenuBarDispatcher_::DriverWindowRender(toml::table& Tbl, std::string& Path)
{
    ParseDriverOptions(Tbl);

    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DriverTab.Names.size(); ++StringIndex)
    {
        const bool& Opened_ = MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[StringIndex];
        ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DriverTab.Names[StringIndex].data(), (bool*)&Opened_);
        MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[StringIndex] = Opened_;

        if (*Tbl[MenuBarGlobalVars::DriverTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
            != MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[StringIndex])
        {
            *Tbl[MenuBarGlobalVars::DriverTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
                = MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[StringIndex];

            std::ofstream ofs(Path);
            ofs << Tbl;
            ofs.close();
        }
    }

    if (ImGui::InputText(
        Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DriverTab.DriverPath.data(),
        MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.data(),
        MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &MenuBarGlobalVars::DriverTabGlobalVars::DriverPath))
    {
        std::size_t pos = MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.find('\0');
        if (pos != std::string::npos) {
            std::string Temp = MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.substr(0, pos);
            *Tbl["DriverPath"].as_string() = Temp;

            std::ofstream ofs(Path);
            ofs << Tbl;
            ofs.close();
        }
    }
}

void MenuBarDispatcher_::ParseEventOptions(toml::table& Tbl)
{
    for (int StringIndex = 0; StringIndex < MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox.size(); ++StringIndex)
    {
        MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[StringIndex] =
            (bool)*Tbl[MenuBarGlobalVars::EventTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
    }
}

void MenuBarDispatcher_::EventsWindowRender(toml::table& Tbl, std::string& Path)
{
    ParseEventOptions(Tbl);

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

        if (*Tbl[MenuBarGlobalVars::EventTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
            != MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[StringIndex])
        {
            *Tbl[MenuBarGlobalVars::EventTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
                = MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[StringIndex];

            std::ofstream ofs(Path);
            ofs << Tbl;
            ofs.close();
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

void MenuBarDispatcher_::ParseEngineOptions(toml::table& Tbl)
{
    std::ostringstream ss;
    std::ostringstream ss1;

    MenuBarGlobalVars::EngineTabGlobalVars::TraceCount = (uint64_t)(int64_t)*Tbl["DefaultMaximumTraceCount"].as_integer();
    ss << std::uppercase << std::hex << MenuBarGlobalVars::EngineTabGlobalVars::TraceCount;
    MenuBarGlobalVars::EngineTabGlobalVars::szTraceCount = ss.str();

    MenuBarGlobalVars::EngineTabGlobalVars::AnimationPerStepInterval = (uint64_t)(int64_t)*Tbl["AnimationPerStepInterval"].as_integer();
    ss1 << std::uppercase << std::hex << MenuBarGlobalVars::EngineTabGlobalVars::AnimationPerStepInterval;
    MenuBarGlobalVars::EngineTabGlobalVars::szAnimationPerStepInterval = ss1.str();

    MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType = (int64_t)*Tbl["DefaultSoftwareBreakpointType"].as_integer();
    MenuBarGlobalVars::EngineTabGlobalVars::CalculationType = (int64_t)*Tbl["CalculationType"].as_integer();


    MenuBarGlobalVars::EngineTabGlobalVars::szTraceCount.append(" ");
    MenuBarGlobalVars::EngineTabGlobalVars::szAnimationPerStepInterval.append(" ");

    for (int StringIndex = 0; StringIndex < MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox.size(); ++StringIndex)
    {
        MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[StringIndex] =
            (bool)*Tbl[MenuBarGlobalVars::EventTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
    }
}

void MenuBarDispatcher_::EngineWindowRender(toml::table& Tbl, std::string& Path)
{
    ParseEngineOptions(Tbl);

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

            if (*Tbl[MenuBarGlobalVars::EngineTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
                != MenuBarGlobalVars::EngineTabGlobalVars::EngineTabCheckbox[StringIndex])
            {
                *Tbl[MenuBarGlobalVars::EngineTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
                    = MenuBarGlobalVars::EngineTabGlobalVars::EngineTabCheckbox[StringIndex];

                std::ofstream ofs(Path);
                ofs << Tbl;
                ofs.close();
            }
        }
        else if (StringIndex < 8)
        {
            ImGui::PushItemWidth(150);
            if (ImGui::InputText(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(),
                MenuBarGlobalVars::EngineTabGlobalVars::szTraceCount.data(),
                MenuBarGlobalVars::EngineTabGlobalVars::szTraceCount.size(),
                ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::EngineTabGlobalVars::TraceCount))
            {
                std::size_t pos = MenuBarGlobalVars::EngineTabGlobalVars::szTraceCount.find('\0');
                if (pos != std::string::npos) {
                    std::string Temp = MenuBarGlobalVars::EngineTabGlobalVars::szTraceCount.substr(0, pos);
                    *Tbl["DefaultMaximumTraceCount"].as_integer() = std::stoll(Temp, nullptr, 16);

                    std::ofstream ofs(Path);
                    ofs << Tbl;
                    ofs.close();
                }
            }

            ++StringIndex;
            if (ImGui::InputText(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(),
                MenuBarGlobalVars::EngineTabGlobalVars::szAnimationPerStepInterval.data(),
                MenuBarGlobalVars::EngineTabGlobalVars::szAnimationPerStepInterval.size(),
                ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::EngineTabGlobalVars::AnimationPerStepInterval))
            {
                std::size_t pos = MenuBarGlobalVars::EngineTabGlobalVars::szAnimationPerStepInterval.find('\0');
                if (pos != std::string::npos) {
                    std::string Temp = MenuBarGlobalVars::EngineTabGlobalVars::szAnimationPerStepInterval.substr(0, pos);
                    *Tbl["AnimationPerStepInterval"].as_integer() = std::stoll(Temp, nullptr, 16);

                    std::ofstream ofs(Path);
                    ofs << Tbl;
                    ofs.close();
                }
            }
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

            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
            ImGui::PushItemWidth(100);
            if (ImGui::BeginCombo(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(), DSBT[MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType].data(), ImGuiComboFlags_NoArrowButton))
            {
                for (int n = 0; n < DSBT.size(); n++)
                {
                    const bool is_selected = (MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType == n);
                    if (ImGui::Selectable(DSBT[n].data(), is_selected))
                    {
                        MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType = n;
                        if (*Tbl["DefaultSoftwareBreakpointType"].as_integer()
                            != MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType)
                        {
                            *Tbl["DefaultSoftwareBreakpointType"].as_integer()
                                = MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType;

                            std::ofstream ofs(Path);
                            ofs << Tbl;
                            ofs.close();
                        }
                    }

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            ++StringIndex;

            if (ImGui::BeginCombo(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names[StringIndex].data(), CT[MenuBarGlobalVars::EngineTabGlobalVars::CalculationType].data(), ImGuiComboFlags_NoArrowButton))
            {
                for (int n = 0; n < CT.size(); n++)
                {
                    const bool is_selected = (MenuBarGlobalVars::EngineTabGlobalVars::CalculationType == n);
                    if (ImGui::Selectable(CT[n].data(), is_selected))
                    {
                        MenuBarGlobalVars::EngineTabGlobalVars::CalculationType = n;
                        if (*Tbl["CalculationType"].as_integer()
                            != MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType)
                        {
                            *Tbl["CalculationType"].as_integer()
                                = MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType;

                            std::ofstream ofs(Path);
                            ofs << Tbl;
                            ofs.close();
                        }
                    }

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

void MenuBarDispatcher_::ParseDisassemblyOptions(toml::table& Tbl)
{
    for (int StringIndex = 0; StringIndex < MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox.size(); ++StringIndex)
    {
        MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox[StringIndex] =
            (bool)*Tbl[MenuBarGlobalVars::DisassemlyTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
    }
}

void MenuBarDispatcher_::DisassemblyWindowRender(toml::table& Tbl, std::string& Path)
{
    ParseDisassemblyOptions(Tbl);

    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DisaasemblyTab.Names.size(); ++StringIndex)
    {
        const bool& Opened_ = MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox[StringIndex];
        ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DisaasemblyTab.Names[StringIndex].data(), (bool*)&Opened_);
        MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox[StringIndex] = Opened_;
        if (*Tbl[MenuBarGlobalVars::DisassemlyTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
            != MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox[StringIndex])
        {
            *Tbl[MenuBarGlobalVars::DisassemlyTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
                = MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox[StringIndex];

            std::ofstream ofs(Path);
            ofs << Tbl;
            ofs.close();
        }
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

void MenuBarDispatcher_::ParseGuiOptions(toml::table& Tbl)
{
    for (int StringIndex = 0; StringIndex < MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox.size(); ++StringIndex)
    {
        MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox[StringIndex] =
            (bool)*Tbl[MenuBarGlobalVars::GuiTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
    }
}

void MenuBarDispatcher_::GuiWindowRender(toml::table& Tbl, std::string& Path)
{
    ParseGuiOptions(Tbl);

    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.GuiTab.Names.size(); ++StringIndex)
    {
        const bool& Opened_ = MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox[StringIndex];
        ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.GuiTab.Names[StringIndex].data(), (bool*)&Opened_);
        MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox[StringIndex] = Opened_;
        if (*Tbl[MenuBarGlobalVars::GuiTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
            != MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox[StringIndex])
        {
            *Tbl[MenuBarGlobalVars::GuiTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
                = MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox[StringIndex];

            std::ofstream ofs(Path);
            ofs << Tbl;
            ofs.close();
        }
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

void MenuBarDispatcher_::ParseMiscOptions(toml::table& Tbl)
{
    //static bool Init = false;
    MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore = (std::string)*Tbl["SymbolStore"].as_string();
    MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath = (std::string)*Tbl["SymbolPath"].as_string();

    MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath.append(" ");
    MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore.append(" ");
    //if (!Init)
    //{
    //    MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath.append(" ");
    //    MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore.append(" ");
    //    Init = true;
    //}

    for (int StringIndex = 0; StringIndex < MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox.size(); ++StringIndex)
    {
        if (MenuBarGlobalVars::MiscTabGlobalVars::ConfNames[StringIndex] != "")
        {
            MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox[StringIndex] =
                (bool)*Tbl[MenuBarGlobalVars::MiscTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
        }
    }
}

void MenuBarDispatcher_::MiscWindowRender(toml::table& Tbl, std::string& Path)
{
    ParseMiscOptions(Tbl);

    for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names.size(); ++StringIndex)
    {
        if (StringIndex < 2)
        {
            if (ImGui::InputText(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names[StringIndex].data(),
                MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore.data(),
                MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore))
            {
                std::size_t pos = MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore.find('\0');
                if (pos != std::string::npos) {
                    std::string Temp = MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore.substr(0, pos);
                    *Tbl["SymbolStore"].as_string() = Temp;

                    std::ofstream ofs(Path);
                    ofs << Tbl;
                    ofs.close();
                }
            }

            ++StringIndex;

            if (ImGui::InputText(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names[StringIndex].data(),
                MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath.data(),
                MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath))
            {
                std::size_t pos = MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath.find('\0');
                if (pos != std::string::npos) {
                    std::string Temp = MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath.substr(0, pos);
                    *Tbl["SymbolPath"].as_string() = Temp;

                    std::ofstream ofs(Path);
                    ofs << Tbl;
                    ofs.close();
                }
            }
        }
        else
        {
            const bool& Opened_ = MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox[StringIndex];
            ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names[StringIndex].data(), (bool*)&Opened_);
            MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox[StringIndex] = Opened_;
            if (*Tbl[MenuBarGlobalVars::MiscTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
                != MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox[StringIndex])
            {
                *Tbl[MenuBarGlobalVars::MiscTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean()
                    = MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox[StringIndex];

                std::ofstream ofs(Path);
                ofs << Tbl;
                ofs.close();
            }

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

enum Ids
{
    OptionsId = 0x2274AD5D845C5920,
    IntoTraceID = 0x300137B0711CC9f5,
    OverTraceID = 0x76A5F809B5D5C441,
    LBRTraceID = 0xFBE56B3ADB0F3AED,
};

bool was = false;
void MenuBarDispatcher_::Display(std::string Id)
{
    std::size_t str_hash = std::hash<std::string>{}(Id);
    static bool opened = true;
    if (!opened)
    {
        MenuBarGlobalVars::MapOfDialogs.clear();
        MenuBarGlobalVars::Titles.clear();
    }

    auto Iter = MenuBarGlobalVars::MapOfDialogs.find(str_hash);
    if (Iter != MenuBarGlobalVars::MapOfDialogs.end() && Iter->first == Ids::OptionsId)
    {
        ptrdiff_t Index = std::distance(MenuBarGlobalVars::MapOfDialogs.begin(), Iter);
        ImGui::OpenPopup(MenuBarGlobalVars::Titles[Index].c_str());
        if (ImGui::BeginPopupModal(MenuBarGlobalVars::Titles[Index].c_str(), &opened, ImGuiWindowFlags_NoCollapse))
        {
            static std::string Path;
            if (!was)
            {
                std::string TempBuff; TempBuff.resize(MAX_PATH);
                GetModuleFileNameA(NULL, TempBuff.data(), MAX_PATH);
                std::filesystem::path FullPathExeName = TempBuff;

                Path = FullPathExeName.parent_path().string();
                Path.append("\\RedDbg.toml");
                was = true;
            }

            static toml::table Tbl = toml::parse_file(Path);

            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });
            ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });

            ImGui::SetCursorPosY(20);
            if (ImGui::BeginTabBar(
                Names.Windowses.MainDebuggerInterface.MainTabBarName.data(),
                ImGuiTabBarFlags_NoTooltip | ImGuiTabBarFlags_Reorderable | ImGuiTabBarFlags_TabListPopupButton))
            {
                if (ImGui::BeginTabItem("Driver"))
                {
                    DriverWindowRender(Tbl, Path);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Events"))
                {
                    EventsWindowRender(Tbl, Path);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Engine"))
                {
                    EngineWindowRender(Tbl, Path);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Disassembly"))
                {
                    DisassemblyWindowRender(Tbl, Path);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Gui"))
                {
                    GuiWindowRender(Tbl, Path);
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Misc"))
                {
                    MiscWindowRender(Tbl, Path);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::PopStyleColor(2);

            ImGui::EndPopup();
        }
    }
    else if (Iter != MenuBarGlobalVars::MapOfDialogs.end() && Iter->first == Ids::IntoTraceID ||
        Iter != MenuBarGlobalVars::MapOfDialogs.end() && Iter->first == Ids::OverTraceID ||
        Iter != MenuBarGlobalVars::MapOfDialogs.end() && Iter->first == Ids::LBRTraceID)
    {
        ptrdiff_t Index = std::distance(MenuBarGlobalVars::MapOfDialogs.begin(), Iter);
        ImGui::OpenPopup(MenuBarGlobalVars::Titles[Index].c_str());
        if (ImGui::BeginPopupModal(MenuBarGlobalVars::Titles[Index].c_str(), &opened, ImGuiWindowFlags_NoCollapse))
        {
            ImGui::InputTextWithHint(
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.BreakCondition.data(),
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.HintConditions.data(),
                MenuBarGlobalVars::TraceInto::BreakCondition.data(),
                MenuBarGlobalVars::TraceInto::BreakCondition.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::TraceInto::BreakCondition);

            ImGui::InputTextWithHint(
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.LogText.data(),
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.HintLogText.data(),
                MenuBarGlobalVars::TraceInto::LogText.data(),
                MenuBarGlobalVars::TraceInto::LogText.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::TraceInto::LogText);

            ImGui::InputTextWithHint(
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.LogCondition.data(),
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.HintConditions.data(),
                MenuBarGlobalVars::TraceInto::LogCondition.data(),
                MenuBarGlobalVars::TraceInto::LogCondition.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::TraceInto::LogCondition);

            ImGui::InputTextWithHint(
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.CommandText.data(),
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.HintCommandText.data(),
                MenuBarGlobalVars::TraceInto::CommandText.data(),
                MenuBarGlobalVars::TraceInto::CommandText.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::TraceInto::CommandText);

            ImGui::InputTextWithHint(
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.CommandCondition.data(),
                Names.Windowses.TitleBarMenu.MenuTracing.TracingText.HintConditions.data(),
                MenuBarGlobalVars::TraceInto::CommandCondition.data(),
                MenuBarGlobalVars::TraceInto::CommandCondition.size(),
                ImGuiInputTextFlags_CallbackResize,
                reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                &MenuBarGlobalVars::TraceInto::CommandCondition);

            if (MenuBarGlobalVars::TraceInto::TraceToFunction)
            {
                ImGui::InputTextWithHint(
                    Names.Windowses.TitleBarMenu.MenuTracing.TracingText.ToFunction.data(),
                    Names.Windowses.TitleBarMenu.MenuTracing.TracingText.HintToFunction.data(),
                    MenuBarGlobalVars::TraceInto::ToFunction.data(),
                    MenuBarGlobalVars::TraceInto::ToFunction.size(),
                    ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal,
                    reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                    &MenuBarGlobalVars::TraceInto::ToFunction);
            }
            else
            {
                ImGui::InputTextWithHint(
                    Names.Windowses.TitleBarMenu.MenuTracing.TracingText.MaximumTraceCount.data(),
                    Names.Windowses.TitleBarMenu.MenuTracing.TracingText.HintTraceCount.data(),
                    MenuBarGlobalVars::TraceInto::MaximumTraceCount.data(),
                    MenuBarGlobalVars::TraceInto::MaximumTraceCount.size(),
                    ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal,
                    reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
                    &MenuBarGlobalVars::TraceInto::MaximumTraceCount);
            }
            const bool& Opened_ = MenuBarGlobalVars::TraceInto::TraceToFunction;
            ImGui::Checkbox(Names.Windowses.TitleBarMenu.MenuTracing.TracingText.ToFunctionCheckBox.data(), (bool*)&Opened_);
            MenuBarGlobalVars::TraceInto::TraceToFunction = Opened_;

            ImGui::SameLine(ImGui::GetWindowWidth() - 30);

            if (ImGui::Button("OK"))
            {
                switch (Iter->first)
                {
                case Ids::IntoTraceID:
                case Ids::OverTraceID:
                {
                    break;
                }
                case Ids::LBRTraceID:
                {
                    //Tracer BranchTracer;
                    //BranchTracer.StartAddr
                    //svm_vmmcall(VMMCALL_ID::StartBranchTrace, )
                    break;
                }
                }
            }

            ImGui::EndPopup();
        }
    }
    else
    {
        opened = true;
    }
}