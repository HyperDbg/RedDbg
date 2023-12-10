#pragma once
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

class CPUTab_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;
private:
	const ImGuiTableFlags DisassemblyColumnFlags = ImGuiTableColumnFlags_WidthFixed;
	const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
public:
	void CPUDisasmAndInfoWindowRender();
	void CPURegisterAndCallConventionWindowRender();
	void StackAndDumpWindowRender();

	//StackAndDumpWindowRender
	void StackTableRender();
	//CPUDisasmAndInfoWindowRender
	void DisassemblyTableRender();
public:
	void CPUWindowRender();
};