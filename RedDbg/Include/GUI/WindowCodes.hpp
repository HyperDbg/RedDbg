#pragma once
enum WindowCodes : const char {
	MenuBarOfTitleBarWindowCode,

	CPUTabWindowCode,
	StackMapTabWindowCode,
	
	ThreadsTabWindowCode,
	CallStackTabWindowCode,
	MemoryMapTabWindowCode,
	SEHTabWindowCode,

	PETabWindowCode,

	//CPUTabWindowCode
	DisasmAndInfoWindowCode,
	RegisterAndCallConventionWindowCode,
	StackAndDumpWindowCode,

	//StackAndDumpWindowCode
	StackWindowCode,
	DumpWindowCode,

	//RegisterAndCallConventionWindowCode
	RegisterWindowCode,
	CallConventionWindowCode,

	//DisasmAndInfoWindowCode
	DisassemblyWindowCode,
	InfoWindowCode,

	//StackMapTabWindowCode
	StackMapAddressesWindowCode,
	StackMapReferencesToAddressesWindowCode,

	//ThreadsTabWindowCode
	ThreadsWindowCode,
	FindInThreadsWindowCode,
	InputFieldInFindThreadsWindowCode,
	ComboBoxOfTypesInFindThreadsWindowCode,

	//CallStackTabWindowCode
	CallStackWindowCode,
	FindInCallStackWindowCode,
	InputFieldInFindCallStackWindowCode,
	ComboBoxOfTypesInFindCallStackWindowCode,

	//MemoryMapTabWindowCode
	MemoryMapWindowCode,
	FindInMemoryMapWindowCode,
	InputFieldInFindMemoryMapWindowCode,
	ComboBoxOfTypesInFindMemoryMapWindowCode,

	//SEHTabWindowCode
	SEHWindowCode,
	FindInSEHWindowCode,
	InputFieldInFindSEHWindowCode,
	ComboBoxOfTypesInFindSEHWindowCode,

	//PETabWindowCode
	PETreeNodeWindowCode,
	PEFileInfoNodeCode,
	PEFileInfoInternalWindowCode,
	PEDoSHeaderNodeCode,
	PERichHeaderNodeCode,
	PENtHeaderNodeCode,
	PEFileHeaderNodeCode,
	PEOptionalHeaderNodeCode,
	PEDataDirectoriesNodeCode,
	PESectionHeaderNodeCode,
	PEImportDirectoryNodeCode,
	PEResourceDirectoryNodeCode,
	PEExceptionDirectoryNodeCode,
	PEDependencyWalkerNodeCode,
	PEImportAdderNodeCode,
	PEStructRebuilderWalkerNodeCode,

	//PEFileInfoInternalWindowCode
	PEFileInfoLeftWindowCode,
	PEFileInfoRightWindowCode,

	//PEFileInfoRightWindowCode
	PEFileInfotTopRightWindowCode,
	PEFileInfoBottomRightWindowCode,

	//PEFileInfoLeftWindowCode
	PEFileInfoWindowCode,
	PEFileInfoHashInfoWindowCode,
	PEFileInfoProtectionInfoWindowCode,

	//PEFileInfoProtectionInfoWindowCode
	PEFileInfoProtectionButtonsWindowCode,
	PEFileInfoProtectionFieldWindowCode,

	//PESectionHeaderNodeCode
	PeSectionHeaderTwoWindowInternalWindowCode,
	PeSectionHeaderTableNodeCode,
	PeInfoOfSectionInHeaderWindowCode,

	//PEImportDirectoryNodeCode
	PeImportDirectoryTwoWindowInternalWindowCode,
	PeImportDirectoryTableNodeCode,
	PeInfoOfImportDllWindowCode,
};