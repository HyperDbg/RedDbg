#include "GUI/MenuOfMainTabs/CPUMenuBegin/HexTab.hpp"
#include "imgui_internal.h"
#include <optional>
#include <filesystem>
#include "GUI/UI/ImGui/imgui_memory_editor.h"
#include "GUI/MenuOfMainTabs/PeMenuBegin/Parser/PEReader.hpp"

namespace CustomTitleBarGlobalVars {
    extern HANDLE hDriver;
    extern DWORD Pid;
}

namespace GlobalVarsOfPeTab {
    extern std::shared_ptr<PeReader> objPEInformation;
}

constexpr static uint16_t getByteColumnSeparatorCount(uint16_t columnCount) {
    return (columnCount - 1) / 8;
}

constexpr static bool isColumnSeparatorColumn(uint16_t currColumn, uint16_t columnCount) {
    return currColumn > 0 && (currColumn) < columnCount && ((currColumn) % 8) == 0;
}

static auto getCellPosition() {
    float CellPosX = ImGui::GetCursorScreenPos().x - ImGui::GetStyle().CellPadding.x;
    float CellPosY = ImGui::GetCursorScreenPos().y - ImGui::GetStyle().CellPadding.y;
    return ImVec2{ CellPosX, CellPosY };
}

enum TableColumns
{
    Address,
    Reserved1,
    ByteStart = 2,
    ByteEnd = 17,
    Reserved2,
    ASCII_UNICODE,
};

void HexEditor::DrawEditor()
{
    //const uint8_t Addr = 1;
    //const uint8_t BytesPerRow = 16;
    //const uint8_t ASCII_UNICODE = 1;
    //auto AddrSize = ImGui::CalcTextSize("FFFFFFFFFFFFFFFF");
    //auto ByteSize = ImGui::CalcTextSize("FF");
    //
    //
    //
    //if (ImGui::BeginTable("asdasdasd", Addr + BytesPerRow + ASCII_UNICODE + 1, ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
    //    ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_SizingFixedFit))
    //{
    //    ImGui::TableSetupScrollFreeze(0, 1);
    //    
    //    ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_None, AddrSize.x);
    //    for (int RelativeByteIndex = 0, IndexByte = 0; IndexByte < BytesPerRow; ++IndexByte)
    //    {
    //        ImGui::TableSetupColumn(std::to_string(RelativeByteIndex).data(), ImGuiTableColumnFlags_None, ByteSize.x);
    //        ++RelativeByteIndex;
    //    }
    //
    //    ImGui::TableSetupColumn("ASCII", ImGuiTableColumnFlags_WidthFixed);
    //    ImGui::TableSetupColumn("##1", ImGuiTableColumnFlags_Disabled);
    //
    //    ImGui::TableHeadersRow();
    //
    //    ImGuiListClipper clipper;
    //    clipper.Begin(100, 17); //43534 is count. TODO: DataTransfer
    //    while (clipper.Step())
    //    {
    //        for (uint64_t Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
    //        {
    //            ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
    //            for (int Column = 0; Column < 18; ++Column)
    //            {
    //                ImGui::TableSetColumnIndex(Column);
    //
    //                if (Column == 0)
    //                {
    //                    ImGui::Selectable("FFFFFFFFFFFFFFFF:", false);
    //                }
    //                else if (Column >= 1 && Column <= BytesPerRow)
    //                {
    //                    ImGui::Selectable("FF", false);
    //                }
    //            }
    //        }
    //    }
    //
    //    ImGui::EndTable();
    //}
    //
    //return;


    //const float SeparatorColumWidth = 6;
    //const auto CharacterSize = ImGui::CalcTextSize("0");
    //
    //static uint8_t BytesPerCell = 1;
    //static uint8_t MaxCharsPerCell = 1;
    //static uint8_t m_bytesPerRow = 16;
    //static uint8_t m_byteCellPadding = 0;
    //static uint8_t m_characterCellPadding = 0;
    //static bool m_upperCaseHex = true;
    //static bool m_showAscii = true;
    //
    //const auto bytesPerCell = BytesPerCell;
    //const uint16_t columnCount = m_bytesPerRow / bytesPerCell;
    //auto byteColumnCount = 2 + columnCount + getByteColumnSeparatorCount(columnCount) + 2 + 2;
    //
    //if (/*CustomTitleBarGlobalVars::Pid != 0 &&*/ ImGui::BeginTable("##hex", byteColumnCount, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoKeepColumnsVisible | ImGuiTableFlags_ScrollY)) {
    //    ImGui::TableSetupScrollFreeze(0, 2);
    //
    //    // Row address column
    //    ImGui::TableSetupColumn("Address");
    //    ImGui::TableSetupColumn("");
    //
    //    for (uint16_t i = 0; i < columnCount; i++) {
    //        if (isColumnSeparatorColumn(i, columnCount))
    //            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, SeparatorColumWidth);
    //
    //        std::stringstream ss;
    //        ss << std::hex << std::uppercase << std::setfill('0') << std::setw(sizeof(char) * 2) << (i * bytesPerCell);
    //        std::string formattedString = ss.str();
    //        ImGui::TableSetupColumn(formattedString.c_str(), ImGuiTableColumnFlags_WidthFixed, CharacterSize.x * MaxCharsPerCell + (6 + m_byteCellPadding) * 1);
    //    }
    //
    //    // ASCII column
    //    ImGui::TableSetupColumn("");
    //
    //    if (m_showAscii) {
    //        ImGui::TableSetupColumn("ASCII", ImGuiTableColumnFlags_WidthFixed, (CharacterSize.x + m_characterCellPadding * 1) * m_bytesPerRow);
    //    }
    //    else {
    //        ImGui::TableSetupColumn("UNICODE", ImGuiTableColumnFlags_WidthFixed, (CharacterSize.x + m_characterCellPadding * 1) * m_bytesPerRow);
    //    }
    //
    //    ImGui::TableNextRow();
    //    for (uint32_t i = 0; i < ImGui::TableGetColumnCount(); ++i) {
    //        ImGui::TableNextColumn();
    //        ImGui::TextUnformatted(ImGui::TableGetColumnName(i));
    //        ImGui::Dummy(ImVec2(0, CharacterSize.y / 2));
    //    }
    //
    //    ImGui::TableNextRow();
    //    ImGui::TableNextColumn();
    //
    //    const int ITEMS_COUNT = 10000;
    //      
    //    static ImGuiSelectionBasicStorage selection;
    //
    //    ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape;
    //    ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(flags);
    //    selection.ApplyRequests(ms_io, ITEMS_COUNT);
    //
    //    ImGuiListClipper clipper;
    //    clipper.Begin(ITEMS_COUNT, 17); //43534 is count. TODO: DataTransfer
    //    if (ms_io->RangeSrcItem != -1)
    //    {
    //        clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem);
    //    }
    //
    //    while (clipper.Step())
    //    {
    //        for (uint64_t Row = clipper.DisplayStart; Row < clipper.DisplayEnd; ++Row)
    //        {
    //            ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
    //            for (int Column = 0; Column < byteColumnCount; ++Column)
    //            {
    //                ImGui::TableSetColumnIndex(Column);
    //    
    //                if (Column == TableColumns::Address)
    //                {
    //                    ImGui::Text("FFFFFFFFFFFFFFFF:");
    //                }
    //                else if (Column != 10 && Column >= TableColumns::ByteStart && Column <= m_bytesPerRow + TableColumns::ByteStart)
    //                {
    //                    bool item_is_selected = selection.Contains((ImGuiID)Row);
    //                    ImGui::SetNextItemSelectionUserData(Row);
    //                    ImGui::Selectable("FF", item_is_selected);
    //
    //                    //if (ImGui::IsItemHovered() && ImGui::GetIO().MouseDown[0]/*ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left)*/)
    //                    //{
    //                    //    printf("Hovered %X\n", Column);
    //                    //    Select[Row] ^= 1;
    //                    //}
    //                }
    //            }
    //        }
    //    }
    //    ms_io = ImGui::EndMultiSelect();
    //    selection.ApplyRequests(ms_io, ITEMS_COUNT);
    //
    //    ImGui::EndTable();
    //}

    //static ImGuiSelectionBasicStorage selection;
    //const int ITEMS_COUNT = 10000;
    //ImGui::Text("Selection: %d/%d", selection.Size, ITEMS_COUNT);
    //if (ImGui::BeginTable("##hex", byteColumnCount, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoKeepColumnsVisible | ImGuiTableFlags_ScrollY))
    //{
    //    ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_ClearOnEscape | ImGuiMultiSelectFlags_BoxSelect;
    //    ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(flags);
    //    selection.ApplyRequests(ms_io, ITEMS_COUNT);
    //
    //    ImGuiListClipper clipper;
    //    clipper.Begin(ITEMS_COUNT);
    //    if (ms_io->RangeSrcItem != -1)
    //        clipper.IncludeItemByIndex((int)ms_io->RangeSrcItem); // Ensure RangeSrc item is not clipped.
    //    while (clipper.Step())
    //    {
    //        for (int n = clipper.DisplayStart; n < clipper.DisplayEnd; n++)
    //        {
    //            ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
    //            for (int Column = 0; Column < byteColumnCount; ++Column)
    //            {
    //                ImGui::TableSetColumnIndex(Column);
    //
    //                if (Column == TableColumns::Address)
    //                {
    //                    ImGui::Text("FFFFFFFFFFFFFFFF:");
    //                }
    //                else if (Column != 10 && Column >= TableColumns::ByteStart && Column <= m_bytesPerRow + TableColumns::ByteStart)
    //                {
    //                    bool item_is_selected = selection.Contains((ImGuiID)n);
    //                    ImGui::SetNextItemSelectionUserData(n);
    //                    ImGui::Selectable("FF", item_is_selected);
    //                }
    //            }
    //            //bool item_is_selected = selection.Contains((ImGuiID)n);
    //            //ImGui::SetNextItemSelectionUserData(n);
    //            //ImGui::Selectable("FF", item_is_selected);
    //        }
    //    }
    //
    //    ms_io = ImGui::EndMultiSelect();
    //    selection.ApplyRequests(ms_io, ITEMS_COUNT);
    //
    //    ImGui::EndTable();
    //}

    if (CustomTitleBarGlobalVars::Pid != 0)
    {
        static MemoryEditor mem_edit; 
    
        DWORD Size = (GlobalVarsOfPeTab::objPEInformation->Pe->fileBuffer->bufLen + 4095) & ~4095;
    
        mem_edit.DrawContents(GlobalVarsOfPeTab::objPEInformation->Pe->fileBuffer->buf, Size, GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.ImageBase);
    }
}