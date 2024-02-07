#include "GUI/CustomTitleBar.hpp"
#include "GUI/OptionsEnums.hpp"
#include "GUI/UI/ImGui/FileDialog/ImGuiFileDialog.h"

#include "DriverLoader/include/DriverLoader.hpp"

std::string GenerateRandomString(int length)
{
    static std::string result;
    if (result.size() == 0)
    {
        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_int_distribution<> distr('a', 'z');

        for (int n = 0; n < length; n++) {
            result += distr(eng);
        }
    }
    return result;
}

static std::wstring StringToWideString(
    const std::string& str)
{
    // Create a std::string_view from the input string
    std::string_view strView(str);

    // Create a std::wstring from the std::string_view using the data() function
    std::wstring wideStr(strView.data(), strView.data() + strView.size());

    return wideStr;
}

namespace MenuBarGlobalVars
{
    namespace DriverTabGlobalVars
    {
        extern std::vector<bool> DriverTabCheckbox;
        extern std::string DriverPath;
    }
}

namespace GeneralGlobalVars
{
    extern HWND HwndMainWindow;
}

namespace GlobalVarsOfPeTab {
    extern std::vector<double> Entropy;
    std::shared_ptr<PeReader> objPEInformation = nullptr;
    //PEInformation objPEInformation;
    //PeReader objPeReader;

    bool NotLoaded = true;
    bool Loaded = false;
    uint8_t* Buf = nullptr;
}

namespace CustomTitleBarGlobalVars {
    HANDLE hDriver = nullptr;
    DWORD Pid = 0;
}

void CustomTitleBar_::ContentIsNullModalWindowRender(const std::string_view* Id, const std::string_view* Text, uint64_t* ErrorCode, bool* CheckBoxDontAsk)
{
    ImGui::OpenPopup(Id->data());
    if (ImGui::BeginPopupModal(Id->data(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text(Text->data());
        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::Checkbox(Names.Windowses.TitleBarMenu.OpenFile.Error.CheckBoxText.data(), CheckBoxDontAsk);
        ImGui::PopStyleVar();

        if (ImGui::Button(Names.Windowses.TitleBarMenu.OpenFile.Error.OkButton.data(), ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); *ErrorCode = 0; }
        ImGui::SetItemDefaultFocus();
        ImGui::EndPopup();
    }
}

DWORD CustomTitleBar_::GetProcessIdByName(const std::wstring& ProcessName)
{
    DWORD pid = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &pe32)) {
            do {
                if (wcscmp(ProcessName.c_str(), pe32.szExeFile) == 0) {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &pe32));
        }

        CloseHandle(snapshot);
    }

    return pid;
}

void CustomTitleBar_::CustomWindowInTitleBarRender(MenuBarDispatcher_* Instance)
{
    if (ImGuiFileDialog::Instance()->Display(Names.Windowses.TitleBarMenu.OpenFile.OpenID.data(), 32, ImVec2{ 1000, 400 }))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {       
            if (GlobalVarsOfPeTab::objPEInformation) { peparse::DestructParsedPE(GlobalVarsOfPeTab::objPEInformation->Pe); }

            PeParser Parser;
            GlobalVarsOfPeTab::objPEInformation = Parser.Open(ImGuiFileDialog::Instance()->GetFilePathName(), true);

            static PIMAGE_NT_HEADERS pImageNTHeader = (PIMAGE_NT_HEADERS)((uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->fileBuffer->buf + (uint64_t)GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfanew);
            static PIMAGE_SECTION_HEADER pImageSectionHeader = (PIMAGE_SECTION_HEADER)((uint64_t)pImageNTHeader + 4 + sizeof(IMAGE_FILE_HEADER) + GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.SizeOfOptionalHeader);

            GlobalVarsOfPeTab::Entropy.clear();
            for (int Section = 0; Section < GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.FileHeader.NumberOfSections - 1; ++Section) 
            {
                int Offset = pImageSectionHeader[Section].PointerToRawData;
                int Length = pImageSectionHeader[Section].SizeOfRawData;
                GlobalVarsOfPeTab::Entropy.push_back(CalcEntropy(GlobalVarsOfPeTab::objPEInformation->Pe->fileBuffer->buf, Offset, Offset + Length));
            }

            if (MenuBarGlobalVars::DriverTabGlobalVars::DriverTabCheckbox[OptionEnums::AutomaticallyLoadAfterOpenPEFile])
            {
                static std::unique_ptr<DRIVER> Driver = std::make_unique<DRIVER>(MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.c_str(),
                    GenerateRandomString(16).c_str(),
                    GenerateRandomString(16).c_str(), SERVICE_DEMAND_START);
                if (MenuBarGlobalVars::DriverTabGlobalVars::DriverPath[0] != 0)
                {
                    if (!GlobalVarsOfPeTab::Loaded)
                    {
                        GlobalVarsOfPeTab::NotLoaded = false;
                        GlobalVarsOfPeTab::Loaded = true;
                        Driver->LoadDriver(
                            MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.c_str(),
                            GenerateRandomString(16).c_str(),
                            GenerateRandomString(16).c_str(), SERVICE_DEMAND_START);
                    }

                    CustomTitleBarGlobalVars::hDriver = CreateFileA(Names.Windowses.TitleBarMenu.Device.data(),
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ |
                        FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL |
                        FILE_FLAG_OVERLAPPED,
                        NULL);
                }
            }
            CustomTitleBarGlobalVars::Pid = GetProcessIdByName(std::filesystem::path(ImGuiFileDialog::Instance()->GetFilePathName()).filename());
        }
        ImGuiFileDialog::Instance()->Close();
    }

    if (Instance != nullptr)
    {
        Instance->Display(Names.Windowses.TitleBarMenu.MenuOptions.Preferences.WindowId.data());
    }

    //static bool dont_ask_me_next_time = false;
    //switch (GlobalVarsOfPeTab::objPeReader.ErrorCode)
    //{
    //case AllOk: return;
    //case hPeFileContainsInvalidHandleValue: return ContentIsNullModalWindowRender(
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.hPeFileContentId,
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.hPeFileContentText,
    //    &GlobalVarsOfPeTab::objPeReader.ErrorCode,
    //    &dont_ask_me_next_time);
    //case pImageDOSHeaderOfPeContainsNullptr: return ContentIsNullModalWindowRender(
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageDOSHeaderOfPeId,
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageDOSHeaderOfPeText,
    //    &GlobalVarsOfPeTab::objPeReader.ErrorCode,
    //    &dont_ask_me_next_time);
    //case pImageNTHeaderOfPeContainsNullptr: return ContentIsNullModalWindowRender(
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageNTHeaderOfPeId,
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageNTHeaderOfPeText,
    //    &GlobalVarsOfPeTab::objPeReader.ErrorCode,
    //    &dont_ask_me_next_time);
    //case pImageNTHeader64ContainsNullptr: return ContentIsNullModalWindowRender(
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageNTHeader64Id,
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageNTHeader64Text,
    //    &GlobalVarsOfPeTab::objPeReader.ErrorCode,
    //    &dont_ask_me_next_time);
    //case pImageSectionHeaderContainsNullptr: return ContentIsNullModalWindowRender(
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageSectionHeaderId,
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageSectionHeaderText,
    //    &GlobalVarsOfPeTab::objPeReader.ErrorCode,
    //    &dont_ask_me_next_time);
    //case pImageImportHeaderContainsNullptr: return ContentIsNullModalWindowRender(
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageImportHeaderId,
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageImportHeaderText,
    //    &GlobalVarsOfPeTab::objPeReader.ErrorCode,
    //    &dont_ask_me_next_time);
    //case pImageImportDescriptorContainsNullptr: return ContentIsNullModalWindowRender(
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageImportDescriptorId,
    //    &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageImportDescriptorText,
    //    &GlobalVarsOfPeTab::objPeReader.ErrorCode,
    //    &dont_ask_me_next_time);
    //}
}

void CustomTitleBar_::CustomTitleBarMenuRender()
{
    static MenuBarDispatcher_* Instance = nullptr;
    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuFileName.data()))
    {
        if (ImGui::BeginMenu("User debug"))
        {
            if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuOpenItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuOpenItemShortcutName.data(), false))
            {
                ImGuiFileDialog::Instance()->OpenDialog(
                    Names.Windowses.TitleBarMenu.OpenFile.OpenID.data(),
                    Names.Windowses.TitleBarMenu.OpenFile.OpenTitle.data(),
                    Names.Windowses.TitleBarMenu.OpenFile.Filter.data(),
                    "D:\\Programs\\Games\\JustCause4\\JustCause4",
                    "");
            }

            ImGui::Separator();

            if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuAttachItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuAttachItemShortcutName.data()))
            {

            }
            if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuDetachItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuDetachItemShortcutName.data()))
            {

            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Kernel debug"))
        {
            if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuOpenItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuOpenItemShortcutName.data(), false))
            {
                ImGuiFileDialog::Instance()->OpenDialog(
                    Names.Windowses.TitleBarMenu.OpenFile.OpenID.data(),
                    Names.Windowses.TitleBarMenu.OpenFile.OpenTitle.data(),
                    Names.Windowses.TitleBarMenu.OpenFile.Filter.data(),
                    "D:\\Programs\\Games\\JustCause4\\JustCause4",
                    "");
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("System"))
            {
                if (ImGui::MenuItem("NET"))
                {

                }
                if (ImGui::MenuItem("USB"))
                {

                }
                if (ImGui::MenuItem("1394"))
                {

                }
                if (ImGui::MenuItem("Local"))
                {

                }
                if (ImGui::MenuItem("COM"))
                {

                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Local"))
            {

            }
            ImGui::Separator();

            if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuAttachItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuAttachItemShortcutName.data()))
            {

            }
            if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuDetachItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuDetachItemShortcutName.data()))
            {

            }

            ImGui::Separator();

            if (ImGui::MenuItem("Driver load", (const char*)0, false, GlobalVarsOfPeTab::NotLoaded))
            {
                if (MenuBarGlobalVars::DriverTabGlobalVars::DriverPath[0] != 0)
                {
                    static std::unique_ptr<DRIVER> Driver = std::make_unique<DRIVER>(MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.c_str(),
                        GenerateRandomString(16).c_str(),
                        GenerateRandomString(16).c_str(), SERVICE_DEMAND_START);

                    GlobalVarsOfPeTab::NotLoaded = false;
                    GlobalVarsOfPeTab::Loaded = true;
                    Driver->LoadDriver(
                        MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.c_str(),
                        GenerateRandomString(16).c_str(),
                        GenerateRandomString(16).c_str(), SERVICE_DEMAND_START);
                    CustomTitleBarGlobalVars::hDriver = CreateFileA(Names.Windowses.TitleBarMenu.Device.data(),
                        GENERIC_READ | GENERIC_WRITE,
                        FILE_SHARE_READ |
                        FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL |
                        FILE_FLAG_OVERLAPPED,
                        NULL);
                }
            }
            if (ImGui::MenuItem("Driver unload", (const char*)0, false, GlobalVarsOfPeTab::Loaded))
            {
                GlobalVarsOfPeTab::NotLoaded = true;
                GlobalVarsOfPeTab::Loaded = false;
                static std::unique_ptr<DRIVER> Driver = std::make_unique<DRIVER>(MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.c_str(),
                    GenerateRandomString(16).c_str(),
                    GenerateRandomString(16).c_str(), SERVICE_DEMAND_START);
                Driver->UnloadDriver();
                CustomTitleBarGlobalVars::hDriver = nullptr;
            }

            ImGui::EndMenu();
        }

        {
            ImGui::Separator();
            if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuRecentFilesName.data()))
            {

                ImGui::EndMenu();
            }
        }

        {
            ImGui::Separator();
            if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuDatabaseName.data()))
            {
                if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuDatabaseItems.MenuSaveDatabaseItemName.data()))
                {

                }
                if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuDatabaseItems.MenuReloadDatabaseItemName.data()))
                {

                }
                if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuDatabaseItems.MenuRestoreBackupDatabaseItemName.data()))
                {

                }
                if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuDatabaseItems.MenuClearDatabaseItemName.data()))
                {

                }

                {
                    ImGui::Separator();
                    if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuDatabaseItems.MenuImportDatabaseItemName.data()))
                    {

                    }
                    if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuDatabaseItems.MenuExportDatabaseItemName.data()))
                    {

                    }
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
        }

        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuPatchFileItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuPatchFileItemShortcutName.data()))
        {

        }
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuChangeCommandLineItemName.data()))
        {

        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuDebugName.data()))
    {
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuDebug.MenuRunItemName.data(), Names.Windowses.TitleBarMenu.MenuDebug.Shortcut.MenuRunItemShortcutName.data()))
        {

        }
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuDebug.MenuPauseItemName.data(), Names.Windowses.TitleBarMenu.MenuDebug.Shortcut.MenuPauseItemShortcutName.data()))
        {

        }
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuDebug.MenuRestartItemName.data(), Names.Windowses.TitleBarMenu.MenuDebug.Shortcut.MenuRestartItemShortcutName.data()))
        {

        }
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuDebug.MenuCloseItemName.data(), Names.Windowses.TitleBarMenu.MenuDebug.Shortcut.MenuCloseItemShortcutName.data()))
        {

        }

        {
            ImGui::Separator();
            if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuDebug.MenuStepIntoItemName.data(), Names.Windowses.TitleBarMenu.MenuDebug.Shortcut.MenuStepIntoShortcutItemName.data()))
            {

            }
            if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuDebug.MenuStepOverItemName.data(), Names.Windowses.TitleBarMenu.MenuDebug.Shortcut.MenuStepOverShortcutItemName.data()))
            {

            }

            {
                if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuDebug.MenuStepLeftItemName.data(), Names.Windowses.TitleBarMenu.MenuDebug.Shortcut.MenuStepLeftShortcutItemName.data()))
                {

                }
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip(Names.Windowses.TitleBarMenu.MenuDebug.MenuStepLeftItemDescription.data());
                }
            }
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuTracingName.data()))
    {
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuTracing.MenuTraceIntoItemName.data(), Names.Windowses.TitleBarMenu.MenuTracing.Shortcut.MenuTraceIntoItemShortcutName.data()))
        {

        }
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuTracing.MenuTraceIntoItemName.data(), Names.Windowses.TitleBarMenu.MenuTracing.Shortcut.MenuTraceIntoItemShortcutName.data()))
        {

        }
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuTracing.MenuTraceLBRItemName.data(), Names.Windowses.TitleBarMenu.MenuTracing.Shortcut.MenuTraceLBRItemShortcutName.data()))
        {

        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuOptionsName.data()))
    {
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuOptions.MenuPreferencesItemName.data()))
        {
            Instance = MenuBarDispatcher.OpenInstance(
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.WindowId.data(),
                Names.Windowses.TitleBarMenu.MenuOptions.Preferences.HostTitleName.data());
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuHelpName.data()))
    {
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuHelp.MenuAboutItemName.data()))
        {

        }

        {
            ImGui::Separator();
            if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuHelp.DropDownMenuGitHubName.data()))
            {
                if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuHelp.DropDownMenuGitHubItems.MenuRedDbgItemName.data()))
                {
                    ShellExecuteA(0, 0, Names.Windowses.TitleBarMenu.MenuHelp.DropDownMenuGitHubItems.MenuRedDbgLinkName.data(), 0, 0, SW_SHOW);
                }
                if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuHelp.DropDownMenuGitHubItems.MenuSVMHyperVisorFrameWorkItemName.data()))
                {
                    ShellExecuteA(0, 0, Names.Windowses.TitleBarMenu.MenuHelp.DropDownMenuGitHubItems.MenuSVMHyperVisorFrameWorkLinkName.data(), 0, 0, SW_SHOW);
                }
                ImGui::EndMenu();
            }
            ImGui::Separator();
        }

        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuHelp.MenuCrackLabItemName.data()))
        {
            ShellExecuteA(0, 0, Names.Windowses.TitleBarMenu.MenuHelp.MenuCrackLabLinkName.data(), 0, 0, SW_SHOW);
        }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();

    CustomWindowInTitleBarRender(Instance);
}

void CustomTitleBar_::CustomTitleBarRender()
{
	WINDOWPLACEMENT wp{};
	GetWindowPlacement(GeneralGlobalVars::HwndMainWindow, &wp);

    static const ImGuiWindowFlags TitleBarFlags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse;

    static const ImGuiWindowFlags TitleBarFlagsOfMenuWindow =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(ImVec2{ viewport->WorkSize.x, Dimensionses.TitleBarHeight });

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{ 0.0f, 0.0f });

    // Properly center the content accoring to the titlebar height
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ Dimensionses.TitleBarHeight / 3, Dimensionses.TitleBarHeight / 3 });
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });

    ImGui::Begin(Names.Windowses.TitleBarName.data(), nullptr, TitleBarFlags);
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(1);

    ImGui::TextColored(ImVec4{ 1.0f,1.0f,1.0f,1.0f }, Names.Windowses.RedDebuggerName.data());
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
    ImGui::SameLine(ImGui::GetWindowWidth() - 3 * Dimensionses.TitleBarHeight);

    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Dimensionses.U32GrayColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, Dimensionses.U32GrayColor);
    if (ImGui::Button("-")) {
        ShowWindow(GeneralGlobalVars::HwndMainWindow, SW_MINIMIZE);
    }
    ImGui::SameLine();

    static bool Maximized = false;
    static RECT prevRect;
    if (ImGui::Button("[]")) {
        HWND hwnd = GeneralGlobalVars::HwndMainWindow;
        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hwnd, &wp);

        if (!Maximized) {
            CopyRect(&prevRect, &wp.rcNormalPosition);
        }

        if (!Maximized) {
            HMONITOR hmon = MonitorFromWindow(GeneralGlobalVars::HwndMainWindow, MONITOR_DEFAULTTONEAREST);
            MONITORINFO mi;
            mi.cbSize = sizeof(MONITORINFO);
            GetMonitorInfo(hmon, &mi);
            SetWindowPos(GeneralGlobalVars::HwndMainWindow, NULL, mi.rcWork.left, mi.rcWork.top, mi.rcWork.right - mi.rcWork.left, mi.rcWork.bottom - mi.rcWork.top, SWP_NOZORDER | SWP_NOACTIVATE);
            Maximized = true;
        }
        else {
            SetWindowPos(hwnd, NULL, prevRect.left, prevRect.top, prevRect.right - prevRect.left, prevRect.bottom - prevRect.top, SWP_NOZORDER | SWP_NOACTIVATE);
            Maximized = false;
        }
    }
    ImGui::PopStyleColor(2);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Dimensionses.U32RedColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, Dimensionses.U32RedColor);
    if (ImGui::Button("X")) {
        if (GlobalVarsOfPeTab::Loaded)
        {
            static std::unique_ptr<DRIVER> Driver = std::make_unique<DRIVER>(MenuBarGlobalVars::DriverTabGlobalVars::DriverPath.c_str(),
                GenerateRandomString(16).c_str(),
                GenerateRandomString(16).c_str(), SERVICE_DEMAND_START);
            Driver->UnloadDriver();
            CustomTitleBarGlobalVars::hDriver = nullptr;
        }
        PostQuitMessage(0);
    }
    ImGui::PopStyleColor(3);

    /*RedDebugger*/
    ImGuiViewport WorkPos{};
    WorkPos.WorkPos.x = viewport->WorkPos.x + Dimensionses.TitleBarFromTextRedDbg;
    WorkPos.WorkPos.y = viewport->WorkPos.y + Dimensionses.MiddleOfText;
    WorkPos.WorkSize.x = viewport->WorkSize.x - 200;
    WorkPos.WorkSize.y = viewport->WorkSize.y;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{ 0.0f, 0.0f });

    // Properly center the content accoring to the titlebar height
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ Dimensionses.TitleBarHeight / 3, Dimensionses.TitleBarHeight / 3 });
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4{ 0.00f, 0.00f, 0.00f, 0.00f });

    Window.ChildWindowRender(
        Names.Windowses.MenuBarOfTitleBarName,
        ImVec2(WorkPos.WorkPos.x, WorkPos.WorkPos.y),
        ImVec2(WorkPos.WorkSize.x, WorkPos.WorkSize.y),
        WindowCodes::MenuBarOfTitleBarWindowCode,
        *this,
        true, 
        TitleBarFlagsOfMenuWindow);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);

    ImGui::End();
}