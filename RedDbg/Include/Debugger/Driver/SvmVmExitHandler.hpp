#pragma once

#include "Debugger/Driver/GuestContext.hpp"
#include "Debugger/Driver/CommonHandlers.hpp"
#include "AppToDrv/MiddleStructs.hpp"
#include "Debugger/Driver/Common.hpp"
#include "Zydis/Zydis.h"
#include "HyperVisor/HyperVisor.hpp"
#include "HyperVisor/Npt/HookVmmCommon.hpp"
#include "HyperVisor/Npt/HookCommon.hpp"
//Define in asm file
extern "C" void SvmVmmRun(_In_ void* InitialVmmStackPointer);

namespace SvmDbg
{
	void VMMCallHandler(_Inout_ VMMData_& VMMData);
}