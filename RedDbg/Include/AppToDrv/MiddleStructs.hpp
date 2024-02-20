#pragma once
#include "HyperVisor/CommonTypes/Registers.hpp"
#include <memory>
#include <vector>
//#include <minwindef.h>

#define MAX_PTH 256
typedef void* PVOID;

enum ModuleLoadType
{
	LoadOrder,
	MemoryOrder,
	InitOrder
};

typedef struct _USERMODE_LOADED_MODULE_DETAILS
{
	unsigned int  ProcessId;
	bool OnlyCountModules;
	bool Is32Bit;
	unsigned int  ModulesCount;
	unsigned int  Result;
	unsigned char TypeOfLoad;
	//
	// Here is a list of USERMODE_LOADED_MODULE_SYMBOLS (appended)
	//

} USERMODE_LOADED_MODULE_DETAILS, * PUSERMODE_LOADED_MODULE_DETAILS;

typedef struct _USERMODE_HEAP_DETAILS
{
	unsigned int  ProcessId;
	unsigned int  TotalHeaps;
	unsigned long NumberOfHeapsToReturn;
	PVOID* ProcessHeap;
	void* HeapHandle;//HANDLE
	unsigned int  Result;
} USERMODE_HEAP_DETAILS, * PUSERMODE_HEAP_DETAILS;

typedef struct _USERMODE_LOADED_MODULE_SYMBOLS
{
	unsigned long long  BaseAddress;
	unsigned long long  Entrypoint;
	unsigned short FilePath[MAX_PTH];//wchar_t

} USERMODE_LOADED_MODULE_SYMBOLS, * PUSERMODE_LOADED_MODULE_SYMBOLS;

typedef struct _USERMODE_READ_PROCESS_MEMORY
{
	unsigned int Pid;
	PVOID Dest;
	PVOID Src;
	size_t Size;
} USERMODE_READ_PROCESS_MEMORY, * PUSERMODE_READ_PROCESS_MEMORY;

enum class VMMCALL_ID
{
	SetNptHook,
	RemoveNptHook,
	IsHvPresent,
	SandboxPage,
	DenySandboxReads,
	StartBranchTrace,
	StartInstructionTrace,
	HookEferSyscall,
	UnboxPage,
	StopBranchTrace,
	StopInstructionTrace
};

typedef struct SetNptHook_
{
	void* address = nullptr;
	unsigned char* patch = nullptr;
	unsigned long long patch_len = 0;
	int ncr3_id = 0;
} SetNptHook, * PSetNptHook;

typedef struct RemoveNptHook_
{
	void* callback_data = nullptr;
} RemoveNptHook, * PRemoveNptHook;

typedef struct Tracer_
{
	bool Initialized = false;
	bool IsSystem = false;
	CR3 ProcessCr3{};
	uintptr_t StartAddr = 0;
	uintptr_t StopAddr = 0;
	uintptr_t TraceRangeBase = 0;
	uintptr_t TraceRangeSize = 0;
	size_t Count = 0;
} Tracer, *PTracer;