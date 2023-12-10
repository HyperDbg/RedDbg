#pragma once
#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

#include "Debugger/Application/Pe/PEReader.hpp"
#include "Debugger/Application/Pe/PEInformation.hpp"

class CustomTitleBar_ 
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;
private:
	void ContentIsNullModalWindowRender(const std::string_view* Id, const std::string_view* Text, uint64_t* ErrorCode, bool* CheckBoxDontAsk);
private:
	void CustomWindowInTitleBarRender();
public:
	void CustomTitleBarMenuRender();
public:
	void CustomTitleBarRender();
};