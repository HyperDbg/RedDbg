#include "GUI/UI/ImGui/imgui.h"

#include "GUI/CustomTitleBar.hpp"
#include "GUI/MenuOfMainTabs/CPUMenuBegin/CPUTab.hpp"
#include "GUI/MenuOfMainTabs/StackMapMenuBegin/StackMapTab.hpp"
#include "GUI/MenuOfMainTabs/ThreadsMenuBegin/ThreadsTab.hpp"
#include "GUI/MenuOfMainTabs/CallStackMenuBegin/CallStackTab.hpp"
#include "GUI/MenuOfMainTabs/MemoryMapMenuBegin/MemoryMapTab.hpp"
#include "GUI/MenuOfMainTabs/SEHMenuBegin/SehTab.hpp"
#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"
#include "GUI/Window.hpp"

#include <string_view>

bool Window_::SplitterHorizontal(const std::string_view id, int* posX, const int width, const ImVec2& posMin, const ImVec2& posMax, static int* startPosX)
{
    if ((posX == nullptr) || (posMin.x >= posMax.x) || (posMin.y >= posMax.y)) { return false; }

    ImGui::SetCursorPos(ImVec2(posMin.x, (float)*posX));
    ImGui::InvisibleButton(id.data(), ImVec2(posMax.x - posMin.x, width));

    if (ImGui::IsItemHovered() || ImGui::IsItemActive()) { ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS); }

    int lastPosX = *posX;

    if (ImGui::IsItemActive())
    {
        int move = (int)ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).y;
        if (move != 0)
        {
            *posX = *startPosX + move;

            if (*posX < (int)posMin.y) { *posX = (int)posMin.y; }
            if (*posX > (int)posMax.y - width) { *posX = (int)posMax.y - width; }
        }
    }
    else { *startPosX = *posX; }

    return *posX != lastPosX;
}

bool Window_::SplitterVertical(const std::string_view id, int* posY, const int height, const ImVec2& posMin, const ImVec2& posMax, static int* startPosY)
{
    if ((posY == nullptr) || (posMin.x >= posMax.x) || (posMin.y >= posMax.y)) { return false; }

    ImGui::SetCursorPos(ImVec2((float)*posY, posMin.y));
    ImGui::InvisibleButton(id.data(), ImVec2((float)height, posMax.y - posMin.y));

    if (ImGui::IsItemHovered() || ImGui::IsItemActive()) { ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW); }

    int lastPosY = *posY;

    if (ImGui::IsItemActive())
    {
        int move = (int)ImGui::GetMouseDragDelta(ImGuiMouseButton_Left).x;
        if (move != 0)
        {
            *posY = *startPosY + move;

            if (*posY < (int)posMin.x) { *posY = (int)posMin.x; }
            if (*posY > (int)posMax.x - height) { *posY = (int)posMax.x - height; }
        }
    }
    else { *startPosY = *posY; }

    return *posY != lastPosY;
}

void Window_::ChildWindowDispatcher(const WindowCodes Code, std::any ObjOfClass)
{
    switch (Code)
    {
    //TitleBar
    {
        case MenuBarOfTitleBarWindowCode:
        {
            CustomTitleBar_ MethodsFromMainClass = std::any_cast<CustomTitleBar_&>(ObjOfClass);

            return MethodsFromMainClass.CustomTitleBarMenuRender();
        }
    }
    //CPUTab
    {
        case DisasmAndInfoWindowCode: __fallthrough;
        case RegisterAndCallConventionWindowCode: __fallthrough;
        case StackAndDumpWindowCode: __fallthrough;
        case StackWindowCode: __fallthrough;
        case DumpWindowCode: __fallthrough;
        case RegisterWindowCode: __fallthrough;
        case CallConventionWindowCode: __fallthrough;
        case DisassemblyWindowCode: __fallthrough;
        case InfoWindowCode:
        {
            CPUTab_ MethodsFromMainClass = std::any_cast<CPUTab_&>(ObjOfClass);

            switch (Code)
            {
            case DisasmAndInfoWindowCode: return MethodsFromMainClass.CPUDisasmAndInfoWindowRender();
            case RegisterAndCallConventionWindowCode: return MethodsFromMainClass.CPURegisterAndCallConventionWindowRender();
            case StackAndDumpWindowCode: return MethodsFromMainClass.StackAndDumpWindowRender();
            case StackWindowCode: return MethodsFromMainClass.StackTableRender();
            case DumpWindowCode: return;
            case RegisterWindowCode: return;
            case CallConventionWindowCode: return;
            case DisassemblyWindowCode: return MethodsFromMainClass.DisassemblyTableRender();
            case InfoWindowCode: return;
            }
        }
    }
    //StackMapTab
    {
        case StackMapAddressesWindowCode: __fallthrough;
        case StackMapReferencesToAddressesWindowCode:
        {
            StackMapTab_ MethodsFromMainClass = std::any_cast<StackMapTab_&>(ObjOfClass);

            switch (Code)
            {
            case StackMapAddressesWindowCode: return MethodsFromMainClass.StackMapAddressTableRender();
            case StackMapReferencesToAddressesWindowCode: return MethodsFromMainClass.StackMapReferncesTableRender();
            }
        }
    }

    //ThreadsTab
    {
        case ThreadsWindowCode: __fallthrough;
        case FindInThreadsWindowCode: __fallthrough;
        case InputFieldInFindThreadsWindowCode: __fallthrough;
        case ComboBoxOfTypesInFindThreadsWindowCode:
        {
            ThreadsTab_ MethodsFromMainClass = std::any_cast<ThreadsTab_&>(ObjOfClass);

            switch (Code)
            {
            case ThreadsWindowCode: return MethodsFromMainClass.ThreadsTableRender();
            case FindInThreadsWindowCode: return MethodsFromMainClass.FindInThreadsWindowRender();
            case InputFieldInFindThreadsWindowCode: return MethodsFromMainClass.InputFieldInFindInThreadsWindowRender();
            case ComboBoxOfTypesInFindThreadsWindowCode: return MethodsFromMainClass.ComboBoxOfTypesInFindInThreadsWindowRender();
            }
        }
    }
    //CallStackTab
    {
        case CallStackWindowCode: __fallthrough;
        case FindInCallStackWindowCode: __fallthrough;
        case InputFieldInFindCallStackWindowCode: __fallthrough;
        case ComboBoxOfTypesInFindCallStackWindowCode:
        {
            CallStackTab_ MethodsFromMainClass = std::any_cast<CallStackTab_&>(ObjOfClass);

            switch (Code)
            {
            case CallStackWindowCode: return MethodsFromMainClass.CallStackTableRender();
            case FindInCallStackWindowCode: return MethodsFromMainClass.FindInCallStackWindowRender();
            case InputFieldInFindCallStackWindowCode: return MethodsFromMainClass.InputFieldInFindInCallStackWindowRender();
            case ComboBoxOfTypesInFindCallStackWindowCode: return MethodsFromMainClass.ComboBoxOfTypesInFindInCallStackWindowRender();
            }
        }
    }
    //MemoryMapTab
    {
        case MemoryMapWindowCode: __fallthrough;
        case FindInMemoryMapWindowCode: __fallthrough;
        case InputFieldInFindMemoryMapWindowCode: __fallthrough;
        case ComboBoxOfTypesInFindMemoryMapWindowCode: 
        {
            MemoryMapTab_ MethodsFromMainClass = std::any_cast<MemoryMapTab_&>(ObjOfClass);

            switch (Code)
            {
            case MemoryMapWindowCode: return MethodsFromMainClass.MemoryMapTableRender();
            case FindInMemoryMapWindowCode: return MethodsFromMainClass.FindInMemoryMapWindowRender();
            case InputFieldInFindMemoryMapWindowCode: return MethodsFromMainClass.InputFieldInFindInMemoryMapWindowRender();
            case ComboBoxOfTypesInFindMemoryMapWindowCode: return MethodsFromMainClass.ComboBoxOfTypesInFindInMemoryMapWindowRender();
            }
        }
    }
    //SehTab
    {
        case SEHWindowCode: __fallthrough;
        case FindInSEHWindowCode: __fallthrough;
        case InputFieldInFindSEHWindowCode: __fallthrough;
        case ComboBoxOfTypesInFindSEHWindowCode: 
        {
            SEHTab_ MethodsFromMainClass = std::any_cast<SEHTab_&>(ObjOfClass);

            switch (Code)
            {
            case SEHWindowCode: return MethodsFromMainClass.SEHTableRender();
            case FindInSEHWindowCode: return MethodsFromMainClass.FindInSEHWindowRender();
            case InputFieldInFindSEHWindowCode: return MethodsFromMainClass.InputFieldInFindInSEHWindowRender();
            case ComboBoxOfTypesInFindSEHWindowCode: return MethodsFromMainClass.ComboBoxOfTypesInFindInSEHWindowRender();
            }
        }
    }
    //PETab
    {
        //case PEWindowCode: __fallthrough;
        case PETreeNodeWindowCode: __fallthrough;
        case PEFileInfoNodeCode: __fallthrough;
        case PEFileInfoInternalWindowCode: __fallthrough;
        case PEDoSHeaderNodeCode: __fallthrough;
        case PERichHeaderNodeCode: __fallthrough;
        case PENtHeaderNodeCode: __fallthrough;
        case PEFileHeaderNodeCode: __fallthrough;
        case PEOptionalHeaderNodeCode: __fallthrough; 
        case PEDataDirectoriesNodeCode: __fallthrough;
        case PESectionHeaderNodeCode: __fallthrough; 
        case PeSectionHeaderTableNodeCode: __fallthrough;
        case PeInfoOfSectionInHeaderWindowCode: __fallthrough;
        case PEImportDirectoryNodeCode: __fallthrough;
        case PeImportDirectoryTableNodeCode: __fallthrough;
        case PeInfoOfImportDllWindowCode: __fallthrough;
        case PEResourceDirectoryNodeCode: __fallthrough;
        case PEExceptionDirectoryNodeCode: __fallthrough;
        case PEDependencyWalkerNodeCode: __fallthrough;
        case PEImportAdderNodeCode: __fallthrough;
        case PEStructRebuilderWalkerNodeCode: __fallthrough;

        case PEFileInfoLeftWindowCode: __fallthrough;
        case PEFileInfoRightWindowCode: __fallthrough;
        case PEFileInfotTopRightWindowCode: __fallthrough;
        case PEFileInfoBottomRightWindowCode: __fallthrough;
        case PEFileInfoWindowCode: __fallthrough;
        case PEFileInfoHashInfoWindowCode: __fallthrough;
        case PEFileInfoProtectionInfoWindowCode: __fallthrough;
        case PEFileInfoProtectionButtonsWindowCode:
        {
            PETab_ MethodsFromMainClass = std::any_cast<PETab_&>(ObjOfClass);

            switch (Code)
            {

            case PEDoSHeaderNodeCode: return MethodsFromMainClass.PeDoSHeaderTableRender();
            case PERichHeaderNodeCode: return MethodsFromMainClass.PeRichHeaderTableRender();
            case PENtHeaderNodeCode: return MethodsFromMainClass.PeNtHeaderTableRender();
            case PEFileHeaderNodeCode: return MethodsFromMainClass.PeFileHeaderTableRender();
            case PEOptionalHeaderNodeCode: return MethodsFromMainClass.PeOptionalHeaderTableRender();
            case PEDataDirectoriesNodeCode: return MethodsFromMainClass.PeDataDirectoriesTableRender();
            case PESectionHeaderNodeCode: return MethodsFromMainClass.PeSectionHeaderInternalWindowRender();
            case PeSectionHeaderTableNodeCode: return MethodsFromMainClass.PeSectionHeaderTableRender();
            case PeInfoOfSectionInHeaderWindowCode: return MethodsFromMainClass.PeInfoOfSectionInSectionHeaderWindowRender();
            case PEImportDirectoryNodeCode: return MethodsFromMainClass.PeImportDirectoryInternalWindowRender();
            case PeImportDirectoryTableNodeCode: return MethodsFromMainClass.PeImportDirectoryTableRender();
            case PeInfoOfImportDllWindowCode: return MethodsFromMainClass.PeInfoOfImportDllWindowRender();

            case PETreeNodeWindowCode: return MethodsFromMainClass.PeTreeNodeRender();
            case PEFileInfoNodeCode: return MethodsFromMainClass.PeFileInfoInternalWindowRender();
            case PEFileInfoLeftWindowCode: return MethodsFromMainClass.PeFileInfoInternalWindow2Render();
            case PEFileInfoRightWindowCode: return MethodsFromMainClass.PeFileInfoInternalWindow3Render();

            case PEFileInfotTopRightWindowCode: return MethodsFromMainClass.PeFileInfoDefaultButtonsRightWindowRender();
            case PEFileInfoBottomRightWindowCode: return MethodsFromMainClass.PeFileInfoProtectionButtonsRightWindowRender();


            case PEFileInfoWindowCode: return MethodsFromMainClass.PeFileInfoOfInfoWindowRender();
            case PEFileInfoHashInfoWindowCode: return MethodsFromMainClass.PeFileInfoHashInfoWindowRender();
            case PEFileInfoProtectionInfoWindowCode: return MethodsFromMainClass.PeFileInfoProtectionInfoWindowRender();
            case PEFileInfoProtectionButtonsWindowCode: return MethodsFromMainClass.PeFileInfoProtectionButtonsWindowRender();
            }
        }
    }
    }
}

void Window_::ClickOnTreeNodeDispatcher(const ClickCodesOfTreeNodes_ Code, std::vector<bool>* Nodes)
{
    for (unsigned char Size = 0; Size < Nodes->size(); ++Size)
    {
        if (Size == Code)
        {
            (*Nodes)[Size] = true;
        }
        else
        {
            (*Nodes)[Size] = false;
        }
    }
}

bool Window_::ClickOnTreeNode(const bool Open, const ClickCodesOfTreeNodes_ Code, std::vector<bool>* Nodes)
{
    if (!Open && ImGui::IsItemClicked(ImGuiMouseButton_::ImGuiMouseButton_Left) && !ImGui::IsItemToggledOpen()) 
    { ClickOnTreeNodeDispatcher(Code, Nodes); return true; }
    else if (Open) { return Open; }
    
    return false;
}

void Window_::SelectAllAndColored(const unsigned char CountOfColumns, const uint32_t U32Color)
{
    if (ImGui::IsItemHovered())
    {
        for (int SeletableColumn = 0; SeletableColumn < CountOfColumns; ++SeletableColumn)
        {
            ImGui::TableSetColumnIndex(SeletableColumn);
            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, U32Color, SeletableColumn);
        }
    }

    return;
}

void Window_::ChildWindowRender(
    const std::string_view id, 
    const ImVec2& pos, 
    const ImVec2& size, 
    const WindowCodes Code, 
    std::any ObjOfClass,
    const bool border, 
    const ImGuiWindowFlags WindowFlags)
{
    if ((size.x <= 0.0f) || (size.y <= 0.0f)) { return; }

    ImGui::SetCursorPos(pos);

    ImGui::BeginChild(id.data(), size, border, WindowFlags);

    ChildWindowDispatcher(Code, ObjOfClass);

    ImGui::EndChild();

    return;
}

int Callable::CallBackInputWrapper(void* Arg, void* Class)
{
    ImGuiInputTextCallbackData* Data = reinterpret_cast<ImGuiInputTextCallbackData*>(Arg);
    int Status = static_cast<Window_*>(Class)->InputWrapper(Data);
    return Status;
}

int Window_::InputWrapper(ImGuiInputTextCallbackData* Data)
{
    if (Data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        std::string* my_str = (std::string*)Data->UserData;
        IM_ASSERT(my_str->data() == Data->Buf);
        my_str->resize(Data->BufSize); // NB: On resizing calls, generally data->BufSize == data->BufTextLen + 1
        Data->Buf = my_str->data();
    }
    return 0;
}