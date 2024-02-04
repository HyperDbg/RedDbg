#include "Debugger/TomlParser.hpp"

namespace MenuBarGlobalVars
{
    namespace DriverTabGlobalVars
    {
        extern std::vector<bool> DriverTabCheckbox;
        extern std::string DriverPath;

        extern std::vector<std::string> ConfNames;
    }
    namespace EventTabGlobalVars
    {
        extern std::vector<bool> EventTabCheckbox;
        extern std::vector<std::string> ConfNames;
    }
    namespace EngineTabGlobalVars
    {
        extern int64_t BreakpointType;
        extern int64_t CalculationType;


        extern std::vector<bool> EngineTabCheckbox;
        extern uint64_t TraceCount;
        extern uint64_t AnimationPerStepInterval;
        extern std::string szTraceCount;
        extern std::string szAnimationPerStepInterval;

        extern std::vector<std::string> ConfNames;
    }
    namespace DisassemlyTabGlobalVars
    {
        extern std::vector<bool> DisassemblyTabCheckbox;
        extern std::vector<std::string> ConfNames;
    }
    namespace GuiTabGlobalVars
    {
        extern std::vector<bool> GuiTabCheckbox;
        extern std::vector<std::string> ConfNames;
    }
    namespace MiscTabGlobalVars
    {
        extern std::vector<bool> MiscTabCheckBox;
        extern std::vector<std::string> ConfNames;
        extern std::string SymbolStore;
        extern std::string SymbolPath;
    }
}

TomlParser_::TomlParser_()
{
    static std::string Path;
    static bool Init = false;
    if (!Init)
    {
        std::string TempBuff; TempBuff.resize(MAX_PATH);
        GetModuleFileNameA(NULL, TempBuff.data(), MAX_PATH);
        std::filesystem::path FullPathExeName = TempBuff;

        Path = FullPathExeName.parent_path().string();
        Path.append("\\RedDbg.toml");
        Init = true;
    }

    static toml::table Tbl = toml::parse_file(Path);

    {
        for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DriverTab.Names.size(); ++StringIndex)
        {
            MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[StringIndex] =
                (bool)*Tbl[MenuBarGlobalVars::DriverTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
        }

        MenuBarGlobalVars::DriverTabGlobalVars::DriverPath = (std::string)*Tbl["DriverPath"].as_string();
    }

    {
        for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EventsTab.Names.size(); ++StringIndex)
        {
            MenuBarGlobalVars::EventTabGlobalVars::EventTabCheckbox[StringIndex] =
                (bool)*Tbl[MenuBarGlobalVars::EventTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
        }
    }

    {
        for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.EngineTab.Names.size(); ++StringIndex)
        {
            if (StringIndex < 6)
            {
                MenuBarGlobalVars::EngineTabGlobalVars::EngineTabCheckbox[StringIndex] = (bool)*Tbl[MenuBarGlobalVars::EngineTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
            }
            else if (StringIndex < 8)
            {
                std::ostringstream ss;
                std::ostringstream ss1;

                MenuBarGlobalVars::EngineTabGlobalVars::TraceCount = (uint64_t)(int64_t)*Tbl["DefaultMaximumTraceCount"].as_integer();
                ss << std::uppercase << std::hex << MenuBarGlobalVars::EngineTabGlobalVars::TraceCount;
                MenuBarGlobalVars::EngineTabGlobalVars::szTraceCount = ss.str();

                ++StringIndex;

                MenuBarGlobalVars::EngineTabGlobalVars::AnimationPerStepInterval = (uint64_t)(int64_t)*Tbl["AnimationPerStepInterval"].as_integer();
                ss1 << std::uppercase << std::hex << MenuBarGlobalVars::EngineTabGlobalVars::AnimationPerStepInterval;
                MenuBarGlobalVars::EngineTabGlobalVars::szAnimationPerStepInterval = ss1.str();
            }
            else
            {
                MenuBarGlobalVars::EngineTabGlobalVars::BreakpointType = (int64_t)*Tbl["DefaultSoftwareBreakpointType"].as_integer();

                ++StringIndex;

                MenuBarGlobalVars::EngineTabGlobalVars::CalculationType = (int64_t)*Tbl["CalculationType"].as_integer();
            }
        }
    }

    {
        for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.DisaasemblyTab.Names.size(); ++StringIndex)
        {
            MenuBarGlobalVars::DisassemlyTabGlobalVars::DisassemblyTabCheckbox[StringIndex] =
                (bool)*Tbl[MenuBarGlobalVars::DisassemlyTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
        }
    }

    {
        for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.GuiTab.Names.size(); ++StringIndex)
        {
            MenuBarGlobalVars::GuiTabGlobalVars::GuiTabCheckbox[StringIndex] =
                (bool)*Tbl[MenuBarGlobalVars::GuiTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
        }
    }

    {
        for (int StringIndex = 0; StringIndex < Names.Windowses.TitleBarMenu.MenuOptions.Preferences.MiscTab.Names.size(); ++StringIndex)
        {
            if (MenuBarGlobalVars::MiscTabGlobalVars::ConfNames[StringIndex] != "")
            {
                MenuBarGlobalVars::MiscTabGlobalVars::MiscTabCheckBox[StringIndex] =
                    (bool)*Tbl[MenuBarGlobalVars::MiscTabGlobalVars::ConfNames[StringIndex].c_str()].as_boolean();
            }
        }

        MenuBarGlobalVars::MiscTabGlobalVars::SymbolStore = (std::string)*Tbl["SymbolStore"].as_string();
        MenuBarGlobalVars::MiscTabGlobalVars::SymbolPath = (std::string)*Tbl["SymbolPath"].as_string();
    }
}