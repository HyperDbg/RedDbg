#include "GUI/MenuOfMainTabs/MemoryMapMenuBegin/MemoryMapTab.hpp"

#include <vector>
#include <Debugger/ErrorCodes.hpp>

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

void MemoryMapTab_::MemoryMapWindowRender()
{
    static float MemoryMapHorizontalSplitterRelPosY = 0.95f;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        MemoryMapHorizontalSplitterRelPosY,
        WindowCodes::MemoryMapTabWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.RightPos.x + WindowGeneralSizes.RightSize.x;
    posMax.y += WindowGeneralSizes.RightPos.y + WindowGeneralSizes.RightSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos,
        posMax,
        &StartPosY))
    {
        MemoryMapHorizontalSplitterRelPosY =
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::MemoryMapWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMapWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::FindInMemoryMapWindowCode,
        *this,
        false);
}

//static void CallBackUpdateMemoryCache(std::shared_ptr<MemoryRefreshThread> objMemoryRefreshThread)
//{
//    objMemoryRefreshThread->Parse->UpdateMemoryCache(objMemoryRefreshThread->Active);
//}

void MemoryMapTab_::GetMemoryInfoSafe(MemoryParser_& Parse, std::shared_ptr<std::atomic<bool>> Active) {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    if (duration_cast<std::chrono::seconds>(now - Parse.Cache.LastUpdated) > std::chrono::milliseconds(Dimensionses.CacheRefreshRate))
    {
        std::jthread updateThread = std::jthread([&Parse, Active]() {
            Parse.UpdateMemoryCache(Active);
            });

        updateThread.detach();
    }
    return;
}

void MemoryMapTab_::MemoryMapTableRender()
{
    if (CustomTitleBarGlobalVars::Pid != 0 &&
        ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableName.data(), Dimensionses.CountOfMemoryMapColumns, DefaultTableFlags))
    {
        //static int i = 0;
        //if (i == 0)
        //{
        //    Parse.GetMemoryMapOfUserProcess();
        //    ++i;
        //}
        //static int i = 0;
        //if (CustomTitleBarGlobalVars::hDriver != nullptr && i < 1)
        //{
        //    ULONG                           ReturnedLength;
        //    UINT32                          ModuleDetailsSize = 0;
        //    UINT32                          ModulesCount = 0;
        //    PUSERMODE_LOADED_MODULE_DETAILS ModuleDetailsRequest = NULL;
        //    PUSERMODE_LOADED_MODULE_SYMBOLS Modules = NULL;
        //    USERMODE_LOADED_MODULE_DETAILS  ModuleCountRequest = { 0 };
        //
        //    ModuleCountRequest.ProcessId = CustomTitleBarGlobalVars::Pid;
        //    ModuleCountRequest.OnlyCountModules = TRUE;
        //    ModuleCountRequest.TypeOfLoad = ModuleLoadType::InitOrder;
        //
        //    bool Status = DeviceIoControl(
        //        CustomTitleBarGlobalVars::hDriver,
        //        0, 
        //        &ModuleCountRequest,                    // Input Buffer to driver.
        //        sizeof(USERMODE_LOADED_MODULE_DETAILS), // Input buffer length
        //        &ModuleCountRequest,                    // Output Buffer from driver.
        //        sizeof(USERMODE_LOADED_MODULE_DETAILS), // Length of output
        //        // buffer in bytes.
        //        &ReturnedLength,                        // Bytes placed in buffer.
        //        NULL);
        //
        //    if (!Status)
        //    {
        //        printf("ioctl failed with code 0x%x\n", GetLastError());
        //    }
        //
        //    //
        //    // Check if counting modules was successful or not
        //    //
        //    if (ModuleCountRequest.Result == SuccessCodes::DEBUGGER_OPERATION_WAS_SUCCESSFUL)
        //    {
        //        ModulesCount = ModuleCountRequest.ModulesCount;
        //
        //        printf("Count of modules : 0x%x\n", ModuleCountRequest.ModulesCount);
        //
        //        ModuleDetailsSize = sizeof(USERMODE_LOADED_MODULE_DETAILS) +
        //            (ModuleCountRequest.ModulesCount * sizeof(USERMODE_LOADED_MODULE_SYMBOLS));
        //
        //        ModuleDetailsRequest = (PUSERMODE_LOADED_MODULE_DETAILS)malloc(ModuleDetailsSize);
        //
        //        if (ModuleDetailsRequest == NULL)
        //        {
        //            printf("ModuleDetailsRequest is NULL\n");
        //        }
        //
        //        //RtlZeroMemory(ModuleDetailsRequest, ModuleDetailsSize);
        //
        //        //
        //        // Set the module details to get the modules (not count)
        //        //
        //        ModuleDetailsRequest->ProcessId = CustomTitleBarGlobalVars::Pid;
        //        ModuleDetailsRequest->OnlyCountModules = FALSE;
        //        ModuleDetailsRequest->TypeOfLoad = ModuleLoadType::InitOrder;
        //        //
        //        // Send the request to the kernel
        //        //
        //        Status = DeviceIoControl(
        //            CustomTitleBarGlobalVars::hDriver, // Handle to device
        //            0, // IO Control
        //            // code
        //            ModuleDetailsRequest,                   // Input Buffer to driver.
        //            sizeof(USERMODE_LOADED_MODULE_DETAILS), // Input buffer length
        //            ModuleDetailsRequest,                   // Output Buffer from driver.
        //            ModuleDetailsSize,                      // Length of output
        //            // buffer in bytes.
        //            &ReturnedLength,                        // Bytes placed in buffer.
        //            NULL                                    // synchronous call
        //        );
        //
        //        if (!Status)
        //        {
        //            free(ModuleDetailsRequest);
        //            printf("ioctl failed with code 0x%x\n", GetLastError());
        //        }
        //
        //        //
        //        // Show modules list
        //        //
        //        if (ModuleCountRequest.Result == DEBUGGER_OPERATION_WAS_SUCCESSFUL)
        //        {
        //            Modules = (PUSERMODE_LOADED_MODULE_SYMBOLS)((UINT64)ModuleDetailsRequest +
        //                sizeof(USERMODE_LOADED_MODULE_DETAILS));
        //            printf("user mode\n");
        //            printf("start\t\t\tentrypoint\t\tpath\n\n");
        //
        //            /*
        //            if (SearchModule != NULL)
        //            {
        //                CharSize = strlen(SearchModule) + 1;
        //                WcharBuff = (wchar_t*)malloc(CharSize * 2);
        //
        //                if (WcharBuff == NULL)
        //                {
        //                    return FALSE;
        //                }
        //
        //                RtlZeroMemory(WcharBuff, CharSize);
        //
        //                mbstowcs(WcharBuff, SearchModule, CharSize);
        //
        //                SearchModuleString.assign(WcharBuff, wcslen(WcharBuff));
        //            }
        //            */
        //            for (size_t i = 0; i < ModulesCount; i++)
        //            {
        //                //
        //                // Check if we need to search for the module or not
        //                //
        //                /*
        //                if (SearchModule != NULL)
        //                {
        //                    //
        //                    // Convert FullPathName to string
        //                    //
        //                    std::wstring FullPathName((wchar_t*)Modules[i].FilePath);
        //
        //                    if (FindCaseInsensitiveW(FullPathName, SearchModuleString, 0) == std::wstring::npos)
        //                    {
        //                        //
        //                        // not found
        //                        //
        //                        continue;
        //                    }
        //                }
        //                */
        //                //
        //                // Check if module is 32-bit or not
        //                //
        //                if (ModuleDetailsRequest->Is32Bit)
        //                {
        //                    //ShowMessages("%016llx\t%016llx\t%ws\n",
        //                    //    Modules[i].BaseAddress,
        //                    //    Modules[i].Entrypoint,
        //                    //    CommandLmConvertWow64CompatibilityPaths(Modules[i].FilePath).c_str());
        //                }
        //                else
        //                {
        //                    printf("%016llx\t%016llx\t%ws\n",
        //                        Modules[i].BaseAddress,
        //                        Modules[i].Entrypoint,
        //                        Modules[i].FilePath);
        //                }
        //            }
        //            /*
        //            if (SearchModule != NULL)
        //            {
        //                free(WcharBuff);
        //            }
        //            */
        //        }
        //        else
        //        {
        //            printf("2 ErrorMessage 0x%x", ModuleCountRequest.Result);
        //        }
        //
        //        free(ModuleDetailsRequest);
        //    }
        //    else
        //    {
        //        printf("1 ErrorMessage 0x%x", ModuleCountRequest.Result);
        //    }
        //    ++i;
        //}

        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableAddressColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableSizeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTablePartyColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableInfoColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableContentColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableTypeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableProtectionColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableInitialColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        static MemoryParser_ Parse;
        static std::shared_ptr<std::atomic<bool>> Active = std::make_shared<std::atomic<bool>>(true);
        GetMemoryInfoSafe(Parse, Active);
        Active->store(true);
        ImGuiListClipper clipper;
        clipper.Begin(Parse.Cache.vMemoryInfo.size(), 17);//TODO: Data transfer
        while (clipper.Step())
        {
            for (int Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfMemoryMapColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfMemoryMapColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);
                    
                    if (Column == 0)
                    {
                        std::stringstream Ss;
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint64_t) * 2) << std::hex <<
                                Parse.Cache.vMemoryInfo[Row].BaseAddress;
                        }
                        else
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint32_t) * 2) << std::hex <<
                                Parse.Cache.vMemoryInfo[Row].BaseAddress;
                        }

                        ImGui::Selectable(Ss.str().c_str());
                    }
                    else if (Column == 1)
                    {
                        std::stringstream Ss;
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint64_t) * 2) << std::hex <<
                                Parse.Cache.vMemoryInfo[Row].Size;
                        }
                        else
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint32_t) * 2) << std::hex <<
                                Parse.Cache.vMemoryInfo[Row].Size;
                        }

                        ImGui::Selectable(Ss.str().c_str());
                    }
                    else if (Column == 2) { ImGui::Selectable(Parse.Cache.vMemoryInfo[Row].Party == nsMemoryParser::Party::User ? "User" : "System"); }
                    else if (Column == 3)
                    {
                        ImGui::Selectable(Parse.Cache.vMemoryInfo[Row].Info.c_str());
                        std::size_t found = Parse.Cache.vMemoryInfo[Row].FullExePath.filename().string().find(Parse.Cache.vMemoryInfo[Row].Info.c_str());
                        //if (found == std::string::npos) { found = Parse.Cache.vMemoryInfo[Row].FullExePath.string().find(".dll"); }
                        
                        if(found != std::string::npos && Parse.Cache.vMemoryInfo[Row].FullExePath.string().size() > 0 && ImGui::IsItemHovered() && ImGui::BeginTooltip())
                        {
                            ImGui::Text(Parse.Cache.vMemoryInfo[Row].FullExePath.string().data());
                            ImGui::EndTooltip();
                        }
                    }
                    else if (Column == 4) { ImGui::Selectable(Parse.Cache.vMemoryInfo[Row].Content.c_str()); }
                    else if (Column == 5) { ImGui::Selectable(Parse.Cache.vMemoryInfo[Row].szType.c_str()); }
                    else if (Column == 6) { ImGui::Selectable(Parse.Cache.vMemoryInfo[Row].szProtect.c_str()); }
                    else if (Column == 7) { ImGui::Selectable(Parse.Cache.vMemoryInfo[Row].szInitialProtect.c_str()); }
                    ImGui::PopStyleColor(2);
                }
            }
        }
        ImGui::EndTable();
        Active->store(false);
    }
}

void MemoryMapTab_::FindInMemoryMapWindowRender()
{
    static float FindInMemoryMapVerticalSplitterRelPosX = 0.15f;
    static int StartPosX = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        FindInMemoryMapVerticalSplitterRelPosX,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::FindInMemoryMapWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.FindInMemoryMapVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        FindInMemoryMapVerticalSplitterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.InputFieldInFindMemoryMapWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::InputFieldInFindMemoryMapWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.ComboBoxOfTypesWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::ComboBoxOfTypesInFindMemoryMapWindowCode,
        *this,
        true);
}

void MemoryMapTab_::InputFieldInFindInMemoryMapWindowRender()
{
    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::InputFieldInFindMemoryMapWindowCode);

    ImVec2 TextSize = ImGui::CalcTextSize(Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.InputFieldInFindMemoryMapWindowName.data());

    static std::vector<char> InputFindBuffer(256);

    ImGui::PushItemWidth(WindowGeneralSizes.LeftSize.x);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
    ImGui::InputText(
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.FindInMemoryMap.InputFieldInFindMemoryMapWindowName.data(),
        InputFindBuffer.data(),
        InputFindBuffer.size());
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();
}

void MemoryMapTab_::ComboBoxOfTypesInFindInMemoryMapWindowRender()
{
    const std::vector<std::string_view> Items
    {
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableAddressColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableSizeColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTablePartyColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableInfoColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableContentColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableTypeColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableProtectionColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.MemoryMapTab.MemoryMapTableInitialColumnName
    };

    static int CurrentTypeId = 4;

    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });

    if (ImGui::BeginCombo("Type", Items[CurrentTypeId].data(), ImGuiComboFlags_NoArrowButton))
    {
        for (int n = 0; n < Items.size(); n++)
        {
            const bool is_selected = (CurrentTypeId == n);
            if (ImGui::Selectable(Items[n].data(), is_selected))
                CurrentTypeId = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::PopStyleColor();
}