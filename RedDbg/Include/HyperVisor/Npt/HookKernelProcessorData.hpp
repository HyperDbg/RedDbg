/*!
    @file HookKernelProcessorData.hpp

    @brief Kernel mode code to initialize and cleanup hook data.

    @author Satoshi Tanda

    @copyright Copyright (c) 2018, Satoshi Tanda. All rights reserved.
 */
#pragma once
#include "Common.hpp"
#include "HookCommon.hpp"
#include "HyperVisor/CommonTypes/PTE.hpp"

typedef struct _APIC_BASE
{
    union
    {
        UINT64 AsUInt64;
        struct
        {
            UINT64 Reserved1 : 8;           // [0:7]
            UINT64 BootstrapProcessor : 1;  // [8]
            UINT64 Reserved2 : 1;           // [9]
            UINT64 EnableX2ApicMode : 1;    // [10]
            UINT64 EnableXApicGlobal : 1;   // [11]
            UINT64 ApicBase : 24;           // [12:35]
        } Fields;
    };
} APIC_BASE, * PAPIC_BASE;

_IRQL_requires_max_(DISPATCH_LEVEL)
_Check_return_
NTSTATUS
InitializeHookData (
    _Outptr_result_nullonfailure_ PHOOK_DATA* HookData
    );

_IRQL_requires_max_(DISPATCH_LEVEL)
_Check_return_
NTSTATUS
InitializePreAllocatedEntries(
    _Out_writes_(NumberOfEntries) PVOID* Entries,
    _In_ ULONG NumberOfEntries
);

_IRQL_requires_max_(DISPATCH_LEVEL)
VOID
CleanupHookData (
    _Inout_ PHOOK_DATA HookData
    );

_Check_return_
PHYSICAL_ADDRESS
GetPml4PhysicalAddress (
    _In_ const HOOK_DATA* HookData
    );
