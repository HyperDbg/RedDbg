#pragma once
#include "GUI/UI/ImGui/imgui.h"

#include <Windows.h>
class WGL_WindowData { public: HDC hDC; };

class GlobalClassVars_ {
public:
	HWND HwndMainWindow = nullptr;
	WNDCLASSEXW MainWndClassExW{};

	HGLRC hRC{};
	WGL_WindowData MainWindow{};

	ImGuiIO* io = nullptr;
};
