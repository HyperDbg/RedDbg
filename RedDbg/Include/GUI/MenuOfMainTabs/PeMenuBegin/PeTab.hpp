#pragma once
//#include "pe-parse/parse.h"
#include "GUI/MenuOfMainTabs/PeMenuBegin/Parser/PEReader.hpp"

#include "GUI/UI/ImGui/imgui.h"

#include "GUI/GlobalClass.hpp"
#include "GUI/Names.hpp"
#include "GUI/DimensionsesOfWindowses.hpp"
#include "GUI/Window.hpp"

//#include "Debugger/Application/Pe/PEReader.hpp"
//#include "Debugger/Application/Pe/PEInformation.hpp"

#include <vector>

//https://fpic.in/UDNBrit

namespace PeStruct
{
	namespace DoSHeader
	{
		enum DoSHeaderRows
		{
			Emagic,
			Ecblp,
			Ecp,
			Ecrlc,
			Ecparhdr,
			EminAlloc,
			EmaxAlloc,
			Ess,
			Esp,
			Ecsum,
			Eip,
			Ecs,
			Eifarlc,
			Eovno,
			Eres,
			EresArray1,
			EresArray2,
			EresArray3,
			EoemId,
			EoemInfo,
			Eres2,
			Eres2Array1,
			Eres2Array2,
			Eres2Array3,
			Eres2Array4,
			Eres2Array5,
			Eres2Array6,
			Eres2Array7,
			Eres2Array8,
			Eres2Array9,
			Eifanew
		};

		enum DoSHeaderOffset : unsigned char
		{
			EmagicOffset = 0x00,
			EcblpOffset = 0x02,
			EcpOffset = 0x04,
			EcrlcOffset = 0x06,
			EcparhdrOffset = 0x08,
			EminAllocOffset = 0x0A,
			EmaxAllocOffset = 0x0C,
			EssOffset = 0x0E,
			EspOffset = 0x10,
			EcsumOffset = 0x12,
			EipOffset = 0x14,
			EcsOffset = 0x16,
			EifarlcOffset = 0x18,
			EovnoOffset = 0x1A,
			EresOffset = 0x1C,
			EresArray1Offset = 0x1E,
			EresArray2Offset = 0x20,
			EresArray3Offset = 0x22,
			EoemIdOffset = 0x24,
			EoemInfoOffset = 0x26,
			Eres2Offset = 0x28,
			Eres2Array1Offset = 0x2A,
			Eres2Array2Offset = 0x2C,
			Eres2Array3Offset = 0x2E,
			Eres2Array4Offset = 0x30,
			Eres2Array5Offset = 0x32,
			Eres2Array6Offset = 0x34,
			Eres2Array7Offset = 0x36,
			Eres2Array8Offset = 0x38,
			Eres2Array9Offset = 0x3A,
			EifanewOffset = 0x3C
		};

		const std::vector<uint32_t> DoSHeaderOffsets{
			EmagicOffset,
			EcblpOffset,
			EcpOffset,
			EcrlcOffset,
			EcparhdrOffset,
			EminAllocOffset,
			EmaxAllocOffset,
			EssOffset,
			EspOffset,
			EcsumOffset,
			EipOffset,
			EcsOffset,
			EifarlcOffset,
			EovnoOffset,
			EresOffset,
			EresArray1Offset,
			EresArray2Offset,
			EresArray3Offset,
			EoemIdOffset,
			EoemInfoOffset,
			Eres2Offset,
			Eres2Array1Offset,
			Eres2Array2Offset,
			Eres2Array3Offset,
			Eres2Array4Offset,
			Eres2Array5Offset,
			Eres2Array6Offset,
			Eres2Array7Offset,
			Eres2Array8Offset,
			Eres2Array9Offset,
			EifanewOffset
		};
	}

	namespace RichHeader
	{
		enum RichHeaderRows
		{
			LinkerImportLib,
			IATEntry,
			MasmAsmCoff,
			CvtresResToCoff,
			LinkerLink
		};
	}

	namespace NtHeader
	{
		enum NtHeaderRows
		{
			Signature,
		};
	}

	namespace FileHeader
	{
		enum MachineRows
		{
			Intel386,
			R3000Mips,
			R4000Mips,
			R10000Mips,
			MipsWceV2,
			AlphaAxp,
			SH3,
			SH3DSP,
			SH3E,
			SH4,
			SH5,
			Arm,
			ArmThumb,
			ArmAm33,
			IbmPowerPc,
			IbmPowerPcFp,
			Intel64,
			Mips16,
			MipsFpu,
			MipsFpu16,
			Alpha64,
			InfineonTriCore,
			InfineonCEF,
			EfiByteCode,
			Amd64K8,
			M32R,
			CEE
		};

		/*
		enum class MachineCodes
		{
			Intel386 = 0x014C,
			R3000Mips = 0x0162,
			R4000Mips = 0x0166,
			R10000Mips = 0x0168,
			MipsWceV2 = 0x0169,
			AlphaAxp = 0x0184,
			SH3 = 0x01A2,
			SH3DSP = 0x01A3,
			SH3E = 0x01A4,
			SH4 = 0x01A6,
			SH5 = 0x01A8,
			Arm = 0x01C0,
			ArmThumb = 0x01C2,
			ArmAm33 = 0x01D3,
			IbmPowerPc = 0x01F0,
			IbmPowerPcFp = 0x01F1,
			Intel64 = 0x0200,
			Mips16 = 0x0266,
			MipsFpu = 0x0366,
			MipsFpu16 = 0x0466,
			Alpha64 = 0x0284,
			InfineonTriCore = 0x0520,
			InfineonCEF = 0x0CEF,
			EfiByteCode = 0x0EBC,
			Amd64K8 = 0x8664,
			M32R = 0x9041,
			CEE = 0xC0EE
		};
		*/
		enum CharacteristicsRows
		{
			FileIsExecutable,
			FileIsDll,
			SystemFile,
			RelocationInfoStrippedFromFile,
			LineNumbersStrippedFromFile,
			AgressivelyTrimWorkingSet,
			AppCanHandle2GbAddressSpaceOrMore,
			BytesOfMachineWordAreReversedLow,
			Bit32WordMachine,
			DebuggingInfoStrippedFromFileInDgbFile,
			IfImageIsOnRemovableMediaCopyAndRunFromSwapFile,
			IfImageIsOnNetCopyAndRunFromSwapFile,
			FileShouldOnlyBeRunOnUpMachine,
			BytesOfMachineWordAreReversedHigh
		};

		enum FileHeaderRows
		{
			Machine,
			NumberOfSections,
			TimeDataStamp,
			PointerToSymbolTable,
			NumberOfSymbols,
			SizeOfOptionalHeader,
			Charateristics
		};

		enum FileHeaderOffset
		{
			MachineOffset = 0xDC,
			NumberOfSectionsOffset = 0xDE,
			TimeDataStampOffset = 0xE0,
			PointerToSymbolTableOffset = 0xE4,
			NumberOfSymbolsOffset = 0xE8,
			SizeOfOptionalHeaderOffset = 0xEC,
			CharateristicsOffset = 0xEE
		};

		const std::vector<uint32_t> FileHeaderOffsets{
			MachineOffset,
			NumberOfSectionsOffset,
			TimeDataStampOffset,
			PointerToSymbolTableOffset,
			NumberOfSymbolsOffset,
			SizeOfOptionalHeaderOffset,
			CharateristicsOffset
		};
	}

	namespace OptionalHeader
	{
		enum OptionalHeaderOffset
		{
			MagicOffset = 0xF0,
			MajorLinkerVersionOffset = 0xF2,
			MinorLinkerVersionOffset = 0xF3,
			SizeOfCodeOffset = 0xF4,
			SizeOfInitDataOffset = 0xF8,
			SizeOfUninitDataOffset = 0xFC,
			EntryPointOffset = 0x100,
			BaseOfCodeOffset = 0x104,
			ImageBaseOffset = 0x108,
			SectionAlignmentOffset = 0x110,
			FileAlignmentOffset = 0x114,
			MajorOperatingSystemVersionOffset = 0x118,
			MinorOperatingSystemVersionOffset = 0x11A,
			MajorImageVersionOffset = 0x11C,
			MinorImageVersionOffset = 0x11E,
			MajorSubsystemVersionOffset = 0x120,
			MinorSubsystemVersionOffset = 0x122,
			Win32VersionValueOffset = 0x124,
			SizeOfImageOffset = 0x128,
			SizeOfHeadersOffset = 0x12C,
			ChecksumOffset = 0x130,
			SubsystemOffset = 0x134,
			DllCharacteristicsOffset = 0x136,
			SizeOfStackReserveOffset = 0x138,
			SizeOfStackCommitOffset = 0x140,
			SizeOfHeapReserveOffset = 0x148,
			SizeOfHeapCommitOffset = 0x150,
			LoaderFlagsOffset = 0x158,
			NumberOfRvaAndSizesOffset = 0x15C
		};

		const std::vector<uint32_t> OptionalHeaderOffsets{
			MagicOffset,
			MajorLinkerVersionOffset,
			MinorLinkerVersionOffset,
			SizeOfCodeOffset,
			SizeOfInitDataOffset,
			SizeOfUninitDataOffset,
			EntryPointOffset,
			BaseOfCodeOffset,
			ImageBaseOffset,
			SectionAlignmentOffset,
			FileAlignmentOffset,
			MajorOperatingSystemVersionOffset,
			MinorOperatingSystemVersionOffset,
			MajorImageVersionOffset,
			MinorImageVersionOffset,
			MajorSubsystemVersionOffset,
			MinorSubsystemVersionOffset,
			Win32VersionValueOffset,
			SizeOfImageOffset,
			SizeOfHeadersOffset,
			ChecksumOffset,
			SubsystemOffset,
			DllCharacteristicsOffset,
			SizeOfStackReserveOffset,
			SizeOfStackCommitOffset,
			SizeOfHeapReserveOffset,
			SizeOfHeapCommitOffset,
			LoaderFlagsOffset,
			NumberOfRvaAndSizesOffset
		};
	}

	namespace DataDirectories
	{
		enum DataDirectoriesOffset
		{
			ExportDirectoryRvaOffset = 0x160,
			ExportDirectorySizeOffset = 0x164,
			ImportDirectoryRvaOffset = 0x168,
			ImportDirectorySizeOffset = 0x16C,
			ResourceDirectoryRvaOffset = 0x170,
			ResourceDirectorySizeOffset = 0x174,
			ExceptionDirectoryRvaOffset = 0x178,
			ExceptionDirectorySizeOffset = 0x17C,
			SecurityDirectoryRvaOffset = 0x180,
			SecurityDirectorySizeOffset = 0x184,
			RelocationDirectoryRvaOffset = 0x188,
			RelocationDirectorySizeOffset = 0x18C,
			DebugDirectoryRvaOffset = 0x190,
			DebugDirectorySizeOffset = 0x194,
			ArchitectureDirectoryRvaOffset = 0x198,
			ArchitectureDirectorySizeOffset = 0x19C,
			ReservedRvaOffset = 0x1A0,
			ReservedSizeOffset = 0x1A4,
			TLSDirectoryRvaOffset = 0x1A8,
			TLSDirectorySizeOffset = 0x1AC,
			ConfigurationDirectoryRvaOffset = 0x1B0,
			ConfigurationDirectorySize = 0x1B4,
			BoundImportDirectoryRvaOffset = 0x1B8,
			BoundImportDirectorySizeOffset = 0x1BC,
			ImportAddressTableDirectoryRvaOffset = 0x1C0,
			ImportAddressTableDirectorySizeOffset = 0x1C4,
			DelayImportDirectoryRvaOffset = 0x1C8,
			DelayImportDirectorySizeOffset = 0x1CC,
			DotNetMetaDataDirectoryRvaOffset = 0x1D0,
			DotNetMetaDataDirectorySizeOffset = 0x1D4,
		};

		const std::vector<uint32_t> DataDirectoriesOffsets{
			ExportDirectoryRvaOffset,
			ExportDirectorySizeOffset,
			ImportDirectoryRvaOffset,
			ImportDirectorySizeOffset,
			ResourceDirectoryRvaOffset,
			ResourceDirectorySizeOffset,
			ExceptionDirectoryRvaOffset,
			ExceptionDirectorySizeOffset,
			SecurityDirectoryRvaOffset,
			SecurityDirectorySizeOffset,
			RelocationDirectoryRvaOffset,
			RelocationDirectorySizeOffset,
			DebugDirectoryRvaOffset,
			DebugDirectorySizeOffset,
			ArchitectureDirectoryRvaOffset,
			ArchitectureDirectorySizeOffset,
			ReservedRvaOffset,
			ReservedSizeOffset,
			TLSDirectoryRvaOffset,
			TLSDirectorySizeOffset,
			ConfigurationDirectoryRvaOffset,
			ConfigurationDirectorySize,
			BoundImportDirectoryRvaOffset,
			BoundImportDirectorySizeOffset,
			ImportAddressTableDirectoryRvaOffset,
			ImportAddressTableDirectorySizeOffset,
			DelayImportDirectoryRvaOffset,
			DelayImportDirectorySizeOffset,
			DotNetMetaDataDirectoryRvaOffset,
			DotNetMetaDataDirectorySizeOffset,
		};
	}
}

class PETab_
{
private:
	Dimensionses_ Dimensionses;
	Names_ Names;
	GlobalClassVars_ GlobalClassVars;
	Window_ Window;
private:
	void PeInfoNodesDispatcher(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	template<typename Data>
	/*
	* Arg1: InputTitle
	* Arg2: InputString
	* Arg3: Data(for change and print) in case with Arg 6 is printable
	* Arg4: Length of Input
	* Arg5: Arg 3 have byte vals
	* Arg6: Optional changeable data
	*/
	void EditField(
		const std::string& InputTextTitle,
		std::string& Input, 
		Data* PeData,
		const unsigned char Length,
		bool Byte = false,
		Data* Pointers = nullptr,
		bool EntryPointIsQword = false,
		uint64_t ImageBase = 0,
		bool WithoutInput = false)
	{
		if (!WithoutInput)
		{
			std::stringstream Ss;
			if (!Byte)
			{
				Ss << std::setfill('0') << std::setw(sizeof(Data) * 2) << std::uppercase << std::hex << *PeData << " ";
			}
			else
			{
				Ss << std::setfill('0') << std::setw(sizeof(BYTE) * 2) << std::uppercase << std::hex << (uint16_t)*PeData << " ";
			}
			Input = Ss.str();
		}

		ImVec2 Size = ImGui::GetContentRegionAvail();

		ImGui::PushItemWidth(Size.x);
		ImGui::PushStyleColor(ImGuiCol_Text, Dimensionses.U32OrangeColor);
		if (ImGui::InputText(
			InputTextTitle.c_str(),
			Input.data(),
			Input.size(),
			ImGuiInputTextFlags_CallbackResize | ImGuiInputTextFlags_CharsHexadecimal,
			reinterpret_cast<ImGuiInputTextCallback>(Callable::CallBackInputWrapper),
			&Input))
		{
			if (Pointers == nullptr)
			{
				if (Input[0] != 0 && Input.length() <= Length)
				{
					if(!EntryPointIsQword) { *PeData = static_cast<Data>(std::stoi(Input, nullptr, 16)); }
					else
					{
						*PeData = static_cast<Data>((std::stoull(Input, nullptr, 16) - ImageBase));
					}
				}
			}
			else
			{
				if (Input[0] != 0 && Input.length() <= Length)
				{
					if (!EntryPointIsQword) { *Pointers = static_cast<Data>(std::stoi(Input, nullptr, 16)); }
					else
					{
						*Pointers = (static_cast<Data>(std::stoull(Input, nullptr, 16)) - ImageBase);
					}
				}
			}
		}
		ImGui::PopStyleColor();
		ImGui::PopItemWidth();
	}

	//void CheckBoxOpened(std::vector<bool>* Openeds, bool Opened, const unsigned char Index);

	template <typename DataT>
	char* ConvertToCharString(DataT value) 
	{
		switch (sizeof(DataT))
		{
		case 1:
		{
			char Buffer[2]{};
			Buffer[0] = value;
			Buffer[1] = '\0';
			return Buffer;
		}
		case 2:
		{
			char Buffer[3]{};
			Buffer[0] = (value & 0xFF00) >> 8;
			Buffer[1] = (value & 0x00FF);
			Buffer[2] = '\0';
			return Buffer;
		}
		case 4:
		{
			char Buffer[5]{};
			Buffer[0] = (value & 0xFF000000) >> 8;
			Buffer[1] = (value & 0xFFFF0000) >> 16;
			Buffer[2] = (value & 0xFFFFFF00) >> 24;
			Buffer[3] = (value & 0x00FFFFFF);
			Buffer[4] = '\0';
			return Buffer;
		}
		}

		return nullptr;
	}

	bool OptionalBitnessRowModeChooserA(int Row);
	bool OptionalBitnessRowModeChooserB(int Row);
public:
	void PeWindowRender();

	void PeTreeNodeRender();

	void PeFileInfoWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	//Left and right
	void PeFileInfoInternalWindowRender();
	//AllWindowsesByLeft
	void PeFileInfoInternalWindow2Render();
	void PeFileInfoOfInfoWindowRender();
	void PeFileInfoHashInfoWindowRender();
	void PeFileInfoProtectionInfoWindowRender();
	void PeFileInfoProtectionButtonsWindowRender();
	//AllWindowsesByRight
	void PeFileInfoInternalWindow3Render();
	void PeFileInfoDefaultButtonsRightWindowRender();
	void PeFileInfoProtectionButtonsRightWindowRender();

	void PeDoSHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeDoSHeaderTableRender();

	void PeRichHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeRichHeaderTableRender();

	void PeNtHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeNtHeaderTableRender();

	void PeFileHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeFileHeaderTableRender();

	void PeOptionalHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeOptionalHeaderTableRender();

	void PeDataDirectoriesWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeDataDirectoriesTableRender();

	void PeSectionHeaderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeSectionHeaderInternalWindowRender();
	void PeSectionHeaderTableRender();
	void PeInfoOfSectionInSectionHeaderWindowRender();

	void PeImportDirectoryWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeImportDirectoryInternalWindowRender();
	void PeImportDirectoryTableRender();
	void PeInfoOfImportDllWindowRender();

	void PeResourceDirectoryWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	//void PeResourceDirectoryTreeNodeRender();
	//void PeResourceDirectoryTableRender();

	void PeExceptionDirectoryWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	//void PeExceptionDirectoryTableRender();

	void PeDependencyWalkerWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	//void PeDependencyWalkerInternalWindowRender();

	void PeImportAdderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
	void PeStructRebuilderWindowRender(const Dimensionses_::WindowGeneralSizes* WindowGeneralSizes);
};