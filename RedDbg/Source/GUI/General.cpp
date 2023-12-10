#include "GUI/General.hpp"

#include "GUI/UI/ImGui/imgui.h"

#include "GUI/UI/ImGui/Opengl3&WinApi/imgui_impl_opengl3_loader.h"
#include "GUI/UI/ImGui/Opengl3&WinApi/imgui_impl_opengl3.h"
#include "GUI/UI/ImGui/Opengl3&WinApi/imgui_impl_win32.h"

#include "GUI/UI/ImGui/imgui_internal.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

GeneralGUI_ objGeneralGUI;

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) { return true; }

    switch (msg)
    {
    case WM_NCCALCSIZE:
    {
        // Remove the window's standard sizing border
        if (wParam == TRUE && lParam != NULL)
        {
            NCCALCSIZE_PARAMS* pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
            pParams->rgrc[0].top += 1;
            pParams->rgrc[0].right -= 2;
            pParams->rgrc[0].bottom -= 2;
            pParams->rgrc[0].left += 2;
        }
        return 0;
    }
    case WM_NCHITTEST:
    {
        LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
        if (hit == HTCLIENT) {
            POINT pt;
            GetCursorPos(&pt);
            ScreenToClient(hWnd, &pt);
            if (pt.x < 5 && pt.y < 5) { return HTTOPLEFT; }
            if (pt.x > objGeneralGUI.Width - 5 && pt.y < 5) { return HTTOPRIGHT; }
            if (pt.x < 5 && pt.y > objGeneralGUI.Height - 5) { return HTBOTTOMLEFT; }
            if (pt.x > objGeneralGUI.Width - 5 && pt.y > objGeneralGUI.Height - 5) { return HTBOTTOMRIGHT; }
            if (pt.x < 5) { return HTLEFT; }
            if (pt.x > objGeneralGUI.Width - 5) { return HTRIGHT; }
            if (pt.y < 5) { return HTTOP; }
            if (pt.y > objGeneralGUI.Height - 5) { return HTBOTTOM; }
        }
        return hit;
    }
    case WM_SIZE:
    {
        if (wParam != SIZE_MINIMIZED)
        {
            objGeneralGUI.Width = LOWORD(lParam);
            objGeneralGUI.Height = HIWORD(lParam);
        }
        return 0;
    }
    case WM_SYSCOMMAND:
    {
        if ((wParam & 0xfff0) == SC_KEYMENU) /*Disable ALT application menu*/ { return 0; }
        break;
    }
    case WM_DESTROY:
    {
        ::PostQuitMessage(0);
        return 0;
    }
    }

    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

bool GeneralGUI_::CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0) { return false; }
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE) { return false; }
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!GlobalClassVars.hRC) { GlobalClassVars.hRC = wglCreateContext(data->hDC); }
    return true;
}

void GeneralGUI_::CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(NULL, NULL);
    ::ReleaseDC(hWnd, data->hDC);
}

void GeneralGUI_::OpenGl3DeInit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(GlobalClassVars.HwndMainWindow, &GlobalClassVars.MainWindow);
    wglDeleteContext(GlobalClassVars.hRC);
    ::DestroyWindow(GlobalClassVars.HwndMainWindow);
    ::UnregisterClassW(GlobalClassVars.MainWndClassExW.lpszClassName, GlobalClassVars.MainWndClassExW.hInstance);
}

bool GeneralGUI_::OpenGl3Init()
{
    GlobalClassVars.MainWndClassExW = { sizeof(GlobalClassVars.MainWndClassExW), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui", NULL };
    ::RegisterClassExW(&GlobalClassVars.MainWndClassExW);

    HWND HwndMainWindow = CreateWindowW(GlobalClassVars.MainWndClassExW.lpszClassName, L"RedDbg", WS_POPUP, 100, 100, 1280, 800, NULL, NULL, GlobalClassVars.MainWndClassExW.hInstance, NULL);

    // Initialize OpenGL
    if (!CreateDeviceWGL(HwndMainWindow, &GlobalClassVars.MainWindow))
    {
        CleanupDeviceWGL(HwndMainWindow, &GlobalClassVars.MainWindow);
        ::DestroyWindow(HwndMainWindow);
        ::UnregisterClassW(GlobalClassVars.MainWndClassExW.lpszClassName, GlobalClassVars.MainWndClassExW.hInstance);
        return false;
    }
    wglMakeCurrent(GlobalClassVars.MainWindow.hDC, GlobalClassVars.hRC);

    // Show the window
    ::ShowWindow(HwndMainWindow, SW_SHOWDEFAULT);
    ::UpdateWindow(HwndMainWindow);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    GlobalClassVars.io = &ImGui::GetIO();
    GlobalClassVars.io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    GlobalClassVars.io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(HwndMainWindow);
    ImGui_ImplOpenGL3_Init();
    return true;
}

void GeneralGUI_::OpenGlRenderMainLoop()
{
    ImVec4 ColorMainHostWindow = ImVec4(0, 0, 0, 0);

    bool done = false;

    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT) { done = true; }
        }
        if (done) { break; }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Render your UI
        UIRender();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, Width, Height);
        glClearColor(ColorMainHostWindow.x, ColorMainHostWindow.y, ColorMainHostWindow.z, ColorMainHostWindow.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        ::SwapBuffers(GlobalClassVars.MainWindow.hDC);
    }
}