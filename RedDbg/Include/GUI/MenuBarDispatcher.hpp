#pragma once
#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

#include "GUI/UI/ImGui/imgui.h"

#include <map>

class MenuBarDispatcher_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;
private:
	void EventsWindowRender();
	void DriverWindowRender();
	void EngineWindowRender();
	void DisassemblyWindowRender();
	void GuiWindowRender();
	void MiscWindowRender();
public:
	static MenuBarDispatcher_* OpenInstance(std::string Id, std::string Title);

	void Display(std::string Id);
};