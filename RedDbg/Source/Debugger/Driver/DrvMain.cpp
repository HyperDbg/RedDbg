#include <ntddk.h>

#include "Debugger/Driver/GuestContext.hpp"
#include "Zydis/Zydis.h"
#include "HyperVisor/HyperVisor.hpp"
#include "Log/Logger.hpp"
#include "Log/File.hpp"
#include "Log/Trace.hpp"
#include "IOCTL/IOCTL.hpp"
#include "Local/Functions.hpp"

#include <cstdarg>
#include <stdio.h>
#include <memory.h>
#include <stdio.h>
#include <vector>

HyperVisorSvm objHyperVisorSvm;
Log objLog;
File objFile;
Trace objTrace;
Transparent objTransparent;

bool TransparentMode = true;
bool bTrapFlagHandler = false;

uint64_t CounterOfInstrs = 0;
uint64_t FirstSize = 0;
uint64_t SizeOfAll = 0;
std::vector<ULONG_PTR> AddrsFuncs;
std::vector<std::pair<ULONG_PTR, const std::string>>* Funcs;
ULONG_PTR ExitProcesAddr; 
ULONGLONG freeMemory = 0;

#define LogInfo(format, ...)  \
    objLog.LogSendMessageToQueue(OPERATION_LOG_INFO_MESSAGE, false, false, "[+] Information (%s:%d) | " format "\n",	\
		 __func__, __LINE__, __VA_ARGS__)

#define LogInfoImmediate(format, ...)  \
    objLog.LogSendMessageToQueue(OPERATION_LOG_INFO_MESSAGE, TRUE, false, "[+] Information (%s:%d) | " format "\n",	\
		 __func__, __LINE__, __VA_ARGS__)

#define LogWarning(format, ...)  \
    objLog.LogSendMessageToQueue(OPERATION_LOG_WARNING_MESSAGE, TRUE, false, "[-] Warning (%s:%d) | " format "\n",	\
		__func__, __LINE__, __VA_ARGS__)

#define LogError(format, ...)  \
    objLog.LogSendMessageToQueue(OPERATION_LOG_ERROR_MESSAGE, TRUE, false, "[!] Error (%s:%d) | " format "\n",	\
		 __func__, __LINE__, __VA_ARGS__);	\

#define LogBreakpointImmediate(format, ...)  \
    objLog.LogSendMessageToQueue(OPERATION_LOG_BREAKPOINT_MESSAGE, TRUE, false, "[+] BreakPoint(%s:%d) | " format "\n",	\
		 __func__, __LINE__, __VA_ARGS__);	\

// Log without any prefix
#define Log(format, ...)  \
    objLog.LogSendMessageToQueue(OPERATION_LOG_INFO_MESSAGE, false, false, format "\n", __VA_ARGS__)

SVM::PRIVATE_VM_DATA* Interceptions(
	_Inout_ SVM::PRIVATE_VM_DATA* Private)
{
	Private->Guest.ControlArea.InterceptCpuid = TRUE;
	Private->Guest.ControlArea.InterceptVmrun = TRUE;

	//if (!bTrapFlagHandler)
	//{
	//	Private->Guest.ControlArea.InterceptPushf = TRUE;
	//	Private->Guest.ControlArea.InterceptPopf = TRUE;
	//}

	Private->Guest.ControlArea.InterceptExceptions.Bitmap.InterceptionVectorDB = TRUE;
	Private->Guest.ControlArea.InterceptExceptions.Bitmap.InterceptionVectorBP = TRUE;
	//Private->Guest.ControlArea.InterceptExceptions.Bitmap.InterceptionVectorGP = TRUE;
	Private->Guest.ControlArea.InterceptMsr = TRUE;
	Private->Guest.ControlArea.MsrpmBasePa = reinterpret_cast<UINT64>(PhysicalMemory::GetPhysicalAddress(&Private->Msrpm));

	return Private;
}

void InjectEvent(__out SVM::VMCB* Guest, unsigned char Vector, unsigned char Type, unsigned int Code)
{
	SVM::EVENTINJ Event = {};
	Event.Bitmap.Vector = Vector;
	Event.Bitmap.Type = Type;
	Event.Bitmap.ErrorCodeValid = TRUE;
	Event.Bitmap.Valid = TRUE;
	Event.Bitmap.ErrorCode = Code;
	Guest->ControlArea.EventInjection = Event.Value;
}

void VmFuncSetRflagTrapFlag(_In_ bool Enable, _In_ SVM::PRIVATE_VM_DATA* Private)
{
	Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Bitmap.TF = Enable;
}

void VmFuncSetLBR(_In_ bool Enable, _In_ SVM::PRIVATE_VM_DATA* Private)
{
	Private->Guest.ControlArea.LbrVirtualizationEnable |= (Enable << 0);
	Private->Guest.StateSaveArea.DbgCtl.fields.Btf = Enable;
	Private->Guest.StateSaveArea.DbgCtl.fields.Lbr = Enable;
}

bool BpPass = false;

typedef CPUID_REGS FEATURE_INFORMATION;

extern "C" uint64_t PushfqHandler(_In_ uint64_t Rsp, _In_ bool CSLongMode, _In_ uint64_t RflagsValue, _In_ uint32_t VmFlag, _In_ uint32_t RFlag);
extern "C" uint64_t PushfHandler(_In_ uint64_t Rsp, _In_ bool CSLongMode, _In_ uint64_t RflagsValue, _In_ uint32_t VmFlag, _In_ uint32_t RFlag);

namespace SvmDbg 
{
	ZydisDecoder* SvmDebuggerDecoderMinimal = nullptr;
	Transparent objTransparent;

	class VMMData_ {
	public:
		SVM::PRIVATE_VM_DATA* Private = nullptr;
		GuestContext* Context = nullptr;
		SIMDGuestContext* objSIMDGuestContext = nullptr;

		VMMData_(_In_ SVM::PRIVATE_VM_DATA* PrivateVmData, GuestContext* GContext, SIMDGuestContext* SIMDGuestContext)
		{
			Private = PrivateVmData; Context = GContext; objSIMDGuestContext = SIMDGuestContext;
		}
	};

	bool SvmDebuggerAllocator()
	{
		SvmDebuggerDecoderMinimal = (ZydisDecoder*)ExAllocatePool(NonPagedPoolNx, sizeof(ZydisDecoder));
		if (SvmDebuggerDecoderMinimal != nullptr)
		{
			RtlZeroMemory(SvmDebuggerDecoderMinimal, sizeof(ZydisDecoder));
			SvmDebuggerDecoderMinimal->decoder_mode = ZYDIS_DECODER_MODE_MINIMAL;
			SvmDebuggerDecoderMinimal->machine_mode = ZYDIS_MACHINE_MODE_LONG_64;
			SvmDebuggerDecoderMinimal->stack_width = ZYDIS_STACK_WIDTH_64;
		}
		else { return false; }

		return true;
	}

	VMMData_ LBRHandler(_In_ VMMData_ VMMData)
	{
		if (VMMData.Private->Guest.StateSaveArea.Rip != ExitProcesAddr)
		{
			++CounterOfInstrs;
			VmFuncSetRflagTrapFlag(TRUE, VMMData.Private);
			VmFuncSetLBR(TRUE, VMMData.Private);
		}
		else 
		{
			VmFuncSetRflagTrapFlag(FALSE, VMMData.Private);
			VmFuncSetLBR(FALSE, VMMData.Private);

			KdPrint(("ExitProcessAddress reached!\n"));
			KdPrint(("Count of executed instructions: %p\n", CounterOfInstrs));
		}

		return VMMData;
	}

	VMMData_ TrapFlagHandler(_In_ VMMData_ VMMData)
	{
		__invlpg((void*)VMMData.Private->Guest.StateSaveArea.Rsp);
		if (VMMData.Private->Guest.StateSaveArea.Rip != ExitProcesAddr)
		{
			++CounterOfInstrs;
			VmFuncSetRflagTrapFlag(TRUE, VMMData.Private);
			//objTrace.TraceMnemonic(VMMData.Private);
		}
		else
		{
			//objSpinlock.SpinlockLock((LONG*)&SvmRootLoggingLock);
			//objTrace.TraceFinalization();
			//objSpinlock.SpinlockUnlock((LONG*)&SvmRootLoggingLock);
			VmFuncSetRflagTrapFlag(FALSE, VMMData.Private);

			KdPrint(("ExitProcessAddress reached!\n"));
			KdPrint(("Count of executed instructions: %p\n", CounterOfInstrs));
		}

		if (VMMData.Private->Guest.StateSaveArea.Rip == 0x14F7F4CB0)
		{
			//objTrace.TraceMnemonicFinalization();
			VmFuncSetRflagTrapFlag(FALSE, VMMData.Private);

			KdPrint(("ExitProcessAddress reached!\n"));
			KdPrint(("Count of executed instructions: %p\n", CounterOfInstrs));
		}

		if (((const unsigned char*)VMMData.Private->Guest.StateSaveArea.Rip) != nullptr &&
			VMMData.Private->Guest.StateSaveArea.Cpl == 3)
		{
			ZydisDecodedInstruction SvmDebuggerInstruction;
			ZydisDecoderDecodeInstruction(
				SvmDebuggerDecoderMinimal,
				NULL,
				(const unsigned char*)VMMData.Private->Guest.StateSaveArea.Rip,
				ZYDIS_MAX_INSTRUCTION_LENGTH,
				&SvmDebuggerInstruction);

			if (SvmDebuggerInstruction.mnemonic == ZYDIS_MNEMONIC_PUSHFQ)
			{
				VMMData.Private->Guest.StateSaveArea.Rsp = PushfqHandler(
					VMMData.Private->Guest.StateSaveArea.Rsp,
					VMMData.Private->Guest.StateSaveArea.Cs.Attrib.Bitmap.LongMode,
					VMMData.Private->Guest.StateSaveArea.Rflags.Value,
					VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Bitmap.VM,
					VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Bitmap.RF);

				if (!VMMData.Private->Guest.ControlArea.NextRip) { VMMData.Private->Guest.StateSaveArea.Rip += SvmDebuggerInstruction.length; }
				else { VMMData.Private->Guest.ControlArea.NextRip += SvmDebuggerInstruction.length; }
			}
			else if (SvmDebuggerInstruction.mnemonic == ZYDIS_MNEMONIC_PUSHF)
			{
				VMMData.Private->Guest.StateSaveArea.Rsp = PushfHandler(
					VMMData.Private->Guest.StateSaveArea.Rsp,
					VMMData.Private->Guest.StateSaveArea.Cs.Attrib.Bitmap.LongMode,
					VMMData.Private->Guest.StateSaveArea.Rflags.Value,
					VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Bitmap.VM,
					VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Bitmap.RF);
				if (!VMMData.Private->Guest.ControlArea.NextRip) { VMMData.Private->Guest.StateSaveArea.Rip += SvmDebuggerInstruction.length; }
				else { VMMData.Private->Guest.ControlArea.NextRip += SvmDebuggerInstruction.length; }
			}
		}

		return VMMData;
	}

	bool Active = false;
	bool Crash = false;
	uint64_t ProCr3 = 0;
	HANDLE TargetProcId = 0;

	extern "C" SVM::VMM_STATUS SvmExitHandler(
		_Inout_ SVM::PRIVATE_VM_DATA* Private,
		_Inout_ GuestContext* Context)
	{
		//SIMD SAVE
		SIMDGuestContext objSIMDGuestContext{};
		SIMDGuestContext* PobjSIMDGuestContext = &objSIMDGuestContext;
		_fxsave64(PobjSIMDGuestContext);

		// Load the host state:
		__svm_vmload(reinterpret_cast<size_t>(Private->VmmStack.Layout.InitialStack.HostVmcbPa));
		// Restore the guest's RAX that was overwritten by host's RAX on #VMEXIT:
		Context->Rax = Private->Guest.StateSaveArea.Rax;

		auto VMROOTCR3 = __readcr3();
		__writecr3(((CR3)Private->Guest.StateSaveArea.Cr3).Value);

		VMMData_ VMMData(Private, Context, PobjSIMDGuestContext);

		SVM::VMM_STATUS Status = SVM::VMM_STATUS::VMM_CONTINUE;
		//RDTSC 110
		//RDTSCP 135
		switch (VMMData.Private->Guest.ControlArea.ExitCode)
		{
		case SVM::SVM_EXIT_CODE::VMEXIT_EXCP_DB://65
		{
			if (!bTrapFlagHandler)
			{
				//KdPrint(("---------------Rip: %p\n", VMMData.Private->Guest.StateSaveArea.Rip));
				//KdPrint(("Lbr & Btf: %p\n", VMMData.Private->Guest.StateSaveArea.DbgCtl));
				//KdPrint(("BrFrom: %p\n", VMMData.Private->Guest.StateSaveArea.BrFrom));
				//KdPrint(("BrTo: %p\n", VMMData.Private->Guest.StateSaveArea.BrTo));
				//if (VMMData.Private->Guest.StateSaveArea.BrFrom == 0x1429F2F2D)
				//{
				//	KdPrint(("Rsp data(RET TO): %p\n", VMMData.Private->Guest.StateSaveArea.Rip));
				//	Crash = true;
				//}
				//if (Crash)
				//{
				//	KdPrint(("BrFrom: %p\n", VMMData.Private->Guest.StateSaveArea.BrFrom));
				//	KdPrint(("BrTo: %p\n", VMMData.Private->Guest.StateSaveArea.BrTo));
				//}
				VMMData = LBRHandler(VMMData);

				VMMData.Private->Guest.StateSaveArea.Dr7.x64.Bitmap.G0 = 0;
				VMMData.Private->Guest.StateSaveArea.Dr6.x64.Bitmap.BS = 0;
			}
			else
			{
				VMMData = TrapFlagHandler(VMMData);
			}

			//VmFuncSetLBR(TRUE, VMMData.Private);
			//VmFuncSetRflagTrapFlag(TRUE, VMMData.Private);
			//VMMData.Private->Guest.StateSaveArea.Dr7.x64.Bitmap.G0 = 0;
			//VMMData.Private->Guest.StateSaveArea.Dr6.x64.Bitmap.BS = 0;
			/*
			KdPrint(("----------------BrFrom: %p\n", VMMData.Private->Guest.StateSaveArea.BrFrom));
			KdPrint(("RAX %p\n", Private->Guest.StateSaveArea.Rax));
			KdPrint(("RBX %p\n", Context->Rbx));
			KdPrint(("RCX %p\n", Context->Rcx));
			KdPrint(("RDX %p\n", Context->Rdx));
			KdPrint(("RBP %p\n", Context->Rbp));
			KdPrint(("RSP %p\n", Private->Guest.StateSaveArea.Rsp));
			KdPrint(("RSI %p\n", Context->Rsi));
			KdPrint(("RDI %p\n", Context->Rdi));
			KdPrint(("R8 %p\n", Context->R8));
			KdPrint(("R9 %p\n", Context->R9));
			KdPrint(("R10 %p\n", Context->R10));
			KdPrint(("R11 %p\n", Context->R11));
			KdPrint(("R12 %p\n", Context->R12));
			KdPrint(("R13 %p\n", Context->R13));
			KdPrint(("R14 %p\n", Context->R14));
			KdPrint(("R15 %p\n", Context->R15));
			KdPrint(("Rip: %p\n", VMMData.Private->Guest.StateSaveArea.Rip));
			KdPrint(("BrTo: %p\n", VMMData.Private->Guest.StateSaveArea.BrTo));
			KdPrint(("LBR VIRT: %p\n", Private->Guest.ControlArea.LbrVirtualizationEnable));
			*/
			/*
			else
			{
				//KdPrint(("OF DR7 %p\n", VMMData.Private->Guest.StateSaveArea.Dr7));
				//KdPrint(("OF DR6: %p\n", VMMData.Private->Guest.StateSaveArea.Dr6));
				VmFuncSetLBR(TRUE, VMMData.Private);
				VmFuncSetRflagTrapFlag(TRUE, VMMData.Private);
				Active = true;
				TargetProcId = PsGetCurrentProcessId();
				KdPrint(("----------------BrFrom: %p\n", VMMData.Private->Guest.StateSaveArea.BrFrom));
				KdPrint(("OF RAX %p\n", Private->Guest.StateSaveArea.Rax));
				KdPrint(("OF RBX %p\n", Context->Rbx));
				KdPrint(("OF RCX %p\n", Context->Rcx));
				KdPrint(("OF RDX %p\n", Context->Rdx));
				KdPrint(("OF RBP %p\n", Context->Rbp));
				KdPrint(("OF RSP %p\n", Private->Guest.StateSaveArea.Rsp));
				KdPrint(("OF RSI %p\n", Context->Rsi));
				KdPrint(("OF RDI %p\n", Context->Rdi));
				KdPrint(("OF R8 %p\n", Context->R8));
				KdPrint(("OF R9 %p\n", Context->R9));
				KdPrint(("OF R10 %p\n", Context->R10));
				KdPrint(("OF R11 %p\n", Context->R11));
				KdPrint(("OF R12 %p\n", Context->R12));
				KdPrint(("OF R13 %p\n", Context->R13));
				KdPrint(("OF R14 %p\n", Context->R14));
				KdPrint(("OF R15 %p\n", Context->R15));
				KdPrint(("OF Rip: %p\n", VMMData.Private->Guest.StateSaveArea.Rip));
				KdPrint(("BrTo: %p\n", VMMData.Private->Guest.StateSaveArea.BrTo));

				VMMData.Private->Guest.StateSaveArea.Dr7.x64.Bitmap.G0 = 0;
				VMMData.Private->Guest.StateSaveArea.Dr6.x64.Bitmap.BS = 0;
			}
			*/
			/*
			DR6 dr6 = (DR6)VMMData.Private->Guest.StateSaveArea.Dr6;

			if (dr6.x64.Bitmap.BS == 1)
			{
				if (Active)
				{
					InjectEvent(&Private->Guest, INTERRUPT_VECTOR::Debug, EXCEPTION_VECTOR::FaultTrapException, 0);
				}
				else
				{
					InjectEvent(&Private->Guest, INTERRUPT_VECTOR::Debug, EXCEPTION_VECTOR::FaultTrapException, 0);
				}
				VmFuncSetLBR(FALSE, VMMData.Private);
				VmFuncSetRflagTrapFlag(FALSE, VMMData.Private);
				KdPrint(("FINNNNNNNNNNNNNNN\n"));
			}
			else
			{
				InjectEvent(&Private->Guest, INTERRUPT_VECTOR::Debug, EXCEPTION_VECTOR::FaultTrapException, 0);
			}
			*/
			/*
			if (!bTrapFlagHandler)
			{
				KdPrint(("Rip: %p\n", VMMData.Private->Guest.StateSaveArea.Rip));
				KdPrint(("Lbr & Btf: %p\n", VMMData.Private->Guest.StateSaveArea.DbgCtl));
				KdPrint(("BrFrom: %p\n", VMMData.Private->Guest.StateSaveArea.BrFrom));
				KdPrint(("BrTo: %p\n", VMMData.Private->Guest.StateSaveArea.BrTo));
				VMMData = LBRHandler(VMMData);
			}
			else
			{
				VMMData = TrapFlagHandler(VMMData);
			}*/

			break;
		}
		case SVM::SVM_EXIT_CODE::VMEXIT_EXCP_BP://67
		{
			VmFuncSetLBR(TRUE, VMMData.Private);
			VmFuncSetRflagTrapFlag(TRUE, VMMData.Private);
			ProCr3 = VMMData.Private->Guest.StateSaveArea.Cr3;
			Active = true;
			BpPass = true;
			break;
		}
		case SVM::SVM_EXIT_CODE::VMEXIT_EXCP_GP:
		{
			KdPrint(("BrTo: %p\n", VMMData.Private->Guest.StateSaveArea.BrTo));
			break;
		}
		/*
		case SVM::SVM_EXIT_CODE::VMEXIT_PUSHF://112
		{
			//HANDLE ID = PsGetCurrentProcessId();
			//__invlpg((void*)Private->Guest.StateSaveArea.Rsp);

			if (VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Bitmap.VM)
			{
				KeBugCheck(MANUALLY_INITIATED_CRASH);
			}

			if (*(uint8_t*)VMMData.Private->Guest.StateSaveArea.Rip == 0x66)
			{
				VMMData.Private->Guest.StateSaveArea.Rsp -= sizeof(uint16_t);
				
				*(uint16_t*)VMMData.Private->Guest.StateSaveArea.Rsp = (VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Value & UINT16_MAX);
				//KdPrint(("PIZDEC uint16_t\n"));
			}
			else if (VMMData.Private->Guest.StateSaveArea.Cs.Attrib.Bitmap.LongMode)
			{
				VMMData.Private->Guest.StateSaveArea.Rsp -= sizeof(uintptr_t);
				*(uint64_t*)VMMData.Private->Guest.StateSaveArea.Rsp = VMMData.Private->Guest.StateSaveArea.Rflags.Value;
			}
			else if (!VMMData.Private->Guest.StateSaveArea.Cs.Attrib.Bitmap.LongMode)
			{
				VMMData.Private->Guest.StateSaveArea.Rsp -= sizeof(uint32_t);
				*(uint32_t*)VMMData.Private->Guest.StateSaveArea.Rsp = VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Value;

				VMMData.Private->Guest.StateSaveArea.Rip = VMMData.Private->Guest.ControlArea.NextRip;
			}
			//else
			//{
			//	KdPrint(("PIZDEC\n"));
			//}

			((EFLAGS*)VMMData.Private->Guest.StateSaveArea.Rsp)->Bitmap.RF = 0;
			((EFLAGS*)VMMData.Private->Guest.StateSaveArea.Rsp)->Bitmap.VM = 0;

			if (Active && VMMData.Private->Guest.StateSaveArea.Cpl == 3 && VMMData.Private->Guest.StateSaveArea.Cr3 == ProCr3)
			{
				KdPrint(("PUSHF Rip: %p\n", VMMData.Private->Guest.StateSaveArea.Rip));
				KdPrint(("PUSHF Rsp after: %p\n", VMMData.Private->Guest.StateSaveArea.Rsp));
				((EFLAGS*)VMMData.Private->Guest.StateSaveArea.Rsp)->Bitmap.TF = 0;
			}

			__writecr3(VMROOTCR3);
			if (!Private->Guest.ControlArea.NextRip)
			{
				Private->Guest.StateSaveArea.Rax = Context->Rax;
				return Status;
			}
			else
			{
				Private->Guest.StateSaveArea.Rax = Context->Rax;

				// Go to the next instruction:
				Private->Guest.StateSaveArea.Rip = Private->Guest.ControlArea.NextRip;
				return Status;
			}

			//break;
		}
		case SVM::SVM_EXIT_CODE::VMEXIT_POPF://113
		{
			//HANDLE ID = PsGetCurrentProcessId();
			//__invlpg((void*)VMMData.Private->Guest.StateSaveArea.Rsp);

			uint32_t unchanged_mask = 0x00100000 | 0x00080000 | 0x00020000;
			//VIP && VIF && VM

			if (VMMData.Private->Guest.StateSaveArea.Cpl > 0)
			{
				unchanged_mask |= 0x3000; //IOPL
				if (VMMData.Private->Guest.StateSaveArea.Cpl > VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Bitmap.IOPL)
				{
					unchanged_mask |= 0x0200;//Interrupt enable flag
				}
			}

			RFLAGS StackRFlag{ 0 };
			uint32_t OperandSize = sizeof(uint32_t);

			StackRFlag.Bitmap.Eflags.Value = *(uint32_t*)VMMData.Private->Guest.StateSaveArea.Rsp;

			if (*(uint8_t*)VMMData.Private->Guest.StateSaveArea.Rip == 0x66)
			{
				OperandSize = sizeof(uint16_t);
				StackRFlag.Value = *(uint16_t*)VMMData.Private->Guest.StateSaveArea.Rsp;

				StackRFlag.Value = (uint16_t)StackRFlag.Value | (VMMData.Private->Guest.StateSaveArea.Rflags.Value & 0xffff0000u);
			}
			else if (VMMData.Private->Guest.StateSaveArea.Cs.Attrib.Bitmap.LongMode)
			{
				OperandSize = sizeof(uint64_t);
				StackRFlag.Value = *(uint64_t*)VMMData.Private->Guest.StateSaveArea.Rsp;
			}

			StackRFlag.Value &= 0x257fd5;
			StackRFlag.Value &= unchanged_mask;
			VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Value |= (uint32_t)(StackRFlag.Bitmap.Eflags.Value & ~unchanged_mask) | 0x2;

			if (Active && VMMData.Private->Guest.StateSaveArea.Cr3 == ProCr3)
			{
				KdPrint(("POPF Rip: %p\n", VMMData.Private->Guest.StateSaveArea.Rip));
				KdPrint(("POPF Rsp after: %p\n", VMMData.Private->Guest.StateSaveArea.Rsp));
				((EFLAGS*)VMMData.Private->Guest.StateSaveArea.Rsp)->Bitmap.TF = 1;
			}

			VMMData.Private->Guest.StateSaveArea.Rflags.Bitmap.Eflags.Bitmap.RF = 0;
			VMMData.Private->Guest.StateSaveArea.Rsp += OperandSize;

			Private = VMMData.Private;
			Context = VMMData.Context;

			__writecr3(VMROOTCR3);
			if (!Private->Guest.ControlArea.NextRip)
			{
				Private->Guest.StateSaveArea.Rax = Context->Rax;
				return Status;
			}
			else
			{
				Private->Guest.StateSaveArea.Rax = Context->Rax;

				// Go to the next instruction:
				Private->Guest.StateSaveArea.Rip = Private->Guest.ControlArea.NextRip;
				return Status;
			}

			//break;
		}
		*/
		case SVM::SVM_EXIT_CODE::VMEXIT_CPUID://114
		{
			CPUID_REGS Regs = {};
			int Function = static_cast<int>(VMMData.Context->Rax);
			int SubLeaf = static_cast<int>(VMMData.Context->Rcx);
			__cpuidex(Regs.Raw, Function, SubLeaf);

			if (Function == CPUID_VMM_SHUTDOWN) { Status = SVM::VMM_STATUS::VMM_SHUTDOWN; }
			else if (TransparentMode && Function == CPUID::Generic::CPUID_EXTENDED_FEATURE_INFORMATION)
			{
				CPUID::FEATURE_INFORMATION RegsFeature = (CPUID::FEATURE_INFORMATION)Regs;
				VMMData.Context = objTransparent.InvisibleCpuid(RegsFeature, VMMData.Context);
			}
			else
			{
				VMMData.Context->Rax = Regs.Regs.Eax;
				VMMData.Context->Rbx = Regs.Regs.Ebx;
				VMMData.Context->Rcx = Regs.Regs.Ecx;
				VMMData.Context->Rdx = Regs.Regs.Edx;
			}
			break;
		}
		case SVM::SVM_EXIT_CODE::VMEXIT_MSR://124
		{
			if ((VMMData.Context->Rcx & MAXUINT32) == static_cast<unsigned int>(AMD::AMD_MSR::MSR_EFER) && VMMData.Private->Guest.ControlArea.ExitInfo1)
			{
				AMD::EFER Efer = {};
				Efer.Value = ((VMMData.Context->Rdx & MAXUINT32) << 32) | (VMMData.Context->Rax & MAXUINT32);
				if (!Efer.Bitmap.SecureVirtualMachineEnable)
				{
					InjectEvent(&VMMData.Private->Guest, INTERRUPT_VECTOR::GeneralProtection, EXCEPTION_VECTOR::FaultTrapException, 0); // #GP (Vector = 13, Type = Exception)
					break;
				}
				VMMData.Private->Guest.StateSaveArea.Efer = Efer.Value;
			}
			break;
		}
		case SVM::SVM_EXIT_CODE::VMEXIT_VMRUN://128
		{
			InjectEvent(&Private->Guest, INTERRUPT_VECTOR::GeneralProtection, EXCEPTION_VECTOR::FaultTrapException, 0); // #GP (Vector = 13, Type = Exception)
			break;
		}
		}

		PobjSIMDGuestContext = VMMData.objSIMDGuestContext;
		//SIMD RESTORE
		_fxrstor64(PobjSIMDGuestContext);

		Private = VMMData.Private;
		Context = VMMData.Context;

		//VMCB CLEAN AND TLB CONTROL TRANSFER
		Private->Guest.ControlArea.VmcbCleanBits &= 0xFFFFFFEF;
		Private->Guest.ControlArea.TlbControl.Bitmap.TlbControl = 1;

		__writecr3(VMROOTCR3);
		if (Private->Guest.ControlArea.ExitCode == SVM::SVM_EXIT_CODE::VMEXIT_EXCP_BP || !Private->Guest.ControlArea.NextRip)
		{
			Private->Guest.StateSaveArea.Rax = Context->Rax;
			return Status;
		}
		else
		{
			Private->Guest.StateSaveArea.Rax = Context->Rax;

			// Go to the next instruction:
			Private->Guest.StateSaveArea.Rip = Private->Guest.ControlArea.NextRip;
			return Status;
		}
	}
}

//Define in asm file(in my example)
extern "C" void SvmVmmRun(_In_ void* InitialVmmStackPointer);

void DrvUnload(_In_ PDRIVER_OBJECT DriverObj)
{
	UNREFERENCED_PARAMETER(DriverObj);
	IoDeleteDevice(DriverObj->DeviceObject);
}

NTSTATUS DrvUnsupported(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DrvCreate(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	/*
	objHyperVisorSvm.PInterceptions = &Interceptions;
	objHyperVisorSvm.PSvmVmmRun = &SvmVmmRun;

	if (objHyperVisorSvm.IsSvmSupported()) 
	{ 
		objFile.CreateFile(
			//L"\\??\\E:\\Trace.out",
			L"\\??\\H:\\Trace.out",
			FILE_GENERIC_WRITE,
			FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			FILE_OVERWRITE_IF,
			FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT);
		objHyperVisorSvm.VirtualizeAllProcessors(); 
	}
	*/

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

//#define IOCTL_GET_HEAPS \
//   CTL_CODE( FILE_DEVICE_UNKNOWN, 0x809, METHOD_BUFFERED, FILE_ANY_ACCESS )
//
//#define IOCTL_GET_HEAP \
//   CTL_CODE( FILE_DEVICE_UNKNOWN, 0x808, METHOD_BUFFERED, FILE_ANY_ACCESS )

NTSTATUS DrvDispatchIoControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION IrpStack;
	IoCtlDispatcher_ IoCtlDispatcher;
	PREGISTER_EVENT RegisterEvent;
	NTSTATUS Status = STATUS_SUCCESS;
	UNREFERENCED_PARAMETER(DeviceObject);

	IrpStack = IoGetCurrentIrpStackLocation(Irp);
	ULONG InBuffLength;
	ULONG OutBuffLength;

	switch (IrpStack->Parameters.DeviceIoControl.IoControlCode)
	{
	case IoCtlCode::GetMemoryMapProc:
	{
		PUSERMODE_LOADED_MODULE_DETAILS DebuggerUsermodeModulesRequest = nullptr;
		{
			// First validate the parameters.
			if (IrpStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(USERMODE_LOADED_MODULE_DETAILS) || Irp->AssociatedIrp.SystemBuffer == NULL)
			{
				Status = STATUS_INVALID_PARAMETER;
				LogError("Err, invalid parameter to IOCTL dispatcher");
				break;
			}

			InBuffLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

			if (!InBuffLength || !OutBuffLength)
			{
				Status = STATUS_INVALID_PARAMETER;
				break;
			}
		}

		// Both usermode and to send to usermode and the coming buffer are
		// at the same place
		DebuggerUsermodeModulesRequest = (PUSERMODE_LOADED_MODULE_DETAILS)Irp->AssociatedIrp.SystemBuffer;

		IoCtlDispatcher.ProcessUserLevel.GetMemoryMap(DebuggerUsermodeModulesRequest, OutBuffLength);
		
		Irp->IoStatus.Information = OutBuffLength;
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;
		break;
	}
	case IoCtlCode::GetMemoryMapSystem:
	{
		InBuffLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
		OutBuffLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

		if (!InBuffLength || !OutBuffLength)
		{
			Status = STATUS_INVALID_PARAMETER;
			break;
		}

		IoCtlDispatcher.SystemKernelLevel.GetMemoryMap();

		Irp->IoStatus.Information = OutBuffLength;
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;
		break;
	}
	case IoCtlCode::GetProcessHeap: __fallthrough;
	case IoCtlCode::GetProcessHeaps:
	{
		PUSERMODE_HEAP_DETAILS DebuggerUserModeHeapsRequest = nullptr;
		{
			// First validate the parameters.
			if (IrpStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(USERMODE_HEAP_DETAILS) || Irp->AssociatedIrp.SystemBuffer == NULL)
			{
				Status = STATUS_INVALID_PARAMETER;
				DbgPrint("Err, invalid parameter to IOCTL dispatcher: %x && SystemBuffer = %p", IrpStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(USERMODE_HEAP_DETAILS), Irp->AssociatedIrp.SystemBuffer);
				break;
			}

			InBuffLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

			if (!InBuffLength || !OutBuffLength)
			{
				Status = STATUS_INVALID_PARAMETER;
				break;
			}
		}

		// Both usermode and to send to usermode and the coming buffer are
		// at the same place
		DebuggerUserModeHeapsRequest = (PUSERMODE_HEAP_DETAILS)Irp->AssociatedIrp.SystemBuffer;

		if (IrpStack->Parameters.DeviceIoControl.IoControlCode == IoCtlCode::GetProcessHeaps)
		{
			IoCtlDispatcher.ProcessUserLevel._GetProcessHeaps(DebuggerUserModeHeapsRequest);
		}
		else
		{
			IoCtlDispatcher.ProcessUserLevel._GetProcessHeap(DebuggerUserModeHeapsRequest);
		}

		Irp->IoStatus.Information = OutBuffLength;
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;
		break;
	}
	case IoCtlCode::ReadProcessMemory:
	{
		//DbgBreakPoint();
		PUSERMODE_READ_PROCESS_MEMORY DebuggerUserModeReadMemoryOfProcess = nullptr;
		KAPC_STATE State;
		{
			// First validate the parameters.
			if (IrpStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(USERMODE_READ_PROCESS_MEMORY) || Irp->AssociatedIrp.SystemBuffer == NULL)
			{
				Status = STATUS_INVALID_PARAMETER;
				DbgPrint("Err, invalid parameter to IOCTL dispatcher: %x && SystemBuffer = %p", IrpStack->Parameters.DeviceIoControl.InputBufferLength < sizeof(USERMODE_READ_PROCESS_MEMORY), Irp->AssociatedIrp.SystemBuffer);
				break;
			}

			InBuffLength = IrpStack->Parameters.DeviceIoControl.InputBufferLength;
			OutBuffLength = IrpStack->Parameters.DeviceIoControl.OutputBufferLength;

			if (!InBuffLength || !OutBuffLength)
			{
				Status = STATUS_INVALID_PARAMETER;
				break;
			}
		}

		// Both usermode and to send to usermode and the coming buffer are
		// at the same place
		DebuggerUserModeReadMemoryOfProcess = (PUSERMODE_READ_PROCESS_MEMORY)Irp->AssociatedIrp.SystemBuffer;

		auto BitnessOfProcess = IoCtlDispatcher.ProcessUserLevel.GetUserModeProcessBitness(DebuggerUserModeReadMemoryOfProcess->Pid);

		KeStackAttachProcess((PRKPROCESS)BitnessOfProcess->SourceProcess, &State);
		VirtualMemory::CopyMemory(DebuggerUserModeReadMemoryOfProcess->Dest, DebuggerUserModeReadMemoryOfProcess->Src, DebuggerUserModeReadMemoryOfProcess->Size);
		KeUnstackDetachProcess(&State);
	}
	/*
	case IOCTL_REGISTER_EVENT:
	{
		if (IrpStack->Parameters.DeviceIoControl.InputBufferLength < SIZEOF_REGISTER_EVENT || Irp->AssociatedIrp.SystemBuffer == NULL) {
			Status = STATUS_INVALID_PARAMETER;
			break;
		}

		RegisterEvent = (PREGISTER_EVENT)Irp->AssociatedIrp.SystemBuffer;

		switch (RegisterEvent->Type)
		{
		case IRP_BASED:
			Status = objLog.LogRegisterIrpBasedNotification(DeviceObject, Irp);
			break;
		case EVENT_BASED:
			Status = objLog.LogRegisterEventBasedNotification(DeviceObject, Irp);
			break;
		default:
			Status = STATUS_INVALID_PARAMETER;
			break;
		}
		break;
	}
	case IOCTL_BUFFER_CHECK:
	{
		RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, &objLog.BufferIsReady, sizeof(bool));

		Irp->IoStatus.Information = sizeof(bool);
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;

		break;
	}
	case IOCTL_WRITE:
	{
		//objTrace.AcceptRipMessage(objFile);
		//objTrace.AcceptMnemonicMessage(objFile);
		//objTrace.AcceptGraphMessage(objFile);
		objTrace.AcceptGraphLBRMessage(objFile);
		//objTrace.AcceptGraphCycleFoldingMessage(objFile);
		//while (TRUE)
		//{
		//	objTrace.AcceptCombineBufferRipMessage(objFile, objTrace.MainBuffer);
		//
		////	if(objTrace.Exit) { objTrace.AcceptCombineBufferRipMessage(objFile, objTrace.MainBuffer); }
		//}
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;

		break;
	}
	case IOCTL_CLOSE_FILE:
	{
		objFile.CloseFile();

		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;

		break;
	}
	case IOCTL_BREAKPOINT_PASS:
	{
		RtlCopyBytes(Irp->AssociatedIrp.SystemBuffer, &BpPass, sizeof(bool));
		Irp->IoStatus.Information = sizeof(bool);
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;

		break;
	}
	case IOCTL_BUFFER_CHECK_SUCCESS:
	{
		objLog.BufferIsReady = FALSE;
		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;

		break;
	}
	case IOCTL_PE_ANALYZE:
	{
		Funcs = (std::vector<std::pair<ULONG_PTR, const std::string>>*)Irp->AssociatedIrp.SystemBuffer;
		if (Funcs && Funcs->size() > 0)
		{
			for (const auto& pair : *Funcs)
			{
				if (pair.second == "ExitProcess") { ExitProcesAddr = pair.first; }
				AddrsFuncs.push_back(pair.first);
			}
		}
		else { KdPrint(("Funcs is empty or nullptr\n")); }

		Irp->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		Status = STATUS_SUCCESS;

		break;
	}
	*/
	}
	return Status;
}

NTSTATUS DrvClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);

	__crt_deinit();

	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}


extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegisterPath)
{
	UNREFERENCED_PARAMETER(RegisterPath); NTSTATUS Ntstatus = STATUS_SUCCESS;
	PDEVICE_OBJECT DeviceObject = NULL; UNICODE_STRING DriverName, DosDeviceName;
	__crt_init();

	UnexportedFunctions_ UnexportedFunctions;
	//if (!objLog.LogInitialize()) { DbgPrint("[*] Log buffer is not initialized !\n"); DbgBreakPoint(); }
	//if (!objTrace.TraceInitializeMnemonic()) { DbgPrint("[*] Trace buffer is not initialized !\n"); DbgBreakPoint(); }
	//if (!SvmDbg::SvmDebuggerAllocator()) { DbgPrint("[*] SvmExitCodesAllocator buffer is not initialized !\n"); DbgBreakPoint(); }
	//if (!SvmExitCodesAllocator()) { DbgPrint("[*] SvmExitCodesAllocator buffer is not initialized !\n"); DbgBreakPoint(); }

	RtlInitUnicodeString(&DriverName, L"\\Device\\RedDbgHyperVisor");
	RtlInitUnicodeString(&DosDeviceName, L"\\DosDevices\\RedDbgHyperVisor");
	
	Ntstatus = IoCreateDevice(DriverObject, 0, &DriverName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &DeviceObject);
	if (Ntstatus == STATUS_SUCCESS)
	{
		if (FlagOn(DeviceObject->Flags, DO_BUFFERED_IO)) {
			SetFlag(DeviceObject->Flags, DO_BUFFERED_IO);
		}
		if (FlagOn(DeviceObject->Flags, DO_DIRECT_IO)) {
			SetFlag(DeviceObject->Flags, DO_DIRECT_IO);
		}

		for (UINT64 Index = 0; Index < IRP_MJ_MAXIMUM_FUNCTION; Index++) { DriverObject->MajorFunction[Index] = DrvUnsupported; }

		DriverObject->MajorFunction[IRP_MJ_CLOSE] = DrvClose;
		DriverObject->MajorFunction[IRP_MJ_CREATE] = DrvCreate;
		DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DrvDispatchIoControl;
		DriverObject->DriverUnload = DrvUnload;
		IoCreateSymbolicLink(&DosDeviceName, &DriverName);
	}

	return STATUS_SUCCESS;
}