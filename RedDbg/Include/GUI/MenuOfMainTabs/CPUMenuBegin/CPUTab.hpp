#pragma once
#include "GUI/MenuOfMainTabs/CPUMenuBegin/CPU.hpp"
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

#include <memory>

class CPUTab_
{
private:
	inline static Dimensionses_ Dimensionses;
	inline static Names_ Names;
	inline static GlobalClassVars_ GlobalClassVars;
	Window_ Window;
private:
	const ImGuiTableFlags DisassemblyColumnFlags = ImGuiTableColumnFlags_WidthFixed;
	const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	void GetCPUInfoSafe(CPUParser_& Parse, std::shared_ptr<std::atomic<bool>> Active);
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