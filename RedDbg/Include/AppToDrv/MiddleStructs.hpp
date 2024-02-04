#pragma once
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