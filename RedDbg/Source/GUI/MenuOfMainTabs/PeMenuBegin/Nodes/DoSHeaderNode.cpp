#include "GUI/MenuOfMainTabs/PeMenuBegin/PeTab.hpp"

#include <string>
#include <sstream>

namespace GlobalVarsOfPeTab {
    //extern PEInformation objPEInformation;
    extern std::shared_ptr<PeReader> objPEInformation;
}

void PETab_::PeDoSHeaderTableRender()
{
    const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_ScrollY |
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

    static bool Init = false;

    static std::vector<WORD*> DoSVals{
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_magic,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_cblp,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_cp,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_crlc,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_cparhdr,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_minalloc,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_maxalloc,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_ss,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_sp,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_csum,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_ip,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_cs,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_lfarlc,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_ovno,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res[0],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res[1],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res[2],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res[3],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_oemid,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_oeminfo,
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[0],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[1],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[2],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[3],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[4],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[5],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[6],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[7],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[8],
        //&GlobalVarsOfPeTab::objPEInformation->pImageDOSHeaderOfPe->e_res2[9],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_magic,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_cblp,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_cp,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_crlc,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_cparhdr,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_minalloc,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_maxalloc,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_ss,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_sp,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_csum,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_ip,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_cs,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfarlc,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_ovno,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res[0],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res[1],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res[2],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res[3],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_oemid,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_oeminfo,
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[0],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[1],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[2],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[3],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[4],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[5],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[6],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[7],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[8],
        &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_res2[9],
    };

    static std::vector<std::string> InputStrings;

    if (!Init)
    {
        InputStrings.resize(31);
        Init = true;
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 1, 1 });
    if (ImGui::BeginTable("a", 7, DefaultTableFlags))
    {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Structure Field", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Offset", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("##", ImGuiTableColumnFlags_Disabled);
        ImGui::TableHeadersRow();


        for (int Row = 0; Row < 31; Row++)
        {
            ImGui::TableNextRow(ImGuiTableRowFlags_None, 17);
            for (int Column = 0; Column < 7; ++Column)
            {
                Window.SelectAllAndColored(7, Dimensionses.U32GrayColor);

                ImGui::TableSetColumnIndex(Column);

                ImGui::PushStyleColor(ImGuiCol_HeaderActive, Dimensionses.U32TransparentColor);
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, Dimensionses.U32TransparentColor);

                if (Column == 0)
                {
                    ImGui::Selectable(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.DoSHeader.DoSHeaderNames[Row].data());
                    if (Row <= 14 || Row >= 18 && Row <= 20 || Row == 30)
                    {
                        if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip) && ImGui::BeginTooltip())
                        {
                            ImGui::SetTooltip(Names.Windowses.MainDebuggerInterface.MainTabs.PETab.PENodes.DoSHeader.DosHeaderToopTipNames[Row].data());
                            ImGui::EndTooltip();
                        }
                    }
                }
                else if (Column == 1)
                {
                    std::stringstream Ss;
                    Ss << std::uppercase << std::hex << PeStruct::DoSHeader::DoSHeaderOffsets[Row];

                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 2)
                {
                    if (Row < 30)
                    {
                        ImGui::Selectable("Word");
                    }
                    else
                    {
                        ImGui::Selectable("Dword");
                    }
                }
                else if (Column == 3)
                {
                    std::stringstream Ss;
                    if (GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalMagic == IMAGE_NT_OPTIONAL_HDR64_MAGIC)
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint64_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader64.ImageBase + PeStruct::DoSHeader::DoSHeaderOffsets[Row];
                    }
                    else
                    {
                        Ss << std::uppercase << std::setfill('0') <<
                            std::setw(sizeof(uint32_t) * 2) << std::hex <<
                            GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.nt.OptionalHeader.ImageBase + PeStruct::DoSHeader::DoSHeaderOffsets[Row];
                    }
                    ImGui::Selectable(Ss.str().c_str());
                }
                else if (Column == 4)
                {
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                    if (Row < 30)
                    {
                        static std::string a;
                        EditField<WORD>(
                            "##" + std::to_string(Row),
                            InputStrings[Row],
                            DoSVals[Row], 5);
                    }
                    else
                    {
                        static std::string a;
                        EditField<uint32_t>(
                            "##" + std::to_string(Row),
                            InputStrings[Row],
                            &GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_lfanew, 9);
                    }
                    ImGui::PopStyleColor();
                }
                else if (Column == 5)
                {
                    if (Row == PeStruct::DoSHeader::DoSHeaderRows::Emagic)
                    {
                        ImGui::Selectable(ConvertToCharString(GlobalVarsOfPeTab::objPEInformation->Pe->peHeader.dos.e_magic));
                    }
                }
                ImGui::PopStyleColor(2);
            }
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();
}
