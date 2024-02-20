#include "Debugger/Driver/CommonHandlers.hpp"

namespace VMMObjects
{
	Transparent_ Transparent;
}

void CpuIDHandler(_Inout_ VMMData_& VMMData, SVM::VMM_STATUS& Status)
{
	CPUID_REGS Regs = {};
	int Function = static_cast<int>(VMMData.Context->Rax);
	int SubLeaf = static_cast<int>(VMMData.Context->Rcx);
	__cpuidex(Regs.Raw, Function, SubLeaf);

	if (Function == CPUID_VMM_SHUTDOWN) { Status = SVM::VMM_STATUS::VMM_SHUTDOWN; }
	else if (Function == CPUID::Generic::CPUID_EXTENDED_FEATURE_INFORMATION)
	{
		CPUID::FEATURE_INFORMATION RegsFeature = (CPUID::FEATURE_INFORMATION)Regs;
		VMMData.Context = VMMObjects::Transparent.InvisibleCpuid(RegsFeature, VMMData.Context);
	}
	else
	{
		VMMData.Context->Rax = Regs.Regs.Eax;
		VMMData.Context->Rbx = Regs.Regs.Ebx;
		VMMData.Context->Rcx = Regs.Regs.Ecx;
		VMMData.Context->Rdx = Regs.Regs.Edx;
	}

	return;
}

void MsrHandler(_Inout_ VMMData_& VMMData)
{
	if ((VMMData.Context->Rcx & MAXUINT32) == static_cast<unsigned int>(AMD::AMD_MSR::MSR_EFER) && VMMData.Private->Guest.ControlArea.ExitInfo1)
	{
		AMD::EFER Efer = {};
		Efer.Value = ((VMMData.Context->Rdx & MAXUINT32) << 32) | (VMMData.Context->Rax & MAXUINT32);
		if (!Efer.Bitmap.SecureVirtualMachineEnable)
		{
			InjectEvent(&VMMData.Private->Guest, INTERRUPT_VECTOR::GeneralProtection, EXCEPTION_VECTOR::FaultTrapException, 0); // #GP (Vector = 13, Type = Exception)
			return;
		}
		VMMData.Private->Guest.StateSaveArea.Efer = Efer.Value;
	}

	return;
}