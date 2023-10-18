#pragma once
#include <ntddk.h>

typedef enum MemoryMapperWrapperForMemoryRead_
{
	MemoryMapperWrapperReadPhysicalMemory,
	MemoryMapperWrapperReadVirtualMemory
} MemoryMapperWrapperForMemoryRead;

typedef class MemoryMapperAddresses_
{
public:
	PVOID PteVirtualAddressForRead;  // The virtual address of PTE for read operations
	PVOID VirualAddressForRead;      // The actual kernel virtual address to read

	PVOID PteVirtualAddressForWrite; // The virtual address of PTE for write operations
	PVOID VirualAddressForWrite;     // The actual kernel virtual address to write
} MemoryMapperAddresses, * PMemoryMapperAddresses;


class HvMemoryMapper {
private:
	PMemoryMapperAddresses objMemoryMapperAddresses;
public:
	PVOID CrsAllocateZeroedNonPagedPool(SIZE_T NumberOfBytes)
	{
		PVOID Result = ExAllocatePool(NonPagedPoolNx, NumberOfBytes);

		if (Result != NULL)
			RtlSecureZeroMemory(Result, NumberOfBytes);

		return Result;
	}

	PVOID MemoryMapperMapReservedPageRange(SIZE_T Size)
	{
		return MmAllocateMappingAddress(Size, '4323');
	}

	PVOID MemoryMapperMapPageAndGetPte(PVOID PteAddress)
	{
		PVOID Va;
		PVOID Pte;

		//
		// Reserve the page from system va space
		//
		Va = MemoryMapperMapReservedPageRange(PAGE_SIZE);

		//
		// Get the page's Page Table Entry
		//
		//Pte = MemoryMapperGetPte(Va);

		PteAddress = Pte;

		return Va;
	}

	HvMemoryMapper()
	{
		ULONG ProcessorCount = KeQueryActiveProcessorCount(0);
		objMemoryMapperAddresses = (PMemoryMapperAddresses)CrsAllocateZeroedNonPagedPool(sizeof(MemoryMapperAddresses) * ProcessorCount);

		PVOID TempPte;
		for (size_t i = 0; i < ProcessorCount; i++)
		{
			objMemoryMapperAddresses[i].VirualAddressForRead = MemoryMapperMapPageAndGetPte(&TempPte);
			objMemoryMapperAddresses[i].PteVirtualAddressForRead = TempPte;

			objMemoryMapperAddresses[i].VirualAddressForWrite = MemoryMapperMapPageAndGetPte(&TempPte);
			objMemoryMapperAddresses[i].PteVirtualAddressForWrite = TempPte;
		}
	}

	bool ReadMemorySafeByPhysicalAddressWrapper(MemoryMapperWrapperForMemoryRead TypeOfRead, _In_ UINT64 AddressToRead, _In_ UINT64 BufferToSaveMemory, _In_ SIZE_T SizeToRead);
	bool ReadMemorySafe(_In_ UINT64 VaAddressToRead, _In_ PVOID BufferToSaveMemory, _In_ SIZE_T SizeToRead);
};