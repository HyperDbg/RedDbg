#include "GUI/CustomTitleBar.hpp"
#include "GUI/UI/ImGui/FileDialog/ImGuiFileDialog.h"

namespace GlobalVarsOfPeTab {
    extern std::vector<float> Entropy;
    PEInformation objPEInformation;
    PeReader objPeReader;
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

void CustomTitleBar_::CustomWindowInTitleBarRender()
{
    if (ImGuiFileDialog::Instance()->Display(Names.Windowses.TitleBarMenu.OpenFile.OpenID.data(), 32, ImVec2{ 1000, 400 }))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {       
            GlobalVarsOfPeTab::objPeReader.PathToCurrentDebugging = ImGuiFileDialog::Instance()->GetFilePathName();
            GlobalVarsOfPeTab::objPeReader.Start = true;
            GlobalVarsOfPeTab::objPEInformation = GlobalVarsOfPeTab::objPeReader.Pe(GlobalVarsOfPeTab::objPEInformation);

            GlobalVarsOfPeTab::Entropy.clear();
            for (int Section = 0; Section < GlobalVarsOfPeTab::objPEInformation.ImageFileHeader.NumberOfSections - 1; ++Section) {
                int Offset = GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Section].PointerToRawData;
                int Length = GlobalVarsOfPeTab::objPEInformation.pImageSectionHeader[Section].SizeOfRawData;
                GlobalVarsOfPeTab::Entropy.push_back(GlobalVarsOfPeTab::objPeReader.CalcEntropy((char*)GlobalVarsOfPeTab::objPEInformation.pImageDOSHeaderOfPe, Offset, Offset + Length));
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }

    static bool dont_ask_me_next_time = false;
    switch (GlobalVarsOfPeTab::objPeReader.ErrorCode)
    {
    case AllOk: return;
    case hPeFileContainsInvalidHandleValue: return ContentIsNullModalWindowRender(
        &Names.Windowses.TitleBarMenu.OpenFile.Error.hPeFileContentId,
        &Names.Windowses.TitleBarMenu.OpenFile.Error.hPeFileContentText,
        &GlobalVarsOfPeTab::objPeReader.ErrorCode,
        &dont_ask_me_next_time);
    case pImageDOSHeaderOfPeContainsNullptr: return ContentIsNullModalWindowRender(
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageDOSHeaderOfPeId,
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageDOSHeaderOfPeText,
        &GlobalVarsOfPeTab::objPeReader.ErrorCode,
        &dont_ask_me_next_time);
    case pImageNTHeaderOfPeContainsNullptr: return ContentIsNullModalWindowRender(
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageNTHeaderOfPeId,
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageNTHeaderOfPeText,
        &GlobalVarsOfPeTab::objPeReader.ErrorCode,
        &dont_ask_me_next_time);
    case pImageNTHeader64ContainsNullptr: return ContentIsNullModalWindowRender(
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageNTHeader64Id,
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageNTHeader64Text,
        &GlobalVarsOfPeTab::objPeReader.ErrorCode,
        &dont_ask_me_next_time);
    case pImageSectionHeaderContainsNullptr: return ContentIsNullModalWindowRender(
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageSectionHeaderId,
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageSectionHeaderText,
        &GlobalVarsOfPeTab::objPeReader.ErrorCode,
        &dont_ask_me_next_time);
    case pImageImportHeaderContainsNullptr: return ContentIsNullModalWindowRender(
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageImportHeaderId,
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageImportHeaderText,
        &GlobalVarsOfPeTab::objPeReader.ErrorCode,
        &dont_ask_me_next_time);
    case pImageImportDescriptorContainsNullptr: return ContentIsNullModalWindowRender(
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageImportDescriptorId,
        &Names.Windowses.TitleBarMenu.OpenFile.Error.pImageImportDescriptorText,
        &GlobalVarsOfPeTab::objPeReader.ErrorCode,
        &dont_ask_me_next_time);
    }
}

void CustomTitleBar_::CustomTitleBarMenuRender()
{
    ImGui::BeginMenuBar();
    if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuFileName.data()))
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

        {
            ImGui::Separator();
            if (ImGui::BeginMenu(Names.Windowses.TitleBarMenu.MenuFile.DropDownMenuRecentFilesName.data()))
            {

                ImGui::EndMenu();
            }
            ImGui::Separator();
        }

        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuAttachItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuAttachItemShortcutName.data()))
        {

        }
        if (ImGui::MenuItem(Names.Windowses.TitleBarMenu.MenuFile.MenuDetachItemName.data(), Names.Windowses.TitleBarMenu.MenuFile.Shortcut.MenuDetachItemShortcutName.data()))
        {

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
            ImGui::OpenPopup("asdasdasd");
            if (ImGui::BeginPopupModal("asdasdasd", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Text->data()");
                ImGui::Separator();

                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
            }
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

    CustomWindowInTitleBarRender();
}

void CustomTitleBar_::CustomTitleBarRender()
{
	WINDOWPLACEMENT wp{};
	GetWindowPlacement(GlobalClassVars.HwndMainWindow, &wp);

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
    if (ImGui::Button("-")) {
        ShowWindow(GlobalClassVars.HwndMainWindow, SW_MINIMIZE);
    }
    ImGui::SameLine();
    if (ImGui::Button("[]")) {
        if (wp.showCmd == SW_MAXIMIZE) {
            ShowWindow(GlobalClassVars.HwndMainWindow, SW_RESTORE);
        }
        else {
            ShowWindow(GlobalClassVars.HwndMainWindow, SW_MAXIMIZE);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("X")) {
        PostQuitMessage(0);
    }
    ImGui::PopStyleColor();

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