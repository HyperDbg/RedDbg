#pragma once
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/Threads.hpp"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"
#include <optional>

typedef struct _ThreadRefreshThread {
	ThreadParser_* Parse;
	std::shared_ptr<std::atomic<bool>> Active;
} ThreadRefreshThread, * PThreadRefreshThread;

class ThreadsTab_
{
private:
	inline const static Dimensionses_ Dimensionses{};
	inline const static Names_ Names{};
	inline static Window_ Window{};
	inline static Window_::ClickMenu_ Menu{};
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