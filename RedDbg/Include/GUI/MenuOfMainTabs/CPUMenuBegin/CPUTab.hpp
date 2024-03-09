#pragma once
#include "GUI/MenuOfMainTabs/CPUMenuBegin/CPU.hpp"
#include "GUI/MenuOfMainTabs/CPUMenuBegin/HexTab.hpp"
#include "GUI/UI/ImGui/imgui.h"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

#include <memory>

enum Registers : unsigned char {
	RIP,
	RAX,
	RBX,
	RCX,
	RDX,
	RBP,
	RSP,
	RSI,
	RDI,
	R8,
	R9,
	R10,
	R11,
	R12,
	R13,
	R14,
	R15,
	RFLAGS
};

enum class TypeOfDisplay_: unsigned char{
	ST_X87,
	Mmx,
	X87_ST
};

enum class TypeOfFpuBitFlags_ : unsigned char {
	X87TW,
	X87SW,
	X87CW,
	Sse
};

enum DumpTab_ : const uint64_t {
	Dump1,
	Dump2,
	Dump3,
	Dump4,
	Dump5,
	Watch1,
	Locals,
	Struct
};

class CPUTab_
{
private:
	inline static Dimensionses_ Dimensionses;
	inline static Names_ Names;
	inline static GlobalClassVars_ GlobalClassVars;
	Window_ Window;
private:
	const ImGuiTableFlags DisassemblyColumnFlags = ImGuiTableColumnFlags_WidthFixed;
	const ImGuiTableFlags DefaultTableFlags = ImGuiTableFlags_HighlightHoveredColumn | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

	void GeneralRegister(const std::vector<std::string_view>& vGeneralRegs, std::string_view& Data);
	void FlagsRegister(const std::vector<std::string_view>& vBitsOfFlag);
	void LastError_Status();
	void SegmentsRegisters();

	void X87r_MmxRegisters(const TypeOfDisplay_ Type);
	void X87FlagAndSseFlags(
		const std::string_view& X87Word, 
		const std::vector<std::string_view> vX87,
		TypeOfFpuBitFlags_ TypeOfFpuBitFlags);

	void AvxRegisters(const std::vector<std::string_view> vAvxRegs);
	void SseRegisters(const std::vector<std::string_view> vSseRegs);

	void DrRegisters();
public:
	void CPUDisasmAndInfoWindowRender();
	void CPURegisterAndCallConventionWindowRender();
	void StackAndDumpWindowRender();

	//StackAndDumpWindowRender
	void StackTableRender();
	//CPUDisasmAndInfoWindowRender
	void DumpRender();
	void TabDispatcherDump(DumpTab_ TabCode, bool* open);
	void RegisterRender();
	void CallConventionRender();
	void DisassemblyTableRender();
public:
	void CPUWindowRender();
};