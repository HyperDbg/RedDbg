#include "IOCTL/IOCTL.hpp"
#include "Debugger/ErrorCodes.hpp"

bool IoCtlDispatcher_::SystemKernelLevel_::GetMemoryMap()
{



	return true;
}

bool IoCtlDispatcher_::ProcessUserLevel_::UserAccessIsWow64ProcessByEprocess(
    PEPROCESS SourceProcess, 
    bool* Is32Bit)
{
    if (UnexportedFunctions_::PsGetProcessWow64Process(SourceProcess))
    {
        // x86 process, walk x86 module list
        *Is32Bit = true;
        return true;
    }
    else if (UnexportedFunctions_::PsGetProcessPeb(SourceProcess))
    {
        // x64 process, walk x64 module list
        *Is32Bit = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool IoCtlDispatcher_::ProcessUserLevel_::UserAccessIsWow64Process(
    HANDLE ProcessId, 
    bool* Is32Bit)
{
    PEPROCESS  SourceProcess;
    KAPC_STATE State = { 0 };

    if (PsLookupProcessByProcessId(ProcessId, &SourceProcess) != STATUS_SUCCESS)
    {
        // if the process not found
        return false;
    }

    ObDereferenceObject(SourceProcess);

    return UserAccessIsWow64ProcessByEprocess(SourceProcess, Is32Bit);
}

bool IoCtlDispatcher_::ProcessUserLevel_::UserAccessPrintLoadedModulesX64(PEPROCESS Proc,
    bool OnlyCountModules,
    PUINT32 ModulesCount,
    PUSERMODE_LOADED_MODULE_SYMBOLS ModulesList,
    UINT32 SizeOfBufferForModulesList,
    UCHAR TypeOfLoad)
{
    KAPC_STATE State;
    //UNICODE_STRING Name;
    uint32_t CountOfModules = 0;
    PPEB Peb = nullptr;
    PPEB_LDR_DATA Ldr = nullptr;
    uint32_t CurrentSavedModules = 0;
    uint32_t TempSize = 0;

    Peb = UnexportedFunctions_::PsGetProcessPeb(Proc);

    if (!Peb) { return false; }

    KeStackAttachProcess((PRKPROCESS)Proc, &State);

    Ldr = (PPEB_LDR_DATA)Peb->Ldr;

    //DbgBreakPoint();
    if (!Ldr)
    {
        DbgPrint("Ldr is Null");
        DbgBreakPoint();
        KeUnstackDetachProcess(&State);
        return false;
    }

    //DbgBreakPoint();
    if (OnlyCountModules)
    {
        DbgBreakPoint();
        while (Peb->ProcessHeap)
        {
            PVOID HeapBase = Peb->ProcessHeap;
            DbgPrint("Heap %p", HeapBase);
            
            Peb->ProcessHeap = (PVOID)((uint64_t)Peb->ProcessHeap + 8);
        }
        DbgBreakPoint();
        //
        // loop the linked list (Computer the size)
        //
        if (TypeOfLoad == ModuleLoadType::LoadOrder)
        {
            for (PLIST_ENTRY List = (PLIST_ENTRY)Ldr->InLoadOrderModuleList.Flink;
                List != &Ldr->InLoadOrderModuleList;
                List = (PLIST_ENTRY)List->Flink)
            {
                //PLDR_DATA_TABLE_ENTRY Entry =
                //    CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

                //
                // Calculate count of modules
                //
                CountOfModules++;
            }
        }
        else if (TypeOfLoad == ModuleLoadType::MemoryOrder)
        {
            for (PLIST_ENTRY List = (PLIST_ENTRY)Ldr->InMemoryOrderModuleList.Flink;
                List != &Ldr->InMemoryOrderModuleList;
                List = (PLIST_ENTRY)List->Flink)
            {
                //PLDR_DATA_TABLE_ENTRY Entry =
                //    CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

                //
                // Calculate count of modules
                //
                CountOfModules++;
            }
        }
        else if (TypeOfLoad == ModuleLoadType::InitOrder)
        {
            for (PLIST_ENTRY List = (PLIST_ENTRY)Ldr->InInitializationOrderModuleList.Flink;
                List != &Ldr->InInitializationOrderModuleList;
                List = (PLIST_ENTRY)List->Flink)
            {
                //PLDR_DATA_TABLE_ENTRY Entry =
                //    CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY, InInitializationOrderLinks);

                //
                // Calculate count of modules
                //
                CountOfModules++;
            }
        }
        *ModulesCount = CountOfModules;

        KeUnstackDetachProcess(&State);
        return true;
    }
    else
    {
        //
        // It's not counting the modules, so we compute the number of modules
        // that can be stored in the buffer by using the size of the buffer
        //
        CountOfModules = SizeOfBufferForModulesList / sizeof(USERMODE_LOADED_MODULE_SYMBOLS);
    }

    //
    // Walk again to save the buffer
    //
    Ldr = (PPEB_LDR_DATA)Peb->Ldr;

    if (!Ldr)
    {
        KeUnstackDetachProcess(&State);
        return true;
    }

    //
    // loop the linked list
    //
    if (TypeOfLoad == ModuleLoadType::LoadOrder)
    {
        for (PLIST_ENTRY List = (PLIST_ENTRY)Ldr->InLoadOrderModuleList.Flink;
            List != &Ldr->InLoadOrderModuleList;
            List = (PLIST_ENTRY)List->Flink)
        {
            PLDR_DATA_TABLE_ENTRY Entry =
                CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

            if (CountOfModules == CurrentSavedModules)
            {
                //
                // Won't continue as the buffer is now full
                // Generally, we shouldn't be at this stage, only when
                // a module is just loaded and we didn't allocate enough
                // memory for it, so it's better to continue
                //
                KeUnstackDetachProcess(&State);
                return true;
            }

            //
            // Save the details into the storage
            //
            ModulesList[CurrentSavedModules].Entrypoint = (UINT64)Entry->EntryPoint;
            ModulesList[CurrentSavedModules].BaseAddress = (UINT64)Entry->DllBase;

            //
            // Copy the path
            //
            TempSize = Entry->FullDllName.Length;
            if (TempSize >= MAX_PATH)
            {
                TempSize = MAX_PATH;
            }

            TempSize = TempSize * 2;
            memcpy(&ModulesList[CurrentSavedModules].FilePath, Entry->FullDllName.Buffer, TempSize);

            CurrentSavedModules++;
        }
    }
    else if (TypeOfLoad == ModuleLoadType::MemoryOrder)
    {
        for (PLIST_ENTRY List = (PLIST_ENTRY)Ldr->InMemoryOrderModuleList.Flink;
            List != &Ldr->InMemoryOrderModuleList;
            List = (PLIST_ENTRY)List->Flink)
        {
            PLDR_DATA_TABLE_ENTRY Entry =
                CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

            if (CountOfModules == CurrentSavedModules)
            {
                //
                // Won't continue as the buffer is now full
                // Generally, we shouldn't be at this stage, only when
                // a module is just loaded and we didn't allocate enough
                // memory for it, so it's better to continue
                //
                KeUnstackDetachProcess(&State);
                return true;
            }

            //
            // Save the details into the storage
            //
            ModulesList[CurrentSavedModules].Entrypoint = (UINT64)Entry->EntryPoint;
            ModulesList[CurrentSavedModules].BaseAddress = (UINT64)Entry->DllBase;

            //
            // Copy the path
            //
            TempSize = Entry->FullDllName.Length;
            if (TempSize >= MAX_PATH)
            {
                TempSize = MAX_PATH;
            }

            TempSize = TempSize * 2;
            memcpy(&ModulesList[CurrentSavedModules].FilePath, Entry->FullDllName.Buffer, TempSize);

            CurrentSavedModules++;
        }
    }
    else if (TypeOfLoad == ModuleLoadType::InitOrder)
    {
        for (PLIST_ENTRY List = (PLIST_ENTRY)Ldr->InInitializationOrderModuleList.Flink;
            List != &Ldr->InInitializationOrderModuleList;
            List = (PLIST_ENTRY)List->Flink)
        {
            PLDR_DATA_TABLE_ENTRY Entry =
                CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY, InInitializationOrderLinks);

            if (CountOfModules == CurrentSavedModules)
            {
                //
                // Won't continue as the buffer is now full
                // Generally, we shouldn't be at this stage, only when
                // a module is just loaded and we didn't allocate enough
                // memory for it, so it's better to continue
                //
                KeUnstackDetachProcess(&State);
                return true;
            }

            //
            // Save the details into the storage
            //
            ModulesList[CurrentSavedModules].Entrypoint = (UINT64)Entry->EntryPoint;
            ModulesList[CurrentSavedModules].BaseAddress = (UINT64)Entry->DllBase;

            //
            // Copy the path
            //
            TempSize = Entry->FullDllName.Length;
            if (TempSize >= MAX_PATH)
            {
                TempSize = MAX_PATH;
            }

            TempSize = TempSize * 2;
            memcpy(&ModulesList[CurrentSavedModules].FilePath, Entry->FullDllName.Buffer, TempSize);

            CurrentSavedModules++;
        }
    }

    
    KeUnstackDetachProcess(&State);

    return true;
}

bool IoCtlDispatcher_::ProcessUserLevel_::UserAccessPrintLoadedModulesX86(PEPROCESS Proc,
    bool OnlyCountModules,
    PUINT32 ModulesCount,
    PUSERMODE_LOADED_MODULE_SYMBOLS ModulesList,
    UINT32 SizeOfBufferForModulesList,
    UCHAR TypeOfLoad)
{
    KAPC_STATE State;
    //UNICODE_STRING Name;
    PPEB32 Peb = nullptr;
    UINT32 CountOfModules = 0;
    UINT32 CurrentSavedModules = 0;
    UINT32 TempSize = 0;

    Peb = (PPEB32)UnexportedFunctions_::PsGetProcessPeb(Proc);

    if (!Peb)
    {
        return false;
    }

    KeStackAttachProcess((PRKPROCESS)Proc, &State);

    PPEB_LDR_DATA32 Ldr = (PPEB_LDR_DATA32)Peb->Ldr;

    if (!Ldr)
    {
        KeUnstackDetachProcess(&State);
        return false;
    }

    if (OnlyCountModules)
    {
        //
        // loop the linked list (Computer the size)
        //
        if (TypeOfLoad == ModuleLoadType::LoadOrder)
        {
            for (PLIST_ENTRY32 List = (PLIST_ENTRY32)Ldr->InLoadOrderModuleList.Flink;
                List != &Ldr->InLoadOrderModuleList;
                List = (PLIST_ENTRY32)List->Flink)
            {
                //PLDR_DATA_TABLE_ENTRY32 Entry =
                //    CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

                //
                // Calculate count of modules
                //
                CountOfModules++;
            }
        }
        else if (TypeOfLoad == ModuleLoadType::MemoryOrder)
        {
            for (PLIST_ENTRY32 List = (PLIST_ENTRY32)Ldr->InMemoryOrderModuleList.Flink;
                List != &Ldr->InMemoryOrderModuleList;
                List = (PLIST_ENTRY32)List->Flink)
            {
                //PLDR_DATA_TABLE_ENTRY32 Entry =
                //    CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY32, InMemoryOrderLinks);

                //
                // Calculate count of modules
                //
                CountOfModules++;
            }
        }
        else if (TypeOfLoad == ModuleLoadType::InitOrder)
        {
            for (PLIST_ENTRY32 List = (PLIST_ENTRY32)Ldr->InInitializationOrderModuleList.Flink;
                List != &Ldr->InInitializationOrderModuleList;
                List = (PLIST_ENTRY32)List->Flink)
            {
                //PLDR_DATA_TABLE_ENTRY32 Entry =
                //    CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY32, InInitializationOrderLinks);

                //
                // Calculate count of modules
                //
                CountOfModules++;
            }
        }

        *ModulesCount = CountOfModules;

        KeUnstackDetachProcess(&State);
        return true;
    }
    else
    {
        //
        // It's not counting the modules, so we compute the number of modules
        // that can be stored in the buffer by using the size of the buffer
        //
        CountOfModules = SizeOfBufferForModulesList / sizeof(USERMODE_LOADED_MODULE_SYMBOLS);
    }

    //
    // Walk again to save the buffer
    //
    Ldr = (PPEB_LDR_DATA32)Peb->Ldr;

    if (!Ldr)
    {
        KeUnstackDetachProcess(&State);
        return false;
    }

    //
    // loop the linked list
    //
    if (TypeOfLoad == ModuleLoadType::LoadOrder)
    {
        for (PLIST_ENTRY32 List = (PLIST_ENTRY32)Ldr->InLoadOrderModuleList.Flink;
            List != &Ldr->InLoadOrderModuleList;
            List = (PLIST_ENTRY32)List->Flink)
        {
            PLDR_DATA_TABLE_ENTRY32 Entry =
                CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

            if (CountOfModules == CurrentSavedModules)
            {
                //
                // Won't continue as the buffer is now full
                // Generally, we shouldn't be at this stage, only when
                // a module is just loaded and we didn't allocate enough
                // memory for it, so it's better to continue
                //
                KeUnstackDetachProcess(&State);
                return true;
            }

            //
            // Save the details into the storage
            //
            ModulesList[CurrentSavedModules].Entrypoint = Entry->EntryPoint;
            ModulesList[CurrentSavedModules].BaseAddress = Entry->DllBase;

            //
            // Copy the path
            //
            TempSize = Entry->FullDllName.Length;
            if (TempSize >= MAX_PATH)
            {
                TempSize = MAX_PATH;
            }

            TempSize = TempSize * 2;
            memcpy(&ModulesList[CurrentSavedModules].FilePath, (const void*)Entry->FullDllName.Buffer, TempSize);

            CurrentSavedModules++;
        }
    }
    else if (TypeOfLoad == ModuleLoadType::MemoryOrder)
    {
        for (PLIST_ENTRY32 List = (PLIST_ENTRY32)Ldr->InMemoryOrderModuleList.Flink;
            List != &Ldr->InMemoryOrderModuleList;
            List = (PLIST_ENTRY32)List->Flink)
        {
            PLDR_DATA_TABLE_ENTRY32 Entry =
                CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY32, InMemoryOrderLinks);

            if (CountOfModules == CurrentSavedModules)
            {
                //
                // Won't continue as the buffer is now full
                // Generally, we shouldn't be at this stage, only when
                // a module is just loaded and we didn't allocate enough
                // memory for it, so it's better to continue
                //
                KeUnstackDetachProcess(&State);
                return true;
            }

            //
            // Save the details into the storage
            //
            ModulesList[CurrentSavedModules].Entrypoint = Entry->EntryPoint;
            ModulesList[CurrentSavedModules].BaseAddress = Entry->DllBase;

            //
            // Copy the path
            //
            TempSize = Entry->FullDllName.Length;
            if (TempSize >= MAX_PATH)
            {
                TempSize = MAX_PATH;
            }

            TempSize = TempSize * 2;
            memcpy(&ModulesList[CurrentSavedModules].FilePath, (const void*)Entry->FullDllName.Buffer, TempSize);

            CurrentSavedModules++;
        }
    }
    else if (TypeOfLoad == ModuleLoadType::InitOrder)
    {
        for (PLIST_ENTRY32 List = (PLIST_ENTRY32)Ldr->InInitializationOrderModuleList.Flink;
            List != &Ldr->InInitializationOrderModuleList;
            List = (PLIST_ENTRY32)List->Flink)
        {
            PLDR_DATA_TABLE_ENTRY32 Entry =
                CONTAINING_RECORD(List, LDR_DATA_TABLE_ENTRY32, InInitializationOrderLinks);

            if (CountOfModules == CurrentSavedModules)
            {
                //
                // Won't continue as the buffer is now full
                // Generally, we shouldn't be at this stage, only when
                // a module is just loaded and we didn't allocate enough
                // memory for it, so it's better to continue
                //
                KeUnstackDetachProcess(&State);
                return true;
            }

            //
            // Save the details into the storage
            //
            ModulesList[CurrentSavedModules].Entrypoint = Entry->EntryPoint;
            ModulesList[CurrentSavedModules].BaseAddress = Entry->DllBase;

            //
            // Copy the path
            //
            TempSize = Entry->FullDllName.Length;
            if (TempSize >= MAX_PATH)
            {
                TempSize = MAX_PATH;
            }

            TempSize = TempSize * 2;
            memcpy(&ModulesList[CurrentSavedModules].FilePath, (const void*)Entry->FullDllName.Buffer, TempSize);

            CurrentSavedModules++;
        }
    }

    KeUnstackDetachProcess(&State);

    return true;
}

void IoCtlDispatcher_::ProcessUserLevel_::ProcessHeapGetterX64(
    PUSERMODE_HEAP_DETAILS ProcessHeapRequest,
    PUSERMODE_BITNESS_OF_PROCESS BitnessOfProcess)
{
    KAPC_STATE State;
    PPEB Peb = nullptr;
    ULONG NumberOfHeapsToCopy;

    Peb = UnexportedFunctions_::PsGetProcessPeb(BitnessOfProcess->SourceProcess);
    KeStackAttachProcess((PRKPROCESS)BitnessOfProcess->SourceProcess, &State);
    ProcessHeapRequest->TotalHeaps = Peb->NumberOfHeaps;

    std::vector<uint64_t> TempHeap;
    if (ProcessHeapRequest->TotalHeaps > ProcessHeapRequest->NumberOfHeapsToReturn) {
        NumberOfHeapsToCopy = ProcessHeapRequest->NumberOfHeapsToReturn;
    }
    else {
        NumberOfHeapsToCopy = ProcessHeapRequest->TotalHeaps;

        for (int Index = 0; Index < NumberOfHeapsToCopy; ++Index)
        {
            TempHeap.push_back((uint64_t)Peb->ProcessHeaps[Index]);
        }
    }

    ProcessHeapRequest->NumberOfHeapsToReturn -= NumberOfHeapsToCopy;
    KeUnstackDetachProcess(&State);
    if (TempHeap.size() > 0)
    {
        size_t Length = NumberOfHeapsToCopy * sizeof(*ProcessHeapRequest->ProcessHeap);
        RtlCopyMemory(ProcessHeapRequest->ProcessHeap,
            TempHeap.data(),
            Length);
    }

    return;
}

//void IoCtlDispatcher_::ProcessUserLevel_::ProcessHeapGetterX86(
//    PUSERMODE_HEAP_DETAILS ProcessHeapRequest,
//    PUSERMODE_BITNESS_OF_PROCESS BitnessOfProcess)
//{
//    PPEB32 Peb = nullptr;
//
//    Peb = (PPEB32)UnexportedFunctions_::PsGetProcessPeb(BitnessOfProcess->SourceProcess);
//
//    return;
//}

const std::shared_ptr<USERMODE_BITNESS_OF_PROCESS> IoCtlDispatcher_::ProcessUserLevel_::GetUserModeProcessBitness(
    uint32_t ProcessId)
{
    auto Bitness = std::make_shared<USERMODE_BITNESS_OF_PROCESS>();

    if (PsLookupProcessByProcessId((HANDLE)ProcessId, &Bitness->SourceProcess) != STATUS_SUCCESS)
    {
        // if the process not found
        return Bitness;
    }

    ObDereferenceObject(Bitness->SourceProcess);

    // check whether the target process is 32-bit or 64-bit
    if (!UserAccessIsWow64Process((HANDLE)ProcessId, &Bitness->Is32Bit))
    {
        // Unable to detect whether it's 32-bit or 64-bit
        return Bitness;
    }

    return Bitness;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IoCtlDispatcher_::ProcessUserLevel_::_GetProcessHeap(
    PUSERMODE_HEAP_DETAILS ProcessHeapRequest)
{
    auto BitnessOfProcess = GetUserModeProcessBitness(ProcessHeapRequest->ProcessId);

    KAPC_STATE State;
    PPEB Peb = nullptr;
    PPEB32 Peb32 = nullptr;
    KeStackAttachProcess((PRKPROCESS)BitnessOfProcess->SourceProcess, &State);
    Peb = UnexportedFunctions_::PsGetProcessPeb(BitnessOfProcess->SourceProcess);
    ProcessHeapRequest->HeapHandle = Peb->ProcessHeap;
    //if (BitnessOfProcess->Is32Bit)
    //{
    //    Peb32 = (PPEB32)UnexportedFunctions_::PsGetProcessPeb(BitnessOfProcess->SourceProcess);
    //    ProcessHeapRequest->HeapHandle = (PVOID)Peb32->ProcessHeap;
    //}
    //else
    //{
    //    Peb = UnexportedFunctions_::PsGetProcessPeb(BitnessOfProcess->SourceProcess);
    //    ProcessHeapRequest->HeapHandle = Peb->ProcessHeap;
    //}
    KeUnstackDetachProcess(&State);

    ProcessHeapRequest->Result = SuccessCodes::DEBUGGER_OPERATION_WAS_SUCCESSFUL;
    return;
}

void IoCtlDispatcher_::ProcessUserLevel_::_GetProcessHeaps(
    PUSERMODE_HEAP_DETAILS ProcessHeapRequest)
    /*++

    Routine Description:

        This routine determines how many individual heaps there are in the
        individual process and fills an array with pointers to each heap.

    Arguments:

        NumberOfHeapsToReturn - Indicates how many heaps the caller
            is willing to accept in the second parameter

        ProcessHeaps - Supplies a pointer to an array of heap pointer
            to be filled in by this routine.  The maximum size of this
            array is specified by the first parameter

    Return Value:

        ULONG - Returns the smaller of the actual number of heaps in the
            the process or the size of the output buffer

    --*/
{
    auto BitnessOfProcess = GetUserModeProcessBitness(ProcessHeapRequest->ProcessId);

    ProcessHeapGetterX64(ProcessHeapRequest, BitnessOfProcess.get());
    ProcessHeapRequest->Result = SuccessCodes::DEBUGGER_OPERATION_WAS_SUCCESSFUL;
    return;

    //if (BitnessOfProcess->Is32Bit)
    //{
    //    ProcessHeapGetterX86(ProcessHeapRequest, BitnessOfProcess.get());
    //    ProcessHeapRequest->Result = SuccessCodes::DEBUGGER_OPERATION_WAS_SUCCESSFUL;
    //    return;
    //}
    //else
    //{
    //    ProcessHeapGetterX64(ProcessHeapRequest, BitnessOfProcess.get());
    //    ProcessHeapRequest->Result = SuccessCodes::DEBUGGER_OPERATION_WAS_SUCCESSFUL;
    //    return;
    //}

    ProcessHeapRequest->Result = ErrorCodes::DEBUGGER_ERROR_UNABLE_TO_GET_MODULES_OF_THE_PROCESS;
    return;
}

bool IoCtlDispatcher_::ProcessUserLevel_::GetMemoryMap(
    PUSERMODE_LOADED_MODULE_DETAILS ProcessLoadedModuleRequest,
    UINT32 BufferSize)
{
    auto BitnessOfProcess = GetUserModeProcessBitness(ProcessLoadedModuleRequest->ProcessId);

    ProcessLoadedModuleRequest->Is32Bit = BitnessOfProcess->Is32Bit;

    if (BitnessOfProcess->Is32Bit)
    {
        // x86 process, walk x86 module list
        if (UserAccessPrintLoadedModulesX86(BitnessOfProcess->SourceProcess,
            ProcessLoadedModuleRequest->OnlyCountModules,
            &ProcessLoadedModuleRequest->ModulesCount,
            PUSERMODE_LOADED_MODULE_SYMBOLS((uint64_t)ProcessLoadedModuleRequest + sizeof(USERMODE_LOADED_MODULE_DETAILS)),
            BufferSize - sizeof(USERMODE_LOADED_MODULE_DETAILS),
            ProcessLoadedModuleRequest->TypeOfLoad))
        {
            ProcessLoadedModuleRequest->Result = DEBUGGER_OPERATION_WAS_SUCCESSFUL;
            return true;
        }
    }
    else
    {
        // x64 process, walk x64 module list
        if (UserAccessPrintLoadedModulesX64(BitnessOfProcess->SourceProcess,
            ProcessLoadedModuleRequest->OnlyCountModules,
            &ProcessLoadedModuleRequest->ModulesCount,
            PUSERMODE_LOADED_MODULE_SYMBOLS((uint64_t)ProcessLoadedModuleRequest + sizeof(USERMODE_LOADED_MODULE_DETAILS)),
            BufferSize - sizeof(USERMODE_LOADED_MODULE_DETAILS),
            ProcessLoadedModuleRequest->TypeOfLoad))
        {
            ProcessLoadedModuleRequest->Result = SuccessCodes::DEBUGGER_OPERATION_WAS_SUCCESSFUL;
            return true;
        }
    }

    ProcessLoadedModuleRequest->Result = ErrorCodes::DEBUGGER_ERROR_UNABLE_TO_GET_MODULES_OF_THE_PROCESS;

    return false;
}