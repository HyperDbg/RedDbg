#pragma once
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/MenuOfMainTabs/CPUMenuBegin/CPUTab.hpp"
#include "GUI/MenuOfMainTabs/StackMapMenuBegin/StackMapTab.hpp"
#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/ThreadsTab.hpp"
#include "GUI/MenuOfMainTabs/CallStackMenuBegin/CallStackTab.hpp"
#include "GUI/MenuOfMainTabs/MemoryMapMenuBegin/MemoryMapTab.hpp"
#include "GUI/MenuOfMainTabs/SEHMenuBegin/SehTab.hpp"
#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"
#include "GUI/Window.hpp"

#include <string>

enum GeneralTab_ : const uint64_t {
	GeneralCPUTab,
	GeneralStackMapTab,
	GeneralNotesTab,
	GeneralThreadsTab,
	GeneralCallStackTab,
	GeneralLogTab,
	GeneralBreakpointsTab,
	GeneralMemoryMapTab,
	GeneralSEHTab,
	GeneralScriptTab,
	GeneralSymbolsTab,
	GeneralSourceTab,
	GeneralHandlesTab,
	GeneralTraceTab,
	GeneralPETab,
	GeneralLiftersTab,
	GeneralDevirtualizersTab,
	GeneralDeobfuscatorsTab,
};

class MenuGeneral_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;

	CPUTab_ CPUTab;
	StackMapTab_ StackMapTab;
	ThreadsTab_ ThreadsTab;
	CallStackTab_ CallStackTab;
	MemoryMapTab_ MemoryMapTab;
	SEHTab_ SEHTab;
	PETab_ PETab;
private:
	const unsigned char DefaultCountOfMainTabs = 18;
private:
	void TabDispatcher(GeneralTab_ TabCode, bool* open);
public:
	void MainInterfaceRender();
};