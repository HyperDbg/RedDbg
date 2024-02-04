#pragma once
#include "GUI/MenuOfMainTabs/PeMenuBegin/Parser/PEReader.hpp"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"
#include "MenuBarDispatcher.hpp"

//#include "Debugger/Application/Pe/PEReader.hpp"
//#include "Debugger/Application/Pe/PEInformation.hpp"

#include <vector>
#include <string>
#include <TlHelp32.h>
#include <utility>
#include <random>

//enum InstanceCodes

class CustomTitleBar_ 
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;
	MenuBarDispatcher_ MenuBarDispatcher;
private:
	DWORD GetProcessIdByName(const std::wstring& ProcessName);
	void ContentIsNullModalWindowRender(const std::string_view* Id, const std::string_view* Text, uint64_t* ErrorCode, bool* CheckBoxDontAsk);
private:
	void CustomWindowInTitleBarRender(MenuBarDispatcher_* Instance);
public:
	void CustomTitleBarMenuRender();
public:
	void CustomTitleBarRender();
};