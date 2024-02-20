#include "Debugger/Driver/SvmVmExitHandler.hpp"

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

namespace SvmDbg
{
	extern HyperVisorSvm objHyperVisorSvm;

	namespace Trace
	{
		PTracer BranchTracer;
		PTracer InstructionTracer;

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

		void TrapOnHandler(_Inout_ VMMData_& VMMData)
		{
			if (VMMData.Private->Guest.StateSaveArea.Rip != BranchTracer->StopAddr)
			{
				VmFuncSetRflagTrapFlag(TRUE, VMMData.Private);
			}
			else
			{
				VmFuncSetRflagTrapFlag(FALSE, VMMData.Private);
			}
		}

		void LBROnHandler(_Inout_ VMMData_& VMMData)
		{
			if (VMMData.Private->Guest.StateSaveArea.Rip != BranchTracer->StopAddr)
			{
				VmFuncSetRflagTrapFlag(TRUE, VMMData.Private);
				VmFuncSetLBR(TRUE, VMMData.Private);
			}
			else 
			{
				VmFuncSetRflagTrapFlag(FALSE, VMMData.Private);
				VmFuncSetLBR(FALSE, VMMData.Private);
			}

			return;
		}

		void StartBranch(_Inout_ VMMData_& VMMData)
		{
			
		}

		void StartInstruction(_Inout_ VMMData_& VMMData)
		{

		}
	}
	/*  VmmcallHandler only handles the vmmcall for 1 core.
	It is the guest's responsibility to set thread affinity.

	Parameters are passed in the order of rcx(ID), rdx(Struct)
	*/
	void VMMCallHandler(_Inout_ VMMData_& VMMData)
	{
		VMMCALL_ID VmmCallExitCode = (VMMCALL_ID)VMMData.Context->Rcx;

		switch (VmmCallExitCode)
		{
		case VMMCALL_ID::SetNptHook:
		{
			PSetNptHook SetNptHook = (PSetNptHook)VMMData.Context->Rdx;
			objHyperVisorSvm.objNptHooks.SetNptHook(VMMData.Private, SetNptHook->address, SetNptHook->patch, SetNptHook->patch_len, SetNptHook->ncr3_id);
			break;
		}
		case VMMCALL_ID::RemoveNptHook:
		{
			PRemoveNptHook RemoveNptHook = (PRemoveNptHook)VMMData.Context->Rdx;
			objHyperVisorSvm.objNptHooks.ForEachHook(
				[](auto hook_entry, auto data)-> auto {

					if (hook_entry->address == data)
					{
						DbgPrint("unsetting NPT hook at %p \n", hook_entry->address);
						objHyperVisorSvm.objNptHooks.UnsetHook(hook_entry);
					}

					return false;
				},
				(void*)RemoveNptHook->callback_data
			);
			break;
		}
		case VMMCALL_ID::IsHvPresent:
		{
			break;
		}
		case VMMCALL_ID::SandboxPage:
		{
			break;
		}
		case VMMCALL_ID::DenySandboxReads:
		{
			break;
		}
		case VMMCALL_ID::StartBranchTrace:
		{
			Trace::BranchTracer = (PTracer)VMMData.Context->Rdx;
			Trace::BranchTracer->IsSystem = ((uintptr_t)Trace::BranchTracer->StartAddr < 0x7FFFFFFFFFFF) ? false : true;
			Trace::BranchTracer->ProcessCr3.Value = VMMData.Private->Guest.StateSaveArea.Cr3;
			break;
		}
		case VMMCALL_ID::StartInstructionTrace:
		{
			Trace::InstructionTracer = (PTracer)VMMData.Context->Rdx;
			Trace::InstructionTracer->IsSystem = ((uintptr_t)Trace::InstructionTracer->StartAddr < 0x7FFFFFFFFFFF) ? false : true;
			Trace::InstructionTracer->ProcessCr3.Value = VMMData.Private->Guest.StateSaveArea.Cr3;
			break;
		}
		case VMMCALL_ID::HookEferSyscall:
		{
			break;
		}
		case VMMCALL_ID::UnboxPage:
		{
			break;
		}
		case VMMCALL_ID::StopBranchTrace:
		{
			break;
		}
		case VMMCALL_ID::StopInstructionTrace:
		{
			break;
		}
		default:
		{
			InjectEvent(&VMMData.Private->Guest, INTERRUPT_VECTOR::InvalidOpcode, EXCEPTION_VECTOR::FaultTrapException, 0); break;
			return;
		}
		}

		return;
	}

	extern "C" SVM::VMM_STATUS SvmExitHandler(
		_Inout_ SVM::PRIVATE_VM_DATA * Private,
		_Inout_ GuestContext * Context)
	{
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
		bool EndSvm = false;

		SVM::VMM_STATUS Status = SVM::VMM_STATUS::VMM_CONTINUE;
		switch (VMMData.Private->Guest.ControlArea.ExitCode)
		{
		case SVM::SVM_EXIT_CODE::VMEXIT_EXCP_DB:
		{
			if (Trace::BranchTracer->Initialized)
			{
				Trace::LBROnHandler(VMMData);
			}
			else if (Trace::InstructionTracer->Initialized)
			{
				Trace::TrapOnHandler(VMMData);
			}

			break;
		}
		case SVM::SVM_EXIT_CODE::VMEXIT_EXCP_BP:
		{
			if (Trace::BranchTracer->Initialized && VMMData.Private->Guest.StateSaveArea.Rip == Trace::BranchTracer->StartAddr)
			{
				Trace::StartBranch(VMMData);
			}
			else if (Trace::InstructionTracer->Initialized && VMMData.Private->Guest.StateSaveArea.Rip == Trace::InstructionTracer->StartAddr)
			{
				Trace::StartInstruction(VMMData);
			}
			break;
		}
		case SVM::SVM_EXIT_CODE::VMEXIT_EXCP_GP:
		{
			break;
		}
		case SVM::SVM_EXIT_CODE::VMEXIT_CPUID: { CpuIDHandler(VMMData, Status); break; }
		case SVM::SVM_EXIT_CODE::VMEXIT_MSR: { MsrHandler(VMMData); break; }
		case SVM::SVM_EXIT_CODE::VMEXIT_VMRUN: { InjectEvent(&VMMData.Private->Guest, INTERRUPT_VECTOR::GeneralProtection, EXCEPTION_VECTOR::FaultTrapException, 0); break; }
		case SVM::SVM_EXIT_CODE::VMEXIT_VMMCALL: { VMMCallHandler(VMMData); break; }

		//case SVM::SVM_EXIT_CODE::VMEXIT_NPF: { 
		//	//DbgBreakPoint();
		//	HandleNestedPageFault(&VMMData.Private->Guest, &HookData); 
		//	break;
		//}
		}

		//objSIMDGuestContext = *VMMData.objSIMDGuestContext;
		_fxrstor64(VMMData.objSIMDGuestContext);
		
		//Private = VMMData.Private;
		//Context = VMMData.Context;
				
		//VMCB CLEAN AND TLB CONTROL TRANSFER
		VMMData.Private->Guest.ControlArea.VmcbCleanBits &= 0xFFFFFFEF;
		VMMData.Private->Guest.ControlArea.TlbControl.Bitmap.TlbControl = 1;
		
		__writecr3(VMROOTCR3);
		if (VMMData.Private->Guest.ControlArea.ExitCode == SVM::SVM_EXIT_CODE::VMEXIT_EXCP_BP || !VMMData.Private->Guest.ControlArea.NextRip)
		{
			VMMData.Private->Guest.StateSaveArea.Rax = Context->Rax;
			return Status;
		}

		VMMData.Private->Guest.StateSaveArea.Rax = Context->Rax;

		// Go to the next instruction:
		VMMData.Private->Guest.StateSaveArea.Rip = VMMData.Private->Guest.ControlArea.NextRip;
		return Status;
	}
}