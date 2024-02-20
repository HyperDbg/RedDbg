#pragma once 
#include "Debugger/Driver/GuestContext.hpp"
#include "HyperVisor/HyperVisor.hpp"

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

void InjectEvent(__out SVM::VMCB* Guest, unsigned char Vector, unsigned char Type, unsigned int Code);