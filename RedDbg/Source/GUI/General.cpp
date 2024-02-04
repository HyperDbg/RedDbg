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
#include <windowsx.h>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

GeneralGUI_ objGeneralGUI;

namespace GeneralGlobalVars
{
    HWND HwndMainWindow = nullptr;
}

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
        // Handle window resizing and moving
        LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
        POINT cursor = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

        const POINT border{
            static_cast<LONG>((::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER)) * 1.0),
            static_cast<LONG>((::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CXPADDEDBORDER)) * 1.0)
        };

        RECT window;
        if (!::GetWindowRect(hWnd, &window)) {
            return HTNOWHERE;
        }

        constexpr static auto RegionClient = 0b0000;
        constexpr static auto RegionLeft = 0b0001;
        constexpr static auto RegionRight = 0b0010;
        constexpr static auto RegionTop = 0b0100;
        constexpr static auto RegionBottom = 0b1000;

        const auto result =
            RegionLeft * (cursor.x < (window.left + border.x)) |
            RegionRight * (cursor.x >= (window.right - border.x)) |
            RegionTop * (cursor.y < (window.top + border.y)) |
            RegionBottom * (cursor.y >= (window.bottom - border.y));

        if (result != 0 && (ImGui::IsAnyItemHovered() || ImGui::IsPopupOpen(nullptr, ImGuiPopupFlags_AnyPopupId))) {
            break;
        }

        std::string_view hoveredWindowName = GImGui->HoveredWindow == nullptr ? "" : GImGui->HoveredWindow->Name;

        switch (result) {
        case RegionLeft: return HTLEFT;
        case RegionRight: return HTRIGHT;
        case RegionTop: return HTTOP;
        case RegionBottom: return HTBOTTOM;
        case RegionTop | RegionLeft: return HTTOPLEFT;
        case RegionTop | RegionRight: return HTTOPRIGHT;
        case RegionBottom | RegionLeft: return HTBOTTOMLEFT;
        case RegionBottom | RegionRight: return HTBOTTOMRIGHT;
        case RegionClient:
        default:
            if (cursor.y < (window.top + 25 * 2)) {
                if (hoveredWindowName == "##TitleBar/##MenuBarOfTitleBar" || hoveredWindowName == "##TitleBar") {
                    if (!ImGui::IsAnyItemHovered()) {
                        return HTCAPTION;
                    }
                }
            }

            break;
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

    CleanupDeviceWGL(GeneralGlobalVars::HwndMainWindow, &GlobalClassVars.MainWindow);
    wglDeleteContext(GlobalClassVars.hRC);
    ::DestroyWindow(GeneralGlobalVars::HwndMainWindow);
    ::UnregisterClassW(GlobalClassVars.MainWndClassExW.lpszClassName, GlobalClassVars.MainWndClassExW.hInstance);
}

bool GeneralGUI_::OpenGl3Init()
{
    GlobalClassVars.MainWndClassExW = { sizeof(GlobalClassVars.MainWndClassExW), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui", NULL };
    ::RegisterClassExW(&GlobalClassVars.MainWndClassExW);

    GeneralGlobalVars::HwndMainWindow = CreateWindowW(GlobalClassVars.MainWndClassExW.lpszClassName, L"RedDbg", WS_POPUP, 100, 100, 1280, 800, NULL, NULL, GlobalClassVars.MainWndClassExW.hInstance, NULL);
    //GlobalClassVars.HwndMainWindow = HwndMainWindow;
    // Initialize OpenGL
    if (!CreateDeviceWGL(GeneralGlobalVars::HwndMainWindow, &GlobalClassVars.MainWindow))
    {
        CleanupDeviceWGL(GeneralGlobalVars::HwndMainWindow, &GlobalClassVars.MainWindow);
        ::DestroyWindow(GeneralGlobalVars::HwndMainWindow);
        ::UnregisterClassW(GlobalClassVars.MainWndClassExW.lpszClassName, GlobalClassVars.MainWndClassExW.hInstance);
        return false;
    }
    wglMakeCurrent(GlobalClassVars.MainWindow.hDC, GlobalClassVars.hRC);

    // Show the window
    ::ShowWindow(GeneralGlobalVars::HwndMainWindow, SW_SHOWDEFAULT);
    ::UpdateWindow(GeneralGlobalVars::HwndMainWindow);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    GlobalClassVars.io = &ImGui::GetIO();
    GlobalClassVars.io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    GlobalClassVars.io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(GeneralGlobalVars::HwndMainWindow);
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