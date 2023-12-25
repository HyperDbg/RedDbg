#pragma once
#include "toml++/toml.hpp"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

#include "GUI/UI/ImGui/imgui.h"

#include <map>
#include <filesystem>

class MenuBarDispatcher_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;
private:
	void EventsWindowRender(toml::table& Tbl, std::string& Path);
	void ParseEventOptions(toml::table& Tbl);

	void DriverWindowRender(toml::table& Tbl, std::string& Path);
	void ParseDriverOptions(toml::table& Tbl);

	void EngineWindowRender(toml::table& Tbl, std::string& Path);
	void ParseEngineOptions(toml::table& Tbl);

	void DisassemblyWindowRender(toml::table& Tbl, std::string& Path);
	void ParseDisassemblyOptions(toml::table& Tbl);

	void GuiWindowRender(toml::table& Tbl, std::string& Path);
	void ParseGuiOptions(toml::table& Tbl);

	void MiscWindowRender(toml::table& Tbl, std::string& Path);
	void ParseMiscOptions(toml::table& Tbl);
public:
	static MenuBarDispatcher_* OpenInstance(std::string Id, std::string Title);

	void Display(std::string Id);
};