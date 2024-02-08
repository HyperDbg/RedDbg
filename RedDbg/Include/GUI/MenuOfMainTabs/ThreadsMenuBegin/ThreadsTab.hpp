#pragma once
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/Threads.hpp"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

typedef struct _ThreadRefreshThread {
	ThreadParser_* Parse;
	std::shared_ptr<std::atomic<bool>> Active;
} ThreadRefreshThread, * PThreadRefreshThread;

class ThreadsTab_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;

private:
	const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	void GetThreadInfoSafe(ThreadParser_& Parse, std::shared_ptr<std::atomic<bool>> Active);
public:
	void ComboBoxOfTypesInFindInThreadsWindowRender();
	void InputFieldInFindInThreadsWindowRender();
	void FindInThreadsWindowRender();

	void ThreadsWindowRender();
	void ThreadsTableRender();
};