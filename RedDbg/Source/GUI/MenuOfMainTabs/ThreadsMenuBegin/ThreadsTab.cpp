#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/ThreadsTab.hpp"

#include <vector>

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

static void CallBackUpdateThreadCache(std::shared_ptr<ThreadRefreshThread> objThreadRefreshThread)
{
    objThreadRefreshThread->Parse->UpdateThreadCache(objThreadRefreshThread->Active);
}

void ThreadsTab_::GetThreadInfoSafe(ThreadParser_& Parse, std::shared_ptr<std::atomic<bool>> Active)
{
    ThreadRefreshThread objThreadRefreshThread;
    objThreadRefreshThread.Parse = &Parse;
    objThreadRefreshThread.Active = Active;
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    if (duration_cast<std::chrono::seconds>(now - Parse.Cache.LastUpdated) > std::chrono::milliseconds(Dimensionses.CacheRefreshRate))
    {
        std::thread updateThread(CallBackUpdateThreadCache, std::make_shared<ThreadRefreshThread>(objThreadRefreshThread)); updateThread.detach();
    }
    return;
}

void ThreadsTab_::ThreadsTableRender()
{
    if (CustomTitleBarGlobalVars::Pid != 0 && 
        ImGui::BeginTable(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableName.data(), Dimensionses.CountOfThreadsColumns, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableNumberColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableIDColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableEntryColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableTEBColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableStackBaseColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableStackLimitColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableRIPColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableSuspendCountColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTablePriorityColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableWaitReasonColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableLastErrorColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableUserTimeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableKernelTimeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableCreationTimeColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableCPUCyclesColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableContextSwitchColumnName.data(), ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableNameColumnName.data(), ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableDisabledColumnName.data(), ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();

        static ThreadParser_ Parse;
        static std::shared_ptr<std::atomic<bool>> Active = std::make_shared<std::atomic<bool>>(true);
        GetThreadInfoSafe(Parse, Active);
        ImGuiListClipper clipper;
        clipper.Begin(Parse.Cache.vThreadInfo.size(), 17);//TODO: Data transfer
        while (clipper.Step())
        {
            for (int Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
                for (int Column = 0; Column < Dimensionses.CountOfThreadsColumns; ++Column)
                {
                    Window.SelectAllAndColored(Dimensionses.CountOfThreadsColumns, Dimensionses.U32GrayColor);

                    ImGui::TableSetColumnIndex(Column);
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32GrayColor);
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32GrayColor);
                   
                    switch (Column)
                    {
                    case 0:
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Row;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 1:
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Thread.ThreadInfo.ClientId.UniqueThread;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 2: 
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Thread.Win32StartAddress;
                        ImGui::Selectable(Ss.str().c_str());
                        if(ImGui::IsItemHovered() && ImGui::BeginTooltip())
                        {
                            std::stringstream Ss1;
                            Ss1 << "Created on: " << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Thread.ThreadInfo.StartAddress;
                            ImGui::Text(Ss1.str().c_str());
                            ImGui::EndTooltip();
                        }
                        break;
                    }
                    case 3:
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].TebBase;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 4:
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Thread.StackBase;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 5:
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Thread.StackLimit;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 6:
                    {
                        //std::stringstream Ss;
                        //Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Thread.StackLimit;
                        //ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 7: 
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].SuspendCount;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 8:
                    {
                        ImGui::Selectable(Parse.Cache.vThreadInfo[Row].szPriority.c_str());
                        break;
                    }
                    case 9:
                    {
                        ImGui::Selectable(Parse.Cache.vThreadInfo[Row].szWaitReason.c_str());
                        break;
                    }
                    case 10:
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Thread.TebBase->LastErrorValue;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 11:
                    {
                        ImGui::Selectable(Parse.Cache.vThreadInfo[Row].szUserTime.c_str());
                        break;
                    }
                    case 12:
                    {
                        ImGui::Selectable(Parse.Cache.vThreadInfo[Row].szKernelTime.c_str());
                        break;
                    }
                    case 13:
                    {
                        ImGui::Selectable(Parse.Cache.vThreadInfo[Row].szCreationTime.c_str());
                        break;
                    }
                    case 14:
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Cycles;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 15:
                    {
                        std::stringstream Ss;
                        Ss << std::uppercase << std::hex << Parse.Cache.vThreadInfo[Row].Thread.ThreadInfo.ContextSwitches;
                        ImGui::Selectable(Ss.str().c_str());
                        break;
                    }
                    case 16:
                    {
                        ImGui::Selectable(Parse.Cache.vThreadInfo[Row].Name.c_str());
                        break;
                    }
                    }

                    ImGui::PopStyleColor(2);
                }
            }
        }
        ImGui::EndTable();
    }
}

void ThreadsTab_::ComboBoxOfTypesInFindInThreadsWindowRender()
{
    const std::vector<std::string_view> Items
    { 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableNumberColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableIDColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableEntryColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableTEBColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableRIPColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableSuspendCountColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTablePriorityColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableWaitReasonColumnName,
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableLastErrorColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableUserTimeColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableKernelTimeColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableCreationTimeColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableCPUCyclesColumnName, 
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsTableNameColumnName 
    };

    static int CurrentTypeId = 13;

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

void ThreadsTab_::InputFieldInFindInThreadsWindowRender()
{
    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::InputFieldInFindThreadsWindowCode);

    ImVec2 TextSize = ImGui::CalcTextSize(Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.FindInThreads.InputFieldInFindThreadsWindowName.data());

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

void ThreadsTab_::FindInThreadsWindowRender()
{
    static float FindInThreadsVerticalSplitterRelPosX = 0.15f;
    static int StartPosX = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        FindInThreadsVerticalSplitterRelPosX,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::FindInThreadsWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.FindInThreads.FindInThreadsVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        FindInThreadsVerticalSplitterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.FindInThreads.InputFieldInFindThreadsWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::InputFieldInFindThreadsWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.FindInThreads.ComboBoxOfTypesWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::ComboBoxOfTypesInFindThreadsWindowCode,
        *this,
        true);
}

void ThreadsTab_::ThreadsWindowRender()
{
    static float ThreadsHorizontalSplitterRelPosY = 0.95f;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        ThreadsHorizontalSplitterRelPosY,
        WindowCodes::ThreadsTabWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.RightPos.x + WindowGeneralSizes.RightSize.x;
    posMax.y += WindowGeneralSizes.RightPos.y + WindowGeneralSizes.RightSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos,
        posMax,
        &StartPosY))
    {
        ThreadsHorizontalSplitterRelPosY =
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.ThreadsWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::ThreadsWindowCode,
        *this,
        true);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.ThreadsTab.FindInThreadsWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::FindInThreadsWindowCode,
        *this,
        false);
}