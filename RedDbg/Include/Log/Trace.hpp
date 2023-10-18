#pragma once

#include "Zydis/Zydis.h"
#include "HyperVisor/CommonTypes/SVM.hpp"
#include "HyperVisor/CommonApi/Spinlock.hpp"
#include "Log/GraphViz/GraphVizLanguage.hpp"
#include "Log/File.hpp"

#include <ntddk.h>
#include <vector>
#include <array>

const UINT64 ASLRSystemAddr = 0x00007FF000000000;
const int Arcs = 16;

struct Mnemonic {
	unsigned char Opcodes[ZYDIS_MAX_INSTRUCTION_LENGTH]{};
	uint64_t Address = 0;
	size_t Length = 0;
};

struct LBRMnemonic {
	unsigned char Opcodes[ZYDIS_MAX_INSTRUCTION_LENGTH]{};
	uint64_t BrFrom = 0;
	uint64_t BrTo = 0;
};

template<typename Type>
class TraceMessage
{
public:
	std::array<Type, 15 * 1024 * 1024> objMnemonicArray;
	size_t MaxElementsInArray = (15 * 1024 * 1024) / sizeof(Mnemonic);
	size_t ElementsPassedInArray = 0;
	bool Reading = false;
	//DECLSPEC_ALIGN(PAGE_SIZE) Mnemonic objMnemonic[15 * 1024 * 1024];
};

class Trace : virtual public Spinlock {
private:
	int Arc = 0;
private:
	bool WasInSysFunc = false;
	bool EndOfFile = false;
	
	volatile LONG SvmRootLoggingLock;
public:
	std::vector<TraceMessage<Mnemonic>*> CircleOfMnemonics;
	std::vector<TraceMessage<LBRMnemonic>*> CircleOfLbr;
private:
	Mnemonic MnemonicCreator(_In_ SVM::PRIVATE_VM_DATA* Private);
	void MnemonicWriter(_In_ SVM::PRIVATE_VM_DATA* Private);
	void LBRMnemonicWriter(_In_ SVM::PRIVATE_VM_DATA* Private);

	void MnemonicHandler(_In_ SVM::PRIVATE_VM_DATA* Private);
	void LbrMnemonicHandler(_In_ SVM::PRIVATE_VM_DATA* Private);
public:
	void TraceMnemonic(_In_ SVM::PRIVATE_VM_DATA* Private);
	void TraceLBRMnemonic(_In_ SVM::PRIVATE_VM_DATA* Private);

	void AcceptGraphMessage(File& objFile);
	void AcceptGraphLBRMessage(File& objFile);

	//template<typename Type>
	//bool TraceInitializeMnemonic(std::vector<TraceMessage<Type>*> Circle);
	bool TraceInitializeMnemonic();
	void TraceFinalization();
	void TraceLBRFinalization();
};