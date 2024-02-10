#pragma once
#include "GUI/UI/ImGui/imgui.h"
#include "GUI/MenuOfMainTabs/CallStackMenuBegin/CallStack.hpp"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

#include <vector>

typedef struct _CallStackRefreshThread{
	CallStackParser_* Parse;
	std::shared_ptr<std::atomic<bool>> Active;
} CallStackRefreshThread, * PCallStackRefreshThread;

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

	void GetCallStackInfoSafe(CallStackParser_& Parse, std::shared_ptr<std::atomic<bool>> Active);
public:
	void ComboBoxOfTypesInFindInCallStackWindowRender();
	void InputFieldInFindInCallStackWindowRender();
	void FindInCallStackWindowRender();

	void CallStackWindowRender();
	void CallStackTableRender();
};