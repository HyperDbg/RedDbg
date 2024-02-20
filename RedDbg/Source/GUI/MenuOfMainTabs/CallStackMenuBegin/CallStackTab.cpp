#include "GUI/MenuOfMainTabs/CallStackMenuBegin/CallStackTab.hpp"

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

void CallStackTab_::CallStackWindowRender()
{
    static float CallStackHorizontalSplitterRelPosY = 0.95f;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        CallStackHorizontalSplitterRelPosY,
        WindowCodes::CallStackTabWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.RightPos.x + WindowGeneralSizes.RightSize.x;
    posMax.y += WindowGeneralSizes.RightPos.y + WindowGeneralSizes.RightSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos,
        posMax,
        &StartPosY))
    {
        CallStackHorizontalSplitterRelPosY =
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::CallStackWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.FindInCallStackWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::FindInCallStackWindowCode,
        *this,
        false);
}

//static void CallBackUpdateCallStackCache(std::shared_ptr<CallStackRefreshThread> objCallStackRefreshThread)
//{
//    objCallStackRefreshThread->Parse->UpdateCallStackCache(objCallStackRefreshThread->Active);
//}

void CallStackTab_::GetCallStackInfoSafe(CallStackParser_& Parse, std::shared_ptr<std::atomic<bool>> Active)
{
    //CallStackRefreshThread objCallStackRefreshThread;
    //objCallStackRefreshThread.Parse = &Parse;
    //objCallStackRefreshThread.Active = Active;
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    if (duration_cast<std::chrono::seconds>(now - Parse.Cache.LastUpdated) > std::chrono::milliseconds(Dimensionses.CacheRefreshRate))
    {
        std::jthread updateThread = std::jthread([&Parse, Active]() {
            Parse.UpdateCallStackCache(Active);
            });

        updateThread.detach();
        //std::thread updateThread(CallBackUpdateCallStackCache, std::make_shared<CallStackRefreshThread>(objCallStackRefreshThread)); updateThread.detach();
    }
    return;
}

void CallStackTab_::CallStackTableRender()
{
    if (CustomTitleBarGlobalVars::Pid != 0 && 
        ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableName.data(), Dimensionses.CountOfCallStackColumns, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableThreadIDColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableAddressColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableToColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableFromColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableSizeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTablePartyColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableLabelColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        static CallStackParser_ Parse;
        static std::shared_ptr<std::atomic<bool>> Active = std::make_shared<std::atomic<bool>>(true);
        GetCallStackInfoSafe(Parse, Active);
        Active->store(true);
        ImGuiListClipper clipper;
        clipper.Begin(Parse.Cache.vCallStackInfo.size(), 17);//TODO: Data transfer
        while (clipper.Step())
        {
            for (int Row = clipper.DisplayStart; Row < clipper.DisplayEnd; Row++)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfCallStackColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfCallStackColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);
                    
                    if (Row > 0)
                    {
                        if (Parse.Cache.vCallStackInfo[Row - 1]->stack_frame.KdHelp.Thread != Parse.Cache.vCallStackInfo[Row]->stack_frame.KdHelp.Thread)
                        {
                            if (Column > 0) { ImGui::Separator(); }
                            else
                            {
                                std::stringstream Ss;
                                Ss << std::uppercase << std::hex << (DWORD)Parse.Cache.vCallStackInfo[Row]->stack_frame.KdHelp.Thread;
                                ImGui::Selectable(Ss.str().c_str());
                            }
                        }
                    }
                    else
                    {
                        if (Column > 0) { ImGui::Separator(); }
                        else
                        {
                            std::stringstream Ss;
                            Ss << std::uppercase << std::hex << (DWORD)Parse.Cache.vCallStackInfo[Row]->stack_frame.KdHelp.Thread;
                            ImGui::Selectable(Ss.str().c_str());
                        }
                    }
                    
                    if (Column == 1)
                    {
                        std::stringstream Ss;
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint64_t) * 2) << std::hex << Parse.Cache.vCallStackInfo[Row]->stack_frame.AddrFrame.Offset + sizeof(uint64_t);
                        }
                        else
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint32_t) * 2) << std::hex << Parse.Cache.vCallStackInfo[Row]->stack_frame.AddrFrame.Offset + sizeof(uint32_t);
                        }
                        ImGui::Selectable(Ss.str().c_str());
                    }
                    else if (Column == 2)
                    {
                        std::stringstream Ss;
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint64_t) * 2) << std::hex << Parse.Cache.vCallStackInfo[Row]->stack_frame.AddrReturn.Offset;
                        }
                        else
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint32_t) * 2) << std::hex << Parse.Cache.vCallStackInfo[Row]->stack_frame.AddrReturn.Offset;
                        }
                        ImGui::Selectable(Ss.str().c_str());
                    }
                    else if (Column == 3)
                    {
                        std::stringstream Ss;
                        if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint64_t) * 2) << std::hex << Parse.Cache.vCallStackInfo[Row]->stack_frame.AddrPC.Offset;
                        }
                        else
                        {
                            Ss << std::uppercase << std::setfill('0') <<
                                std::setw(sizeof(uint32_t) * 2) << std::hex << Parse.Cache.vCallStackInfo[Row]->stack_frame.AddrPC.Offset;
                        }
                        ImGui::Selectable(Ss.str().c_str());
                    }
                    else if (Column == 4)
                    {
                        if (Row > 0 && 
                            Parse.Cache.vCallStackInfo[Row - 1]->stack_frame.KdHelp.Thread == Parse.Cache.vCallStackInfo[Row]->stack_frame.KdHelp.Thread)
                        {
                            std::stringstream Ss;
                            if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                            {
                                Ss << std::uppercase << std::hex <<
                                    (Parse.Cache.vCallStackInfo[Row]->stack_frame.AddrFrame.Offset + sizeof(uint64_t)) -
                                    (Parse.Cache.vCallStackInfo[Row-1]->stack_frame.AddrFrame.Offset + sizeof(uint64_t));
                            }
                            else
                            {
                                Ss << std::uppercase << std::hex <<
                                    (Parse.Cache.vCallStackInfo[Row]->stack_frame.AddrFrame.Offset + sizeof(uint32_t)) -
                                    (Parse.Cache.vCallStackInfo[Row-1]->stack_frame.AddrFrame.Offset + sizeof(uint32_t));
                            }
                            ImGui::Selectable(Ss.str().c_str());
                        }
                    }
                    else if (Column == 5)
                    {

                    }
                    else if (Column == 6)
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vCallStackInfo[Row]->name;
                        ImGui::Selectable(Ss.str().c_str());
                    }
                    //if (Column > 0) { ImGui::Separator(); }
                    ImGui::PopStyleColor(2);
                }
            }
        }
        ImGui::EndTable();
        Active->store(false);
    }
}

void CallStackTab_::ComboBoxOfTypesInFindInCallStackWindowRender()
{
    const std::vector<std::string_view> Items
    {
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableThreadIDColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableToColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableFromColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableSizeColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTablePartyColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.CallStackTableLabelColumnName,
    };

    static int CurrentTypeId = 0;

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

void CallStackTab_::InputFieldInFindInCallStackWindowRender()
{
    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::InputFieldInFindCallStackWindowCode);

    ImVec2 TextSize = ImGui::CalcTextSize(Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.FindInCallStack.InputFieldInFindCallStackWindowName.data());

    static std::vector<char> InputFindBuffer(256);

    ImGui::PushItemWidth(WindowGeneralSizes.LeftSize.x);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4{ 0,0,0,0 });
    ImGui::InputText(
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.FindInThreads.InputFieldInFindThreadsWindowName.data(),
        InputFindBuffer.data(),
        InputFindBuffer.size());
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();
}

void CallStackTab_::FindInCallStackWindowRender()
{
    static float FindInCallStackVerticalSplitterRelPosX = 0.1f;
    static int StartPosX = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        FindInCallStackVerticalSplitterRelPosX,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::FindInCallStackWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.FindInCallStack.FindInCallStackVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        FindInCallStackVerticalSplitterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.FindInCallStack.InputFieldInFindCallStackWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::InputFieldInFindCallStackWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.CallStackTab.FindInCallStack.ComboBoxOfTypesWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::ComboBoxOfTypesInFindCallStackWindowCode,
        *this,
        true);
}