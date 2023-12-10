#pragma once
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

class CallStackTab_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;
private:
	const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
public:
	void ComboBoxOfTypesInFindInCallStackWindowRender();
	void InputFieldInFindInCallStackWindowRender();
	void FindInCallStackWindowRender();

	void CallStackWindowRender();
	void CallStackTableRender();
};