#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

namespace GlobalVarsOfPeTab {
    extern PEInformation objPEInformation;
    extern PeReader objPeReader;
}

void PETab_::PeFileInfoInternalWindowRender()
{
    static float PEVerticalSpliiterRelPosX = 0.9f;
    static int StartPosX = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        PEVerticalSpliiterRelPosX,
        Dimensionses.SplitterIsDisabled,
        WindowCodes::PEFileInfoInternalWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PEHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        PEVerticalSpliiterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PENodeLeftWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::PEFileInfoLeftWindowCode,
        *this,
        false);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.PENodeRightWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::PEFileInfoRightWindowCode,
        *this,
        true);
}

void PETab_::PeFileInfoInternalWindow2Render()
{
    static float PEVerticalSplitterRelPosX = 0.5f;
    static float PEHorizontalSplitterRelPosY = 0.5f;
    static int StartPosX = 0;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        PEVerticalSplitterRelPosX,
        PEHorizontalSplitterRelPosY,
        WindowCodes::PEFileInfoLeftWindowCode);

    ImVec2 posMax{};
    posMax.x += WindowGeneralSizes.pos.x + WindowGeneralSizes.size.x;
    posMax.y += WindowGeneralSizes.pos.y + WindowGeneralSizes.size.y;
    if (Window.SplitterVertical(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PEVerticalSplitterName.data(),
        &WindowGeneralSizes.SplitterVPosX,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.pos,
        posMax,
        &StartPosX))
    {
        PEVerticalSplitterRelPosX =
            ((float)WindowGeneralSizes.SplitterVPosX + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.pos.x) / WindowGeneralSizes.size.x;
    }

    ImVec2 posMax2{};
    posMax2.x += WindowGeneralSizes.RightPos.x + WindowGeneralSizes.RightSize.x;
    posMax2.y += WindowGeneralSizes.RightPos.y + WindowGeneralSizes.RightSize.y;
    if (Window.SplitterHorizontal(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PEHorizontalSplitterName.data(),
        &WindowGeneralSizes.SplitterHPosY,
        Dimensionses.SplitterSize,
        WindowGeneralSizes.LeftPos,
        posMax2,
        &StartPosY))
    {
        PEHorizontalSplitterRelPosY =
            ((float)WindowGeneralSizes.SplitterHPosY + 0.5f * (float)Dimensionses.SplitterSize - WindowGeneralSizes.RightPos.y) / WindowGeneralSizes.RightSize.y;
    }

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.PEFileInfoWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::PEFileInfoWindowCode,
        *this,
        true);
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.PEFileInfoHashInfoWindowName.data(),
        WindowGeneralSizes.RightTopPosition,
        WindowGeneralSizes.RightTopSize,
        WindowCodes::PEFileInfoHashInfoWindowCode,
        *this,
        true);
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.PEFileInfoProtectionInfoWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::PEFileInfoProtectionInfoWindowCode,
        *this,
        true);
}

void PETab_::PeFileInfoInternalWindow3Render()
{
    static float PEHorizontalSplitterRelPosY = 0.5f;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        PEHorizontalSplitterRelPosY,
        WindowCodes::PEFileInfoRightWindowCode);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.DefaultButtonsWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::PEFileInfotTopRightWindowCode,
        *this,
        false);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.RightProtectionButtonsRightWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::PEFileInfoBottomRightWindowCode,
        *this,
        false);
}

void PETab_::PeFileInfoDefaultButtonsRightWindowRender()
{
    auto Size = ImGui::GetContentRegionAvail();
    auto FullWindowButton = ImVec2(Size.x, 0);
    if (ImGui::Button("Hash", FullWindowButton))
    {

    }
    if (ImGui::Button("Entropy", FullWindowButton))
    {

    }
    if (ImGui::Button("Signatures", FullWindowButton))
    {

    }
    if (ImGui::Button("Engine", FullWindowButton))
    {

    }
    if (ImGui::Button("DB", FullWindowButton))
    {

    }
}

void PETab_::PeFileInfoProtectionButtonsRightWindowRender()
{
    auto Size = ImGui::GetContentRegionAvail();
    auto FullWindowButton = ImVec2(Size.x, 0);
    if (ImGui::Button("Deep Scan", FullWindowButton))
    {

    }
    if (ImGui::Button("Heuristic\n\t Scan", FullWindowButton))
    {

    }
    if (ImGui::Button("Recursive\n\t Scan", FullWindowButton))
    {

    }
    if (ImGui::Button("Verbose", FullWindowButton))
    {

    }
    if (ImGui::Button("Scan", FullWindowButton))
    {

    }
}

void PETab_::PeFileInfoProtectionButtonsWindowRender()
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, Dimensionses.U32BlackGrayColor);

    static std::string InputEntryPointBuffer = "";
    ImGui::PushItemWidth(Dimensionses.SizeOfX64Addr);
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##1",
        InputEntryPointBuffer.data(),
        InputEntryPointBuffer.size(),
        ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputEntryPointBuffer);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.EntryPointFieldName.data());

    ImGui::SameLine(Dimensionses.SizeOfX64Addr + Dimensionses.SizeOfX64Addr);

    static std::string InputSectionsBuffer = "6 ";
    ImGui::PushItemWidth(Dimensionses.SizeOfSections);
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##2",
        InputSectionsBuffer.data(),
        InputSectionsBuffer.size(),
        ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_ReadOnly,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputSectionsBuffer);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.SectionsFieldName.data());

    static std::string InputBaseAddressBuffer = "";
    ImGui::PushItemWidth(Dimensionses.SizeOfX64Addr);
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##3",
        InputBaseAddressBuffer.data(),
        InputBaseAddressBuffer.size(),
        ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputBaseAddressBuffer);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.BaseAddressFieldName.data());

    ImGui::SameLine(Dimensionses.SizeOfX64Addr + Dimensionses.SizeOfX64Addr);

    static std::string InputEndiannessBuffer = "LE ";
    ImGui::PushItemWidth(Dimensionses.SizeOfEndianness);
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##4",
        InputEndiannessBuffer.data(),
        InputEndiannessBuffer.size(),
        ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_ReadOnly,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputEndiannessBuffer);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.EndiannessFieldName.data());

    ImGui::SameLine(Dimensionses.SizeOfX64Addr + Dimensionses.SizeOfX64Addr + Dimensionses.SizeOfX64Addr);

    static std::string InputUITypeBuffer = "GUI ";
    ImGui::PushItemWidth(Dimensionses.SizeOfUIType);
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##5",
        InputUITypeBuffer.data(),
        InputUITypeBuffer.size(),
        ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_ReadOnly,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputUITypeBuffer);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.UITypeFieldName.data());

    const std::vector<std::string> Engines = { "Automatic", "Nauz File Detector(NFD)", "Detect It Easy(DIE)", "PeID", "ExeInfo" };
    static int ItemCurrentId = 0;
    if (ImGui::BeginCombo(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.ScanComboBoxName.data(),
        Engines[ItemCurrentId].c_str(),
        ImGuiComboFlags_HeightLargest | ImGuiComboFlags_WidthFitPreview | ImGuiComboFlags_NoArrowButton))
    {
        for (unsigned char ItemId = 0; ItemId < Engines.size(); ItemId++)
        {
            const bool IsSelected = (ItemCurrentId == ItemId);
            if (ImGui::Selectable(Engines[ItemId].c_str(), IsSelected)) { ItemCurrentId = ItemId; }

            if (IsSelected) { ImGui::SetItemDefaultFocus(); }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.EngineComboBoxField.data());

    ImGui::PopStyleColor();
}

void PETab_::PeFileInfoProtectionInfoWindowRender()
{
    static float PEHorizontalSplitterRelPosY = 0.2f;
    static int StartPosY = 0;

    Dimensionses_::WindowGeneralSizes WindowGeneralSizes(
        Dimensionses.TitleBarHeight,
        Dimensionses.SplitterSize,
        Dimensionses.SplitterIsDisabled,
        PEHorizontalSplitterRelPosY,
        WindowCodes::PEFileInfoProtectionInfoWindowCode);

    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.ProtectionButtonsWindowName.data(),
        WindowGeneralSizes.LeftTopPosition,
        WindowGeneralSizes.LeftTopSize,
        WindowCodes::PEFileInfoProtectionButtonsWindowCode,
        *this,
        false);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4{ 0.2,0.2,0.2,0.2 });
    Window.ChildWindowRender(
        Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.ProtectionInfoWindow.ProtectionFieldWindowName.data(),
        WindowGeneralSizes.CenterBottomPosition,
        WindowGeneralSizes.CenterBottomSize,
        WindowCodes::PEFileInfoProtectionFieldWindowCode,
        *this,
        true);
    ImGui::PopStyleColor();
}

void PETab_::PeFileInfoHashInfoWindowRender()
{
    ImGui::PushStyleColor(ImGuiCol_FrameBg, Dimensionses.U32BlackGrayColor);

    //TODO: IF PE IS x32

    static std::string InputCRC32Buffer = "CRC32 Result ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##2",
        InputCRC32Buffer.data(),
        InputCRC32Buffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputCRC32Buffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.HashInfoWindow.CRC32FieldName.data());

    //TODO: IF PE IS x64

    static std::string InputCRC64Buffer = "CRC64 Result ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##2",
        InputCRC64Buffer.data(),
        InputCRC64Buffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputCRC64Buffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.HashInfoWindow.CRC64FieldName.data());

    ImGui::Separator();

    static std::string InputMD2Buffer = "MD2 Result ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##3",
        InputMD2Buffer.data(),
        InputMD2Buffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputMD2Buffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.HashInfoWindow.MD2FieldName.data());

    static std::string InputMD4Buffer = "MD4 Result ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##4",
        InputMD4Buffer.data(),
        InputMD4Buffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputMD4Buffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.HashInfoWindow.MD4FieldName.data());

    static std::string InputMD5Buffer = "MD5 Result ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##5",
        InputMD5Buffer.data(),
        InputMD5Buffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputMD5Buffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.HashInfoWindow.MD5FieldName.data());

    ImGui::Separator();

    static std::string InputSHA1Buffer = "SHA-1 Result ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##8",
        InputSHA1Buffer.data(),
        InputSHA1Buffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputSHA1Buffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.HashInfoWindow.SHA1FieldName.data());

    static std::string InputSHA2Buffer = "SHA-2 Result ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##9",
        InputSHA2Buffer.data(),
        InputSHA2Buffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputSHA2Buffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.HashInfoWindow.SHA2FieldName.data());

    ImGui::PopStyleColor();
}

void PETab_::PeFileInfoOfInfoWindowRender()
{
    static std::string InputFileNameBuffer = "FileName ";
    ImGui::PushStyleColor(ImGuiCol_FrameBg, Dimensionses.U32BlackGrayColor);

    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##1",
        InputFileNameBuffer.data(),
        InputFileNameBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileNameBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileNameFieldName.data());

    ImGui::Separator();

    static std::string InputFileTypeBuffer = "Type ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##2",
        InputFileTypeBuffer.data(),
        InputFileTypeBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileTypeBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileTypeFieldName.data());

    static std::string InputFileCreatedBuffer = "Created ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##3",
        InputFileCreatedBuffer.data(),
        InputFileCreatedBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileCreatedBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileCreatedFieldName.data());

    static std::string InputFileModifiedBuffer = "Modified ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##4",
        InputFileModifiedBuffer.data(),
        InputFileModifiedBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileModifiedBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileModifiedFieldName.data());

    static std::string InputFileAccessedBuffer = "Accessed ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##5",
        InputFileAccessedBuffer.data(),
        InputFileAccessedBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileAccessedBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileAccessedFieldName.data());

    ImGui::Separator();

    static std::string InputFileCompanyBufferBuffer = "Company Name ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##6",
        InputFileCompanyBufferBuffer.data(),
        InputFileCompanyBufferBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileCompanyBufferBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.CompanyNameFieldName.data());

    static std::string InputFileDescriptionBuffer = "File Description ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##7",
        InputFileDescriptionBuffer.data(),
        InputFileDescriptionBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileDescriptionBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileDescriptionFieldName.data());

    static std::string InputFileVersionBuffer = "File Version ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##8",
        InputFileVersionBuffer.data(),
        InputFileVersionBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileVersionBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileVersionFieldName.data());

    static std::string InputFileInternalNameBuffer = "Internal Name ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##9",
        InputFileInternalNameBuffer.data(),
        InputFileInternalNameBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputFileInternalNameBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileInternalNameFieldName.data());

    static std::string InputLegalCopyrightNameBuffer = "Legal Copyright ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##10",
        InputLegalCopyrightNameBuffer.data(),
        InputLegalCopyrightNameBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputLegalCopyrightNameBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.FileLegalCopyrightFieldName.data());

    static std::string InputOriginalFilenameNameBuffer = "Original Filename ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##11",
        InputOriginalFilenameNameBuffer.data(),
        InputOriginalFilenameNameBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputOriginalFilenameNameBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.OriginalFileNameFieldName.data());

    static std::string InputProductFilenameNameBuffer = "Product Filename ";
    ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
    ImGui::InputText(
        "##12",
        InputProductFilenameNameBuffer.data(),
        InputProductFilenameNameBuffer.size(),
        ImGuiInputTextFlags_CallbackResize,
        reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
        &InputProductFilenameNameBuffer);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::Text(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.FileInfo.FileInfoWindow.ProductFileNameFieldName.data());

    ImGui::PopStyleColor();
}