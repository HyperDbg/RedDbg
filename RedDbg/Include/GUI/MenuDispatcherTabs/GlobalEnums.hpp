#pragma once

namespace MenuBarGlobalVars
{
	namespace DriverTabGlobalVars
	{
		enum DriverTabCheckbox_
		{
			AutomaticallyLoadAfterOpenPEFile,
			ModulesIsEditable,
		};
	}
	namespace EventTabGlobalVars
	{
		enum EventTabCheckbox_
		{
			SystemBreakpoint,
			EntryBreakpoint,
			ExitBreakpoint,
			DebugStrings,
			UserTlsCallbacks,
			UserDllEntry,
			UserDllLoad,
			UserDllUnload,
			ThreadEntry,
			ThreadCreate,
			ThreadExit,
			SetThreadNameExceptions,
			SystemTlsCallback,
			SystemDllEntry,
			SystemDllLoad,
			SyScallTableBreakpoint,
			IDTTableBreakpoint,
			LaunchBreakpoint,
			LoadDriverBreakpoint,
			ServiceStart,
			ServiceCreate,
			ServiceDelete,
			KernelThreadEntry,
			KernelThreadCreate,
			KernelThreadExit,
		};
	}
	namespace EngineTabGlobalVars
	{
		enum EngineTabCheckbox_
		{
			UndecoratedSymbolNames,
			DisableASLR,
			LogIfThreadHasSwitched,
			EnableSourceDebugging,
			SaveDatabaseInProgramDirectory,
			IgnoreIncosistentBreakpoints,
			DefaultMaximumTraceCount,
			AnimationPerStepInterval,
			DefaultSoftwareBreakpointType,
			CalculationType,
		};
	}
	namespace DisassemlyTabGlobalVars
	{
		enum DisassemblyTabCheckbox_
		{
			ArgumentSpaces,
			TabBetweenMnemonicAndArguments,
			HidePointerSizes,
			OnlyShowFSAndGSSegments,
			MemorySpaces,
			UpperCase,
			AutocommentsOnlyOnCip,
			DoNotHighlightOperands,
			HideModuleNameForLocalMemoryAddresses,
			PermanentHighlightingMode,
			DisableBranchDisasemblyPreview,
			Prefix0xForHexVals,
			DoNotShowSourceLinesInComments,
			AssembleInstructionOnDoubleClick,
		};
	}
	namespace GuiTabGlobalVars
	{
		enum GuiTabCheckbox_
		{
			ShowFpuRegistersAsLittleEndian,
			DoNotShowCloseDialog,
			ShowPidAndTidInHex,
			GraphZoomMode,
			ShowRvaAddressesInGraphView,
			AutoFollowOperandInStack,
			ShowExitConfirmationDialog,
		};
	}
	namespace MiscTabGlobalVars
	{
		enum MiscTabCheckBox_
		{
			UseLocalHelpFile = 2,
			ParseWinApiAndShowArgumentsForThem,
			QueryProcessCookie,
			QueryWorkingSetBeforeReadingMemory,
		};
	}
}