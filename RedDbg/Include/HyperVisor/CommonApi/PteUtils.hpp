#pragma once

namespace Pte
{
    struct PAGE_TABLES_INFO {
        PML4E* Pml4e;
        PDPE* Pdpe;
        PDE* Pde;
        PTE* Pte;
        enum PAGE_TYPE {
            ptUnknown,
            pt32NonPaePage4Kb, // PDE -> PTE -> PA
            pt32NonPaePage4Mb, // PDE -> PA
            pt32PaePage4Kb, // PDPE -> PDE -> PTE -> PA
            pt32PaePage2Mb, // PDPE -> PDE -> PA
            pt64Page4Kb, // PML4E -> PDPE -> PDE -> PTE -> PA
            pt64Page2Mb, // PML4E -> PDPE -> PDE -> PA
            pt64Page1Gb  // PML4E -> PDPE -> PA
        } Type;
    };

    _IRQL_requires_max_(APC_LEVEL)
    BOOLEAN GetPageTables(_In_ PVOID Address, _Out_ PAGE_TABLES_INFO* Info, OPTIONAL _In_ BOOLEAN CustomCr3 = FALSE, OPTIONAL _In_ unsigned long long NCr3 = 0);

    _IRQL_requires_max_(APC_LEVEL)
    BOOLEAN TriggerCopyOnWrite(OPTIONAL PEPROCESS Process, PVOID Address, OPTIONAL OUT PULONG PageSize = NULL);

    _IRQL_requires_max_(APC_LEVEL)
    BOOLEAN IsPagePresent(PVOID Address, OPTIONAL OUT PULONG PageSize);

    _IRQL_requires_max_(APC_LEVEL)
    BOOLEAN IsProcessPagePresent(OPTIONAL PEPROCESS Process, PVOID Address, OPTIONAL OUT PULONG PageSize);

    _IRQL_requires_max_(APC_LEVEL)
    BOOLEAN IsMemoryRangePresent(OPTIONAL PEPROCESS Process, PVOID Address, SIZE_T Size);
}