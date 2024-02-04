#pragma once
#include <ntddk.h>
#include <vector>
#include <string>
#include "ntdll.h"

///**
// * @brief LDR Data Table 64-bit
// *
// */
//typedef struct _LDR_DATA_TABLE_ENTRY
//{
//    LIST_ENTRY InLoadOrderModuleList;
//    LIST_ENTRY InMemoryOrderLinks;
//    LIST_ENTRY InInitializationOrderLinks;
//    PVOID DllBase;
//    PVOID EntryPoint;
//    ULONG SizeOfImage;
//    UNICODE_STRING FullDllName;
//    UNICODE_STRING BaseDllName;
//    ULONG Flags;
//    USHORT LoadCount;
//    USHORT TlsIndex;
//    LIST_ENTRY HashLinks;
//    ULONG TimeDateStamp;
//} LDR_DATA_TABLE_ENTRY, * PLDR_DATA_TABLE_ENTRY;

///**
// * @brief LDR Data 64-bit
// *
// */
//typedef struct _PEB_LDR_DATA
//{
//    ULONG Length;
//    BOOLEAN Initialized;
//    HANDLE SsHandle;
//    LIST_ENTRY InLoadOrderModuleList;
//    LIST_ENTRY InMemoryOrderModuleList;
//    LIST_ENTRY InInitializationOrderModuleList;
//    PVOID EntryInProgress;
//    BOOLEAN ShutdownInProgress;
//    HANDLE ShutdownThreadId;
//} PEB_LDR_DATA, * PPEB_LDR_DATA;

/**
 * @brief PEB 64-bit
 *
 */
//typedef struct _PEB
//{
//    UCHAR InheritedAddressSpace;
//    UCHAR ReadImageFileExecOptions;
//    UCHAR BeingDebugged;
//    UCHAR BitField;
//    PVOID Mutant;
//    PVOID ImageBaseAddress;
//    PPEB_LDR_DATA Ldr;
//    PVOID ProcessParameters;
//    PVOID SubSystemData;
//    PVOID ProcessHeap;
//    PVOID FastPebLock;
//    PVOID AtlThunkSListPtr;
//    PVOID IFEOKey;
//    PVOID CrossProcessFlags;
//    PVOID UserSharedInfoPtr;
//    ULONG SystemReserved;
//    ULONG AtlThunkSListPtr32;
//    PVOID ApiSetMap;
//    ULONG TlsExpansionCounter;
//    PVOID TlsBitmap;
//    ULONG TlsBitmapBits1;
//    ULONG TlsBitmapBits2;
//    PVOID ReadOnlySharedMemoryBase;
//    PVOID HotpatchInformation;
//    PVOID* ReadOnlyStaticServerData;
//    PVOID AnsiCodePageData;
//    PVOID OemCodePageData;
//    PVOID UnicodeCaseTableData;
//    ULONG NumberOfProcessors;
//    ULONG NtGlobalFlag;
//} PEB, * PPEB;

/**
 * @brief PEB 32-bit
 *
 */
typedef struct _PEB32
{
    UCHAR InheritedAddressSpace;
    UCHAR ReadImageFileExecOptions;
    UCHAR BeingDebugged;
    UCHAR BitField;
    ULONG Mutant;
    ULONG ImageBaseAddress;
    ULONG Ldr;
    ULONG ProcessParameters;
    ULONG SubSystemData;
    ULONG ProcessHeap;
    ULONG FastPebLock;
    ULONG AtlThunkSListPtr;
    ULONG IFEOKey;
    ULONG CrossProcessFlags;
    ULONG UserSharedInfoPtr;
    ULONG SystemReserved;
    ULONG AtlThunkSListPtr32;
    ULONG ApiSetMap;
} PEB32, * PPEB32;

/**
 * @brief LDR Data 32-bit
 *
 */
typedef struct _PEB_LDR_DATA32
{
    ULONG        Length;
    UCHAR        Initialized;
    ULONG        SsHandle;
    LIST_ENTRY32 InLoadOrderModuleList;
    LIST_ENTRY32 InMemoryOrderModuleList;
    LIST_ENTRY32 InInitializationOrderModuleList;
} PEB_LDR_DATA32, * PPEB_LDR_DATA32;

/**
 * @brief LDR Data Table 32-bit
 *
 */
typedef struct _LDR_DATA_TABLE_ENTRY32
{
    LIST_ENTRY32     InLoadOrderLinks;
    LIST_ENTRY32     InMemoryOrderLinks;
    LIST_ENTRY32     InInitializationOrderLinks;
    ULONG            DllBase;
    ULONG            EntryPoint;
    ULONG            SizeOfImage;
    UNICODE_STRING32 FullDllName;
    UNICODE_STRING32 BaseDllName;
    ULONG            Flags;
    UINT16           LoadCount;
    UINT16           TlsIndex;
    LIST_ENTRY32     HashLinks;
    ULONG            TimeDateStamp;
} LDR_DATA_TABLE_ENTRY32, * PLDR_DATA_TABLE_ENTRY32;

#if _WIN64

typedef NTKERNELAPI PVOID (*PsGetProcessWow64Process_) (_In_ PEPROCESS Process);

#endif

typedef NTKERNELAPI PPEB (*PsGetProcessPeb_)(_In_ PEPROCESS Process);

enum FunctionIndexes
{
    PsGetProcessPebIndex,
    PsGetProcessWow64ProcessIndex,
};

class UnexportedFunctions_
{
private:
    std::vector<std::wstring> Names{ L"PsGetProcessPeb", L"PsGetProcessWow64Process" };
public:
    static PsGetProcessPeb_ PsGetProcessPeb;
    static PsGetProcessWow64Process_ PsGetProcessWow64Process;
public:
    UnexportedFunctions_();
};