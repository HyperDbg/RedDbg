#pragma once
//#define PHNT_VERSION PHNT_WIN11_23H2
//#define PHNT_MODE PHNT_MODE_KERNEL
//#define WINDOWS_IGNORE_PACKING_MISMATCH
//#include <kph.h>
#include <string>
#include <ntddk.h>
#include <ntifs.h>
#include "Local/Functions.hpp"
//#include <ntexapi.h>
//#include <ntmmapi.h>
//#include <ntldr.h>
//#include "Local/winternl.h"
//#include "ntfill.h"

typedef struct _USERMODE_LOADED_MODULE_DETAILS
{
	UINT32  ProcessId;
	bool OnlyCountModules;
	bool Is32Bit;
	UINT32  ModulesCount;
	UINT32  Result;

	//
	// Here is a list of USERMODE_LOADED_MODULE_SYMBOLS (appended)
	//

} USERMODE_LOADED_MODULE_DETAILS, * PUSERMODE_LOADED_MODULE_DETAILS;

namespace IoCtlCode
{
	namespace SystemKernelLevel
	{
		enum IoCtlCodes
		{
			GetMemoryMap,
		};
	}
	namespace ProcessUserLevel
	{
		enum IoCtlCodes
		{
			GetMemoryMap,
		};
	}
}

class IoCtlDispatcher_
{
private:
	class SystemKernelLevel_
	{
	public:
		NTSTATUS GetMemoryMap();
	};

	class ProcessUserLevel_
	{
	public:
		NTSTATUS GetMemoryMap(PUSERMODE_LOADED_MODULE_DETAILS ProcessLoadedModuleRequest, UINT32 BufferSize);
	};
public:
	SystemKernelLevel_ SystemKernelLevel;
	ProcessUserLevel_ ProcessUserLevel;
};