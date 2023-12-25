#include "IOCTL/IOCTL.hpp"

NTSTATUS IoCtlDispatcher_::SystemKernelLevel_::GetMemoryMap()
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;



	return Status;
}

bool UserAccessIsWow64ProcessByEprocess(PEPROCESS SourceProcess, bool* Is32Bit)
{
    if (UnexportedFunctions_::PsGetProcessWow64Process(SourceProcess))
    {
        //
        // x86 process, walk x86 module list
        //

        *Is32Bit = TRUE;

        return TRUE;
    }
    else if (UnexportedFunctions_::PsGetProcessPeb(SourceProcess))
    {
        //
        // x64 process, walk x64 module list
        //
        *Is32Bit = FALSE;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool UserAccessIsWow64Process(HANDLE ProcessId, bool* Is32Bit)
{
    PEPROCESS  SourceProcess;
    KAPC_STATE State = { 0 };

    if (PsLookupProcessByProcessId(ProcessId, &SourceProcess) != STATUS_SUCCESS)
    {
        //
        // if the process not found
        //
        return FALSE;
    }

    ObDereferenceObject(SourceProcess);

    return UserAccessIsWow64ProcessByEprocess(SourceProcess, Is32Bit);
}

NTSTATUS IoCtlDispatcher_::ProcessUserLevel_::GetMemoryMap(PUSERMODE_LOADED_MODULE_DETAILS ProcessLoadedModuleRequest, UINT32 BufferSize)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	
	PEPROCESS SourceProcess;
    bool   Is32Bit;

	if (PsLookupProcessByProcessId((HANDLE)ProcessLoadedModuleRequest->ProcessId, &SourceProcess) != STATUS_SUCCESS)
	{
		//
		// if the process not found
		//
		return Status;
	}

	ObDereferenceObject(SourceProcess);
    //
    // check whether the target process is 32-bit or 64-bit
    //
    if (!UserAccessIsWow64Process((HANDLE)ProcessLoadedModuleRequest->ProcessId, &Is32Bit))
    {
        //
        // Unable to detect whether it's 32-bit or 64-bit
        //
        return Status;
    }

    //
    // Indicate that the process is x86
    //
    ProcessLoadedModuleRequest->Is32Bit = Is32Bit;
    /*
    if (Is32Bit)
    {
        //
        // x86 process, walk x86 module list
        //
        if (UserAccessPrintLoadedModulesX86(SourceProcess,
            ProcessLoadedModuleRequest->OnlyCountModules,
            &ProcessLoadedModuleRequest->ModulesCount,
            (UINT64)ProcessLoadedModuleRequest + sizeof(USERMODE_LOADED_MODULE_DETAILS),
            BufferSize - sizeof(USERMODE_LOADED_MODULE_DETAILS)))
        {
            ProcessLoadedModuleRequest->Result = DEBUGGER_OPERATION_WAS_SUCCESSFUL;
            return Status;
        }
    }
    else
    {
        //
        // x64 process, walk x64 module list
        //
        if (UserAccessPrintLoadedModulesX64(SourceProcess,
            ProcessLoadedModuleRequest->OnlyCountModules,
            &ProcessLoadedModuleRequest->ModulesCount,
            (UINT64)ProcessLoadedModuleRequest + sizeof(USERMODE_LOADED_MODULE_DETAILS),
            BufferSize - sizeof(USERMODE_LOADED_MODULE_DETAILS)))
        {
            ProcessLoadedModuleRequest->Result = DEBUGGER_OPERATION_WAS_SUCCESSFUL;
            return TRUE;
        }
    }

    ProcessLoadedModuleRequest->Result = DEBUGGER_ERROR_UNABLE_TO_GET_MODULES_OF_THE_PROCESS;
    return FALSE;
    */
	return Status;
}