#pragma once
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"
#include "GUI/MenuOfMainTabs/MemoryMapMenuBegin/Memory.hpp"

#include <ioapiset.h>

class MemoryMapTab_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;
	MemoryParser Parse;
private:
	const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
public:
	void ComboBoxOfTypesInFindInMemoryMapWindowRender();
	void InputFieldInFindInMemoryMapWindowRender();
	void FindInMemoryMapWindowRender();

	void MemoryMapWindowRender();
	void MemoryMapTableRender();
};