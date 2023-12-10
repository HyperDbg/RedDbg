#include "GUI/DimensionsesOfWindowses.hpp"

Dimensionses_::WindowGeneralSizes::WindowGeneralSizes(
	const float TitleBarHeight, 
	const int SplitterSize,
	static float splitter1_relPosX, 
	static float splitter2_relPosY,
	const WindowCodes WindowCode)
{
	pos = ImGui::GetCursorPos();
	size = ImGui::GetContentRegionAvail();

	switch (WindowCode)
	{
	//MainTabs
	{
		case CPUTabWindowCode: __fallthrough;
		case StackMapTabWindowCode: __fallthrough;
		case ThreadsTabWindowCode: __fallthrough;
		case CallStackTabWindowCode: __fallthrough;
		case MemoryMapTabWindowCode: __fallthrough;
		case SEHTabWindowCode: __fallthrough;
		case PETabWindowCode:
		{
			size.y -= (TitleBarHeight + 15);
			SplitterVPosX = (int)(pos.x - 0.5f * (float)SplitterSize + splitter1_relPosX * size.x);

			LeftPos = pos;
			LeftSize = ImVec2((float)SplitterVPosX - pos.x, size.y);
			RightPos = ImVec2(LeftPos.x + LeftSize.x + (float)SplitterSize, pos.y);
			RightSize = ImVec2(pos.x + size.x - RightPos.x, size.y);

			SplitterHPosY = (int)(RightPos.y - 0.5f * (float)SplitterSize + splitter2_relPosY * RightSize.y);

			RightTopPosition = RightPos;
			RightTopSize = ImVec2(RightSize.x, float(SplitterHPosY - RightPos.y));

			RightBottomPosition = ImVec2(RightPos.x, RightTopPosition.y + RightTopSize.y + (float)SplitterSize);
			RightBottomSize = ImVec2(RightSize.x, RightPos.y + RightSize.y - RightBottomPosition.y);

			CenterBottomPosition = ImVec2(LeftPos.x, RightBottomPosition.y);
			CenterBottomSize = ImVec2(LeftSize.x + RightBottomSize.x + SplitterSize, RightPos.y + RightSize.y - RightBottomPosition.y);

			LeftTopPosition = LeftPos;
			LeftTopSize = ImVec2(LeftSize.x, (float)LeftSize.y - (CenterBottomSize.y + SplitterSize));
			return;
		}
	}
	//WindowsesOnYAxis
	{
		case DisasmAndInfoWindowCode: __fallthrough;
		case RegisterAndCallConventionWindowCode: __fallthrough;
		case PEFileInfoProtectionInfoWindowCode: __fallthrough;
		case PEFileInfoRightWindowCode: __fallthrough;
		case PeSectionHeaderTwoWindowInternalWindowCode: __fallthrough;
		case PeImportDirectoryTwoWindowInternalWindowCode:
		{
			SplitterVPosX = (int)(pos.x - 0.5f * (float)SplitterSize + splitter1_relPosX * size.x);

			LeftPos = pos;
			LeftSize = ImVec2((float)SplitterVPosX - pos.x + SplitterSize + pos.x + size.x - LeftPos.x + (float)SplitterSize, size.y);
			RightPos = ImVec2(LeftPos.x + LeftSize.x + (float)SplitterSize, pos.y);
			RightSize = ImVec2(pos.x + size.x - RightPos.x, size.y);

			SplitterHPosY = (int)(RightPos.y - 0.5f * (float)SplitterSize + splitter2_relPosY * RightSize.y);

			RightTopPosition = RightPos;
			RightTopSize = ImVec2(RightSize.x, ((float)SplitterHPosY - RightPos.y) + SplitterSize);

			RightBottomPosition = ImVec2(RightPos.x, RightTopPosition.y + RightTopSize.y);
			RightBottomSize = ImVec2(RightSize.x, RightPos.y + RightSize.y - RightBottomPosition.y);

			CenterBottomPosition = ImVec2(LeftPos.x, RightBottomPosition.y);
			CenterBottomSize = ImVec2(LeftSize.x + RightBottomSize.x + SplitterSize, RightPos.y + RightSize.y - RightBottomPosition.y);

			LeftTopPosition = LeftPos;
			LeftTopSize = ImVec2(LeftSize.x + RightSize.x + SplitterSize, (float)LeftSize.y - (CenterBottomSize.y + SplitterSize));
			return;
		}
	}
	//WindowsesOnXAxis
	{
		case StackAndDumpWindowCode: __fallthrough;
		case FindInThreadsWindowCode: __fallthrough;
		case InputFieldInFindThreadsWindowCode: __fallthrough;
		case FindInCallStackWindowCode: __fallthrough;
		case InputFieldInFindCallStackWindowCode: __fallthrough;
		case FindInMemoryMapWindowCode: __fallthrough;
		case InputFieldInFindMemoryMapWindowCode: __fallthrough;
		case FindInSEHWindowCode: __fallthrough;
		case InputFieldInFindSEHWindowCode: __fallthrough;
		case PEFileInfoInternalWindowCode:
		{
			SplitterVPosX = (int)(pos.x - 0.5f * (float)SplitterSize + splitter1_relPosX * size.x);

			LeftPos = pos;
			LeftSize = ImVec2((float)SplitterVPosX - pos.x, size.y);
			RightPos = ImVec2(LeftPos.x + LeftSize.x + (float)SplitterSize, pos.y);
			RightSize = ImVec2(pos.x + size.x - RightPos.x, size.y);

			RightTopPosition = RightPos;
			RightTopSize = ImVec2(RightSize.x, RightSize.y);

			LeftTopPosition = LeftPos;
			LeftTopSize = ImVec2(LeftSize.x, LeftSize.y);
			return;
		}
	}
	//WindowsesOnYAxisAndXAxis
	{
		case PEFileInfoLeftWindowCode:
		{
			SplitterVPosX = (int)(pos.x - 0.5f * (float)SplitterSize + splitter1_relPosX * size.x);

			LeftPos = pos;
			LeftSize = ImVec2((float)SplitterVPosX - pos.x, size.y);
			RightPos = ImVec2(LeftPos.x + LeftSize.x + (float)SplitterSize, pos.y);
			RightSize = ImVec2(pos.x + size.x - RightPos.x, size.y);

			SplitterHPosY = (int)(RightPos.y - 0.5f * (float)SplitterSize + splitter2_relPosY * RightSize.y);

			RightTopPosition = RightPos;
			RightTopSize = ImVec2(RightSize.x, float(SplitterHPosY - RightPos.y));

			RightBottomPosition = ImVec2(RightPos.x, RightTopPosition.y + RightTopSize.y + (float)SplitterSize);
			RightBottomSize = ImVec2(RightSize.x, RightPos.y + RightSize.y - RightBottomPosition.y);

			CenterBottomPosition = ImVec2(LeftPos.x, RightBottomPosition.y);
			CenterBottomSize = ImVec2(LeftSize.x + RightBottomSize.x + SplitterSize, RightPos.y + RightSize.y - RightBottomPosition.y);

			LeftTopPosition = LeftPos;
			LeftTopSize = ImVec2(LeftSize.x, (float)LeftSize.y - (CenterBottomSize.y + SplitterSize));
			return;
		}
	}
	}
}