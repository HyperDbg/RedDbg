#pragma once
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/WindowCodes.hpp"

class Dimensionses_ {
public:
	class WindowGeneralSizes 
	{
	public:
		ImVec2 pos{};
		ImVec2 size{};

		int SplitterVPosX = 0;

		ImVec2 LeftPos{};
		ImVec2 LeftSize{};
		ImVec2 RightPos{};
		ImVec2 RightSize{};

		int SplitterHPosY = 0;

		ImVec2 RightTopPosition{};
		ImVec2 RightTopSize{};
		ImVec2 RightBottomPosition{};
		ImVec2 RightBottomSize{};
		ImVec2 CenterBottomPosition{};
		ImVec2 CenterBottomSize{};
		ImVec2 LeftTopPosition{};
		ImVec2 LeftTopSize{};

		WindowGeneralSizes(const float TitleBarHeight, const int SplitterSize, static float splitter1_relPosX, static float splitter2_relPosY, const WindowCodes WindowCode);
	};
public:
	const int SplitterSize = 4;
	const float TitleBarHeight = 25;
	const int TitleBarFromTextRedDbg = 50;
	const int MiddleOfText = 6;

	const float SplitterIsDisabled = 1.0019f;
	const float SizeOfX64Addr = 126.0f;
	const float SizeOfSections = 20.0f;
	const float SizeOfEndianness = 30.0f;
	const float SizeOfUIType = 40.0f;

	const unsigned int U32GrayColor = 0x74747474;// 0x8f8f8f8f;
	const unsigned int U32BlackGrayColor = 0x55555555;
	const unsigned int U32TransparentColor = 0x00ffffff;
	const unsigned int U32OrangeColor = 0xff00f6ff;
	const unsigned int U32RedColor = 0xff0000c8;

	const unsigned char CountOfStackColumns = 4;
	const unsigned char CountOfDisasmColumns = 6;
	const unsigned char CountOfThreadsColumns = 15;
	const unsigned char CountOfStackMapAddrsTableColumns = 2;
	const unsigned char CountOfStackMapRefsToAddrsTableColumns = 6;
	const unsigned char CountOfCallStackColumns = 9;
	const unsigned char CountOfMemoryMapColumns = 9;
	const unsigned char CountOfSEHColumns = 5;

	const unsigned char CountOfAllNodes = 14;
};