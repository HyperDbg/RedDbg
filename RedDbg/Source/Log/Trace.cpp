#include "Log/Trace.hpp"

#include <ntstrsafe.h>
#include <intrin.h>
#include <algorithm>
#include <HyperVisor/CommonApi/MemoryUtils.hpp>
GraphVizLanguage objGraphVizLanguage;

void RtlInt64ToCharExsitingArray(_In_ uint64_t number, IN ULONG Base, _Out_ CHAR* hexString, _Inout_ size_t Length)
{
	CHAR Buffer[65];

	PCHAR Pos = &Buffer[64];
	*Pos = '\0';

	do
	{
		Pos--;
		CHAR Digit = (CHAR)(number % Base);
		number = number / Base;

		if (Digit < 10)
			*Pos = '0' + Digit;
		else
			*Pos = 'A' + Digit - 10;
	} while (number != 0L);

	size_t Len = &Buffer[64] - Pos;

	Length = Len;
	RtlCopyMemory(hexString, Pos, Len);
}

#define STRCAT(dest, src, Len) \
    strcat_s((PCH)dest, sizeof(src), src); \
	Len += strlen(dest); \
    dest = (PCH)(UINT64(dest) + strlen(dest))

#define STRCATADDR(Dest, Addr, Base, LenOut, ValLen) \
	RtlInt64ToCharExsitingArray(Addr, Base, (PCH)Dest, LenOut); \
	ValLen += strlen(Dest); \
	Dest = (PCH)(UINT64(Dest) + strlen(Dest));

#define EntryPointOfGraphVizLanguage(WriteTo, Name, Len) \
	STRCAT(WriteTo, objGraphVizLanguage.DigraphQuote, Len); \
	STRCAT(WriteTo, Name, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.StartOfdigraph, Len)

#define NodeAttributesOfDigraph(WriteTo, Len) \
    STRCAT(WriteTo, objGraphVizLanguage.Node, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.StartOfLabel, Len);\
	STRCAT(WriteTo, objGraphVizLanguage.Shape, Len);\
	STRCAT(WriteTo, objGraphVizLanguage.RecordShape, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.FontCourierName, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.FillBlackColor, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.FontColor, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.StyleTrue, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.EndOfLabel, Len);

#define AddGraph(WriteTo, UINT64FromAddr, UINT64ToAddr, Base, Len, LenOut) \
	STRCAT(WriteTo, objGraphVizLanguage.Address, Len); \
	STRCATADDR(WriteTo, UINT64FromAddr, Base, LenOut, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.Line, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.Address, Len); \
	STRCATADDR(WriteTo, UINT64ToAddr, Base, LenOut, Len);

#define SpecificNodeAdd(WriteTo, UINT64A, Base, Len, LenOut) \
	STRCAT(WriteTo, objGraphVizLanguage.Address, Len); \
	STRCATADDR(WriteTo, UINT64A, Base, LenOut, Len);

#define SpecificNodeLabel(WriteTo, _LabelText, Len) \
    STRCAT(WriteTo, objGraphVizLanguage.StartOfLabel, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.Label, Len); \
	STRCAT(WriteTo, _LabelText, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.EndOfLabel, Len); \
	STRCAT(WriteTo, "\n", Len);

#define SpecificNodeEntryLabel(WriteTo, Len) \
    STRCAT(WriteTo, objGraphVizLanguage.StartOfLabel, Len); \
	STRCAT(WriteTo, objGraphVizLanguage.Label, Len);

#define SpecificNodeFinLabel(WriteTo, Len) \
	STRCAT(WriteTo, "\"", Len); \
	STRCAT(WriteTo, objGraphVizLanguage.EndOfLabel, Len); \
	STRCAT(WriteTo, "\n", Len);

void Trace::AcceptGraphMessage(File& objFile)
{
	int Arc = 0;
	for (;;)
	{
		if (CircleOfMnemonics[Arc]->Reading)
		{
			KdPrint(("Data written %p and ARC IS %x\n", CircleOfMnemonics[Arc]->objMnemonicArray.size(), Arc));

			memset(&CircleOfMnemonics[Arc]->objMnemonicArray, 0, sizeof(CircleOfMnemonics[Arc]->objMnemonicArray));
			CircleOfMnemonics[Arc]->ElementsPassedInArray = 0;
			CircleOfMnemonics[Arc]->Reading = false;
		}
		if (Arc == Arcs) { Arc = 0; }
		else { ++Arc; }
	}
}

void Trace::AcceptGraphLBRMessage(File& objFile)
{
	int Arc = 0;
	for (;;)
	{
		if (CircleOfLbr[Arc]->Reading)
		{
			KdPrint(("Data written %p and ARC IS %x\n", CircleOfLbr[Arc]->objMnemonicArray.size(), Arc));
			/*
			PCH ValuesBuffer = (PCH)ExAllocatePool(NonPagedPoolNx, 4294967296);
			RtlZeroMemory(ValuesBuffer, 4294967296);
			if (ValuesBuffer != nullptr)
			{
				PCH ValuesCWI = ValuesBuffer;
				size_t ValuesLen = 0;
				size_t Length = 0;
				auto a = CircleOfLbr[Arc];

				size_t LenOut = 0;
				for (auto LBRMnem : CircleOfLbr[Arc]->objMnemonicArray)
				{
					STRCATADDR(ValuesCWI, LBRMnem.BrFrom, 16, LenOut, ValuesLen);
					STRCAT(ValuesCWI, "\\n", ValuesLen);
				}
				objFile.WriteFile(ValuesBuffer, ValuesLen);
			}
			*/
			memset(&CircleOfLbr[Arc]->objMnemonicArray, 0, sizeof(CircleOfLbr[Arc]->objMnemonicArray));
			CircleOfLbr[Arc]->ElementsPassedInArray = 0;
			CircleOfLbr[Arc]->Reading = false;
		}
		if (Arc == Arcs) { Arc = 0; }
		else { ++Arc; }
	}
}

/*
template<typename Type>
bool Trace::TraceInitializeMnemonic(std::vector<TraceMessage<Type>*> Circle)
{
	for (int Arc = 0; Arc <= Arcs; ++Arc)
	{
		TraceMessage<Type>* Mnemonic = (TraceMessage<Type>*)PhysicalMemory::AllocZeroedPhys(sizeof(TraceMessage<Type>));
		Circle.push_back(Mnemonic);
		Circle[Arc]->ElementsPassedInArray = 0;
		Circle[Arc]->MaxElementsInArray = (15 * 1024 * 1024) / sizeof(Type);

		//CircleOfMnemonics.push_back(Mnemonic);
		//CircleOfMnemonics[Arc]->ElementsPassedInArray = 0;
		//CircleOfMnemonics[Arc]->MaxElementsInArray = (15 * 1024 * 1024) / sizeof(Mnemonic);
	}
	return true;
}
*/

bool Trace::TraceInitializeMnemonic()
{
	for (int Arc = 0; Arc <= Arcs; ++Arc)
	{
		TraceMessage<LBRMnemonic>* Mnemonic = (TraceMessage<LBRMnemonic>*)PhysicalMemory::AllocZeroedPhys(sizeof(TraceMessage<LBRMnemonic>));
		CircleOfLbr.push_back(Mnemonic);
		CircleOfLbr[Arc]->ElementsPassedInArray = 0;
		CircleOfLbr[Arc]->MaxElementsInArray = (15 * 1024 * 1024) / sizeof(LBRMnemonic);

		//CircleOfMnemonics.push_back(Mnemonic);
		//CircleOfMnemonics[Arc]->ElementsPassedInArray = 0;
		//CircleOfMnemonics[Arc]->MaxElementsInArray = (15 * 1024 * 1024) / sizeof(Mnemonic);
	}
	return true;
}

Mnemonic Trace::MnemonicCreator(_In_ SVM::PRIVATE_VM_DATA* Private)
{
	Mnemonic objMnemonic;
	bool GraphConsequence = false;

	ZydisDecodedInstruction Instruction;
	ZydisDecodedOperand Operands[ZYDIS_MAX_OPERAND_COUNT];
	ZydisDecoder DecoderMinimal;
	DecoderMinimal.decoder_mode = ZYDIS_DECODER_MODE_MINIMAL;
	DecoderMinimal.machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
	DecoderMinimal.stack_width = ZYDIS_STACK_WIDTH_64;;

	ZydisDecoderDecodeInstruction(
		&DecoderMinimal,//DecoderFull,
		0,
		(const void*)Private->Guest.StateSaveArea.Rip,
		ZYDIS_MAX_INSTRUCTION_LENGTH,
		&Instruction);

	objMnemonic.Address = Private->Guest.StateSaveArea.Rip;
	objMnemonic.Length = Instruction.length;
	RtlCopyMemory(objMnemonic.Opcodes, (const void*)objMnemonic.Address, objMnemonic.Length);

	return objMnemonic;
}

void Trace::MnemonicWriter(_In_ SVM::PRIVATE_VM_DATA* Private)
{
	Mnemonic objMnemonic = MnemonicCreator(Private);
	SpinlockLock((LONG*)&SvmRootLoggingLock);
	RtlCopyMemory(&CircleOfMnemonics[Arc]->objMnemonicArray[CircleOfMnemonics[Arc]->ElementsPassedInArray], &objMnemonic, sizeof(Mnemonic));
	SpinlockUnlock((LONG*)&SvmRootLoggingLock);
	++CircleOfMnemonics[Arc]->ElementsPassedInArray;
}

void Trace::MnemonicHandler(_In_ SVM::PRIVATE_VM_DATA* Private)
{
	if (!CircleOfMnemonics[Arc]->Reading)
	{
		if (CircleOfMnemonics[Arc]->ElementsPassedInArray < CircleOfMnemonics[Arc]->MaxElementsInArray) { MnemonicWriter(Private); }
		else
		{
			CircleOfMnemonics[Arc]->Reading = true;
			if (Arc < Arcs) { ++Arc; MnemonicWriter(Private); }
			else { Arc = 0; MnemonicWriter(Private); }
		}
	}
}

void Trace::LBRMnemonicWriter(_In_ SVM::PRIVATE_VM_DATA* Private)
{
	LBRMnemonic objLBRMnemonic;
	objLBRMnemonic.BrFrom = Private->Guest.StateSaveArea.BrFrom;
	objLBRMnemonic.BrTo = Private->Guest.StateSaveArea.BrTo;
	//RtlCopyMemory(objLBRMnemonic.Opcodes, (const void*)objLBRMnemonic.BrFrom, 12);

	SpinlockLock((LONG*)&SvmRootLoggingLock);
	RtlCopyMemory(&CircleOfLbr[Arc]->objMnemonicArray[CircleOfLbr[Arc]->ElementsPassedInArray], &objLBRMnemonic, sizeof(LBRMnemonic));
	SpinlockUnlock((LONG*)&SvmRootLoggingLock);
	++CircleOfLbr[Arc]->ElementsPassedInArray;
}

void Trace::LbrMnemonicHandler(_In_ SVM::PRIVATE_VM_DATA* Private)
{
	if (!CircleOfLbr[Arc]->Reading)
	{
		if (CircleOfLbr[Arc]->ElementsPassedInArray < CircleOfLbr[Arc]->MaxElementsInArray) { LBRMnemonicWriter(Private); }
		else
		{
			CircleOfLbr[Arc]->Reading = true;
			if (Arc < Arcs) { ++Arc; LBRMnemonicWriter(Private); }
			else { Arc = 0; LBRMnemonicWriter(Private); }
		}
	}
}

extern std::vector<ULONG_PTR> AddrsFuncs;

void Trace::TraceMnemonic(_In_ SVM::PRIVATE_VM_DATA* Private)
{
	if (Private->Guest.StateSaveArea.Rip < ASLRSystemAddr)
	{
		WasInSysFunc = false;

		MnemonicHandler(Private);
	}
	else
	{
		if (!WasInSysFunc)
		{
			WasInSysFunc = true;
			for (ULONG_PTR Addr = 0; Addr < AddrsFuncs.size(); ++Addr)
			{
				if (AddrsFuncs[Addr] == Private->Guest.StateSaveArea.Rip)
				{
					MnemonicHandler(Private);
				}
			}
		}
	}
}

void Trace::TraceLBRMnemonic(_In_ SVM::PRIVATE_VM_DATA* Private)
{
	if (Private->Guest.StateSaveArea.Rip < ASLRSystemAddr)
	{
		WasInSysFunc = false;

		LbrMnemonicHandler(Private);
	}
	else
	{
		if (!WasInSysFunc)
		{
			WasInSysFunc = true;
			for (ULONG_PTR Addr = 0; Addr < AddrsFuncs.size(); ++Addr)
			{
				if (AddrsFuncs[Addr] == Private->Guest.StateSaveArea.Rip)
				{
					LbrMnemonicHandler(Private);
				}
			}
		}
	}
}

void Trace::TraceFinalization() { for (auto& Arc : CircleOfMnemonics) { if (Arc->ElementsPassedInArray > 0) { Arc->Reading = true; } } EndOfFile = true; }

void Trace::TraceLBRFinalization() { for (auto& Arc : CircleOfLbr) { if (Arc->ElementsPassedInArray > 0) { Arc->Reading = true; } } EndOfFile = true; }