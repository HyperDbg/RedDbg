#pragma once
#include "UI/ImGui/imgui.h"

#include "GUI/CustomTitleBar.hpp"
#include "GUI/MenuOfMainTabs/MenuGeneral.hpp"
#include "GUI/Names.hpp"
#include "GUI/GlobalClass.hpp"

#include <Windows.h>

class GeneralGUI_ {
private:
	bool CreateDeviceWGL(HWND hWnd, WGL_WindowData * data);

	void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
	void UIRender();
private:
	CustomTitleBar_ CustomTitleBar;
	MenuGeneral_ InterfaceGeneral;

	GlobalClassVars_ GlobalClassVars;
public:
	int Width = 0;
	int Height = 0;
public:
	bool OpenGl3Init();

	void OpenGlRenderMainLoop();
	void OpenGl3DeInit();
};