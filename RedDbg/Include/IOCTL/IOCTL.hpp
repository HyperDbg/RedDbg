#pragma once
#include "AppToDrv/MiddleStructs.hpp"
#include "AppToDrv/IoCtlCodes.hpp"
#include <string>
#include <ntddk.h>
#include <ntifs.h>
#include <memory>
#include "Local/Functions.hpp"
#include <minwindef.h>

typedef struct _USERMODE_BITNESS_OF_PROCESS
{
	PEPROCESS SourceProcess;
	bool Is32Bit;
} USERMODE_BITNESS_OF_PROCESS, * PUSERMODE_BITNESS_OF_PROCESS;

class IoCtlDispatcher_
{
private:
	class SystemKernelLevel_
	{
	public:
		bool GetMemoryMap();
	};

	class ProcessUserLevel_
	{
	private:
		bool UserAccessPrintLoadedModulesX64(PEPROCESS Proc,
			bool OnlyCountModules,
			PUINT32 ModulesCount,
			PUSERMODE_LOADED_MODULE_SYMBOLS ModulesList,
			UINT32 SizeOfBufferForModulesList,
			UCHAR TypeOfLoad);

		bool UserAccessPrintLoadedModulesX86(PEPROCESS Proc,
			bool OnlyCountModules,
			PUINT32 ModulesCount,
			PUSERMODE_LOADED_MODULE_SYMBOLS ModulesList,
			UINT32 SizeOfBufferForModulesList,
			UCHAR TypeOfLoad);

		bool UserAccessIsWow64Process(HANDLE ProcessId, bool* Is32Bit);
		bool UserAccessIsWow64ProcessByEprocess(PEPROCESS SourceProcess, bool* Is32Bit);

		void ProcessHeapGetterX64(PUSERMODE_HEAP_DETAILS ProcessHeapRequest, PUSERMODE_BITNESS_OF_PROCESS BitnessOfProcess);
		void ProcessHeapGetterX86(PUSERMODE_HEAP_DETAILS ProcessHeapRequest, PUSERMODE_BITNESS_OF_PROCESS BitnessOfProcess);
	public:
		bool GetMemoryMap(PUSERMODE_LOADED_MODULE_DETAILS ProcessLoadedModuleRequest, UINT32 BufferSize);

		void _GetProcessHeaps(PUSERMODE_HEAP_DETAILS ProcessHeapRequest);
		void _GetProcessHeap(PUSERMODE_HEAP_DETAILS ProcessHeapRequest);

		const std::shared_ptr<USERMODE_BITNESS_OF_PROCESS> GetUserModeProcessBitness(uint32_t ProcessId);
	};
public:
	SystemKernelLevel_ SystemKernelLevel;
	ProcessUserLevel_ ProcessUserLevel;
};