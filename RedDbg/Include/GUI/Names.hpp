#pragma once
#include <string_view>
#include <vector>
class Names_ 
{
private:
	class Windowses_
	{
	private:
		class TitleBarMenu_
		{
		private:
			class MenuFile_
			{
			private:
				class Shortcut_ {
				public:
					const std::string_view MenuOpenItemShortcutName = "F3";
					const std::string_view MenuAttachItemShortcutName = "Alt + A";
					const std::string_view MenuDetachItemShortcutName = "Ctrl + Alt + A";
					const std::string_view MenuPatchFileItemShortcutName = "Ctrl+P";
				};

				class DropDownMenuDatabaseItems_
				{
				public:
					const std::string_view MenuSaveDatabaseItemName = "Save database";
					const std::string_view MenuReloadDatabaseItemName = "Reload database";
					const std::string_view MenuRestoreBackupDatabaseItemName = "Restore backup database";
					const std::string_view MenuClearDatabaseItemName = "Clear database";
					const std::string_view MenuImportDatabaseItemName = "Import database";
					const std::string_view MenuExportDatabaseItemName = "Export database";
					const std::string_view MenuPatchFileItemName = "Patch File";
					const std::string_view MenuChangeCommandLineItemName = "Change Command Line";
				};
			public:
				const std::string_view MenuOpenItemName = "Open";
				
				const std::string_view DropDownMenuRecentFilesName = "Recent Files";

				const std::string_view MenuAttachItemName = "Attach";
				const std::string_view MenuDetachItemName = "Detach";

				const std::string_view DropDownMenuDatabaseName = "Database";
				DropDownMenuDatabaseItems_ DropDownMenuDatabaseItems;

				const std::string_view MenuPatchFileItemName = "Patch File";
				const std::string_view MenuChangeCommandLineItemName = "Change Command Line";
			public:
				Shortcut_ Shortcut;
			};

			class MenuDebug_
			{
			private:
				class Shortcut_ {
				public:
					const std::string_view MenuRunItemShortcutName = "F9";
					const std::string_view MenuPauseItemShortcutName = "F12";
					const std::string_view MenuRestartItemShortcutName = "Alt+F2";
					const std::string_view MenuCloseItemShortcutName = "Ctrl+Alt+F2";
					const std::string_view MenuStepIntoShortcutItemName = "W";
					const std::string_view MenuStepOverShortcutItemName = "Space";
					const std::string_view MenuStepLeftShortcutItemName = "A";
					const std::string_view MenuStepRightShortcutItemName = "D";
				public:
				};
			public:
				const std::string_view MenuRunItemName = "Run";
				const std::string_view MenuPauseItemName = "Pause";
				const std::string_view MenuRestartItemName = "Restart";
				const std::string_view MenuCloseItemName = "Close";
				const std::string_view MenuStepIntoItemName = "Step into";
				const std::string_view MenuStepOverItemName = "Step over";
				const std::string_view MenuStepLeftItemName = "Step left";
			public:
				const std::string_view MenuStepLeftItemDescription = "Inverted direction of jcc/jmp, and in the variant with \"ret\", you will be asked to change the address in the stack";
			public:
				Shortcut_ Shortcut;
			};

			class MenuTracing_
			{
			private:
				class Shortcut_ {
				public:
					const std::string_view MenuTraceIntoItemShortcutName = "Ctrl+W";
					const std::string_view MenuTraceOverItemShortcutName = "Ctrl+Space";
					const std::string_view MenuTraceLBRItemShortcutName = "Ctrl+J";
				};

				class Tracing_ {
				public:
					const std::string_view BreakCondition = "Break Condition";
					const std::string_view LogText = "Log Text";
					const std::string_view LogCondition = "Log Condition";
					const std::string_view CommandText = "Command Text";
					const std::string_view CommandCondition = "Command Condition";
					const std::string_view ToFunction = "Trace to function";
					const std::string_view ToFunctionCheckBox = "TTF";
					const std::string_view MaximumTraceCount = "Maximum trace count";

					const std::string_view HintConditions = "RBX == B && RAX == A";
					const std::string_view HintLogText = "0x{p:cip} {i:cip}";
					const std::string_view HintCommandText = "RAX == A;StepOut";
					const std::string_view HintTraceCount = "100F";
					const std::string_view HintToFunction = "00007FFC84BC4505";
				};
			public:
				Tracing_ TracingText;

				const std::string_view IntoWindowId = "IntoTraceID";
				const std::string_view IntoHostTitleName = "Trace into";
				const std::string_view MenuTraceIntoItemName = "Trace into";

				const std::string_view OverWindowId = "OverTraceID";
				const std::string_view OverHostTitleName = "Trace over";
				const std::string_view MenuTraceOverItemName = "Trace over";

				const std::string_view LBRWindowId = "LBRTraceID";
				const std::string_view LBRHostTitleName = "Trace LBR";
				const std::string_view MenuTraceLBRItemName = "Trace LBR";
			public:
				Shortcut_ Shortcut;
			};

			class MenuOptions_ 
			{
			private:
				class Preferences_
				{
				private:
					class DriverTab_
					{
					public:
						const std::string_view DriverPath = "Driver path";
						const std::string_view ALAOPF = "Automatically load after open PE file";
						const std::string_view ModulesIsEditable = "Modules is editable";
						//const std::string_view UseGlobalVar = "Use global environment var for driver path";
						
						const std::string_view RedDrvConfVar = "RedDrv";
						const std::string_view HyperDrvConfVar = "HyperDrv";

						std::vector<std::string_view> Names{
							ALAOPF,
							ModulesIsEditable,
							//UseGlobalVar
						};
					};

					class EventsTab_
					{
					public:
						const std::string_view BreakOn = "Break on:";
						const std::string_view User = "User";
						const std::string_view Kernel = "Kernel";
						const std::string_view SystemBreakpoint = "System Breakpoint";
						const std::string_view EntryBreakpoint = "Entry Breakpoint";
						const std::string_view ExitBreakpoint = "Exit Breakpoint";
						const std::string_view DebugStrings = "Debug strings";
						const std::string_view UserTLSCallbacks = "User TLS Callbacks";
						const std::string_view UserDLLEntry = "User DLL Entry";
						const std::string_view UserDLLLoad = "User DLL Load";
						const std::string_view UserDLLUnload = "User DLL Unload";
						const std::string_view ThreadEntry = "Thread entry";
						const std::string_view ThreadCreate = "Thread create";
						const std::string_view ThreadExit = "Thread exit";
						const std::string_view SetThreadNameExceptions = "SetThreadName exceptions";
						const std::string_view SystemTLSCallBack = "System TLS CallBack";
						const std::string_view SystemDLLEntry = "System DLL Entry";
						const std::string_view SystemDLLLoad = "System DLL Load";
						const std::string_view STB = "SyScall Table Breakpoint";
						const std::string_view IDTTableBreakpoint = "IDT Table Breakpoint";
						const std::string_view Launch = "Launch";
						const std::string_view LoadDriver = "Load driver";
						const std::string_view ServiceStart = "Service start";
						const std::string_view ServiceCreate = "Service create";
						const std::string_view ServiceDelete = "Service delete";
						const std::string_view KernelThreadEntry = "Kernel Thread entry";
						const std::string_view KernelThreadCreate = "Kernel Thread Create";
						const std::string_view KernelThreadExit = "Kernel Thread Exit";

						std::vector<std::string_view> Names{
							SystemBreakpoint,
							EntryBreakpoint,
							ExitBreakpoint,
							DebugStrings,
							UserTLSCallbacks,
							UserDLLEntry,
							UserDLLLoad,
							UserDLLUnload,
							ThreadEntry,
							ThreadCreate,
							ThreadExit,
							SetThreadNameExceptions,
							SystemTLSCallBack,
							SystemDLLEntry,
							SystemDLLLoad,
							STB,
							IDTTableBreakpoint,
							Launch,
							LoadDriver,
							ServiceStart,
							ServiceCreate,
							ServiceDelete,
							KernelThreadEntry,
							KernelThreadCreate,
							KernelThreadExit,
						};
					};

					class EngineTab_
					{
					private:
						class DSBTypeList_
						{
						public:
							const std::string_view Int3 = "INT3";
							const std::string_view LongInt3 = "Long INT3";
							const std::string_view UD2 = "UD2";
							const std::string_view CLI = "CLI";
							const std::string_view JMP = "JMP";
						};
						
						class CalculationType_
						{
						public:
							const std::string_view Signed = "Signed";
							const std::string_view Unsigned = "Unsigned";
						};
					public:
						const std::string_view UndecoratedSymbolNames = "Undecorated Symbol Names";
						const std::string_view DisableAslr = "Disable ASLR";
						const std::string_view Ring3App = "It works on Ring 3 and only when you launch PE file through the debugger";
						const std::string_view LogIfThreadHasSwitched = "Log if thread has switched";
						const std::string_view EnableSourceDebugging = "Enable source debugging";
						const std::string_view SaveDataBaseInProgramDirectory = "Save Database in program directory";
						const std::string_view IgnoreIncosistentBreakpoints = "Ignore incosistent breakpoints";
						
						const std::string_view DefaultMaximumTraceCount = "Default Maximum trace count";

						const std::string_view AnimationPerStepInterval = "Animation per-step interval (ms)";

						const std::string_view DefaultSoftwareBreakpointTypeListName = "Default Software Breakpoint Type";
						DSBTypeList_ DSBTypeList;
						const std::string_view CalculationTypeListName = "Calculation Type";
						CalculationType_ CalculationType;

						std::vector<std::string_view> Names{
							UndecoratedSymbolNames,
							DisableAslr,
							LogIfThreadHasSwitched,
							EnableSourceDebugging,
							SaveDataBaseInProgramDirectory,
							IgnoreIncosistentBreakpoints,
							DefaultMaximumTraceCount,
							AnimationPerStepInterval,
							DefaultSoftwareBreakpointTypeListName,
							CalculationTypeListName
						};
					};

					class DisaasemblyTab_
					{
					public:
						const std::string_view ArgumentSpaces = "Argument Spaces";
						const std::string_view TabBetweenMnemonicAndArguments = "Tab between mnemonic and arguments";
						const std::string_view HidePointerSizes = "Hide pointer sizes";
						const std::string_view OnlyShowFSGSSegments = "Only show FS/GS segments";
						const std::string_view MemorySpaces = "Memory spaces";
						const std::string_view Uppercase = "Uppercase";
						const std::string_view AutocommentsOnlyOnCIP = "Autocomments only on CIP";
						const std::string_view DoNotHighlightOperands = "Don't highlight operands";
						const std::string_view HideModuleNameForLocalMemoryAddresses = "Hide module name for local memory addresses";
						const std::string_view PermanentHighlightingMode = "Permanent highlighting mode";
						const std::string_view DisableBranchDisassemblyPreview = "Disable branch disassembly preview";
						const std::string_view PrefixForHexValues = "0x prefix for hex values";
						const std::string_view DoNotShowSourceLinesInComments = "Don't show source lines in comments";
						const std::string_view AssembleIntructionOnDoubleClick = "Assemble instruction on double-click";

						std::vector<std::string_view> Names{
							ArgumentSpaces,
							TabBetweenMnemonicAndArguments,
							HidePointerSizes,
							OnlyShowFSGSSegments,
							MemorySpaces,
							Uppercase,
							AutocommentsOnlyOnCIP,
							DoNotHighlightOperands,
							HideModuleNameForLocalMemoryAddresses,
							PermanentHighlightingMode,
							DisableBranchDisassemblyPreview,
							PrefixForHexValues,
							DoNotShowSourceLinesInComments,
							AssembleIntructionOnDoubleClick
						};
					};

					class GuiTab_
					{
					public:
						const std::string_view SFRALE = "Show FPU registers as little endian";
						const std::string_view DNSCD = "Don't show close dialog";
						const std::string_view ShowPIDTIDInHEX = "Show PID/TID in HEX";
						const std::string_view GraphZoomMode = "Graph zoom mode";
						const std::string_view ShowRVAaddressesInGraphView = "Show RVA addresses in graph view";
						const std::string_view AutoFollowOperandInStack = "Auto follow operand in stack";
						const std::string_view ShowExitConfirmationDialog = "Show exit confirmation dialog";

						std::vector<std::string_view> Names{
							SFRALE,
							DNSCD,
							ShowPIDTIDInHEX,
							GraphZoomMode,
							ShowRVAaddressesInGraphView,
							AutoFollowOperandInStack,
							ShowExitConfirmationDialog
						};
					};

					class MiscTab_
					{
					public:
						const std::string_view SymbolStore = "Symbol Store";
						const std::string_view SymbolPath = "Symbol Path";
						const std::string_view UseLocalHelpFile = "Use local help file";
						const std::string_view ParseWinApi = "Parse winapi and show arguments for them";
						const std::string_view QueryProcessCookie = "Query process cookie";
						const std::string_view QueryWorkingSetBeforeReadingMemory = "Query working set before reading memory";

						std::vector<std::string_view> Names{
							SymbolStore,
							SymbolPath,
							UseLocalHelpFile,
							ParseWinApi,
							QueryProcessCookie,
							QueryWorkingSetBeforeReadingMemory
						};
					};
				public:
					const std::string_view DriverTabName = "Driver";
					DriverTab_ DriverTab;
					const std::string_view EventsTabName = "Events";
					EventsTab_ EventsTab;
					const std::string_view EngineTabName = "Engine";
					EngineTab_ EngineTab;
					const std::string_view DisassemblyTabName = "Disassembly";
					DisaasemblyTab_ DisaasemblyTab;
					const std::string_view GuiTabName = "Gui";
					GuiTab_ GuiTab;
					const std::string_view MiscTabName = "Misc";
					MiscTab_ MiscTab;

					const std::string_view WindowId = "OptionsId";
					const std::string_view HostTitleName = "Options";
				};
			public:
				const std::string_view MenuPreferencesItemName = "Preferences";
				Preferences_ Preferences;
			};

			class MenuHelp_ 
			{
			private:
				class DropDownMenuGitHubItems_
				{
				public:
					const std::string_view MenuRedDbgItemName = "RedDbg Project";
					const std::string_view MenuRedDbgLinkName = "https://github.com/HyperDbg/RedDbg";

					const std::string_view MenuSVMHyperVisorFrameWorkItemName = "SVM HyperVisor FrameWork Project";
					const std::string_view MenuSVMHyperVisorFrameWorkLinkName = "https://github.com/Nitr0-G/SVM-Hypervisor";
				};
			public:
				const std::string_view MenuAboutItemName = "About";

				const std::string_view DropDownMenuGitHubName = "GitHub";
				DropDownMenuGitHubItems_ DropDownMenuGitHubItems;

				const std::string_view MenuCrackLabItemName = "CrackLab =)";

				const std::string_view MenuCrackLabLinkName = "https://cracklab.team/index.php";
			};

			class OpenFile_
			{
			private:
				class Error_
				{
				public:
					const std::string_view hPeFileContentId = "hPeFileContent ERROR";
					const std::string_view hPeFileContentText = "[-] hPeFileContent contains INVALID_HANDLE_VALUE !";

					const std::string_view pImageDOSHeaderOfPeId = "pImageDOSHeaderOfPe ERROR";
					const std::string_view pImageDOSHeaderOfPeText = "[-] pImageDOSHeaderOfPe contains nullptr !";

					const std::string_view pImageNTHeaderOfPeId = "pImageNTHeaderOfPe ERROR";
					const std::string_view pImageNTHeaderOfPeText = "[-] pImageNTHeaderOfPe contains nullptr !";

					const std::string_view pImageNTHeader64Id = "pImageNTHeader64 ERROR";
					const std::string_view pImageNTHeader64Text = "[-] pImageNTHeader64 contains nullptr !";

					const std::string_view pImageSectionHeaderId = "pImageSectionHeader ERROR";
					const std::string_view pImageSectionHeaderText = "[-] pImageSectionHeader contains nullptr !";

					const std::string_view pImageImportHeaderId = "pImageImportHeader ERROR";
					const std::string_view pImageImportHeaderText = "[-] pImageImportHeader contains nullptr !";

					const std::string_view pImageImportDescriptorId = "pImageImportDescriptor ERROR";
					const std::string_view pImageImportDescriptorText = "[-] pImageImportDescriptor contains nullptr !";

					const std::string_view CheckBoxText = "Don't ask me next time";

					const std::string_view OkButton = "OK";
				};
			public:
				const std::string_view OpenID = "OpenFile";
				const std::string_view OpenTitle = "Open";
				const std::string_view Filter = ".exe,.dll,.sys";
				Error_ Error;
			};
		public:
			const std::string_view MenuFileName = "File";
			const std::string_view MenuDebugName = "Debug";
			const std::string_view MenuTracingName = "Tracing";
			const std::string_view MenuOptionsName = "Options";
			const std::string_view MenuHelpName = "Help";

			const std::string_view Device = "\\\\.\\RedDbgHyperVisor";
		public:
			MenuFile_ MenuFile;
			MenuDebug_ MenuDebug;
			MenuTracing_ MenuTracing;
			MenuOptions_ MenuOptions;
			MenuHelp_ MenuHelp;
			OpenFile_ OpenFile;
		};

		class MainDebuggerInterface_ {
		private:
			class MainTabs_ 
			{
			private:
				class CPUTab_
				{
				private:
					class StackAndDump_
					{
					private:
						class Dump_
						{
						public:
							const std::string_view Dump1 = "Dump 1";
							const std::string_view Dump2 = "Dump 2";
							const std::string_view Dump3 = "Dump 3";
							const std::string_view Dump4 = "Dump 4";
							const std::string_view Dump5 = "Dump 5";
							const std::string_view Watch1 = "Watch 1";
							const std::string_view Locals = "Locals";
							const std::string_view Struct = "Struct";
						};
					public:
						const std::string_view StackWindowName = "CPUStackWindow";
						const std::string_view StackWindowTableName = "StackTable";

						const std::string_view StackTableAddressColumnName = "Address";
						const std::string_view StackTableCommentColumnName = "Comment";
						const std::string_view StackTableDataColumnName = "Data";
						const std::string_view StackTableDisabledColumnName = "##";

						Dump_ Dump;
						const std::string_view DumpWindowName = "CPUDumpWindow";

						const std::string_view StackAndDumpVerticalSplitterName = "StackAndDumpVerticalSplitter";
					};

					class RegisterAndCallConvention_
					{
					private:
						class RegistersForWindow_
						{
						private:
							class x64_
							{
							public:
								const std::string_view RIP = "RIP";
								const std::string_view RAX = "RAX";
								const std::string_view RBX = "RBX";
								const std::string_view RCX = "RCX";
								const std::string_view RDX = "RDX";
								const std::string_view RBP = "RBP";
								const std::string_view RSP = "RSP";
								const std::string_view RSI = "RSI";
								const std::string_view RDI = "RDI";
								const std::string_view R8 = "R8";
								const std::string_view R9 = "R9";
								const std::string_view R10 = "R10";
								const std::string_view R11 = "R11";
								const std::string_view R12 = "R12";
								const std::string_view R13 = "R13";
								const std::string_view R14 = "R14";
								const std::string_view R15 = "R15";

								const std::string_view RFLAGS = "RFLAGS";

								const std::vector<std::string_view> x64Regs{
									RIP,
									RAX,
									RBX,
									RCX,
									RDX,
									RBP,
									RSP,
									RSI,
									RDI,
									R8,
									R9,
									R10,
									R11,
									R12,
									R13,
									R14,
									R15,
									RFLAGS
								};
							};
							class x32_
							{
							public:
								const std::string_view EIP = "EIP";
								const std::string_view EAX = "EAX";
								const std::string_view EBX = "EBX";
								const std::string_view ECX = "ECX";
								const std::string_view EDX = "EDX";
								const std::string_view EBP = "EBP";
								const std::string_view ESP = "ESP";
								const std::string_view ESI = "ESI";
								const std::string_view EDI = "EDI";

								const std::string_view EFLAGS = "EFLAGS";

								const std::vector<std::string_view> x32Regs {
									EIP,
									EAX,
									EBX,
									ECX,
									EDX,
									EBP,
									ESP,
									ESI,
									EDI,
									EFLAGS
								};
							};

							class Debug_
							{
							public:
								const std::string_view Dr0 = "DR0";
								const std::string_view Dr1 = "DR1";
								const std::string_view Dr2 = "DR2";
								const std::string_view Dr3 = "DR3";
								//https://en.wikipedia.org/wiki/X86_debug_register#DR4_and_DR5
								const std::string_view Dr4 = "DR4";
								const std::string_view Dr5 = "DR5";
								//https://en.wikipedia.org/wiki/X86_debug_register#DR4_and_DR5
								const std::string_view Dr6 = "DR6";
								const std::string_view Dr7 = "DR7";

								const std::vector<std::string_view> vDr
								{
									Dr0,
									Dr1,
									Dr2,
									Dr3,
									Dr6,
									Dr7
								};
							};

							class BitFlags_
							{
							public:
								const std::string_view ZF = "ZF";
								const std::string_view OF = "OF";
								const std::string_view CF = "CF";
								const std::string_view PF = "PF";
								const std::string_view SF = "SF";
								const std::string_view TF = "TF";
								const std::string_view AF = "AF";
								const std::string_view DF = "DF";
								const std::string_view IF = "IF";

								const std::vector<std::string_view> vBitFlags{
									ZF,
									PF,
									AF,
									OF,
									SF,
									DF,
									CF,
									TF,
									IF
								};
							};
							class Segments_ 
							{
							public:
								const std::string_view GsSegment = "GS";
								const std::string_view EsSegment = "ES";
								const std::string_view CsSegment = "CS";
								const std::string_view FsSegment = "FS";
								const std::string_view DsSegment = "DS";
								const std::string_view SsSegment = "SS";

								const std::vector<std::string_view> vSegments{
									GsSegment,
									FsSegment,
									EsSegment,
									DsSegment,
									CsSegment,
									SsSegment
								};
							};

							class x87_
							{
							public:
								const std::string_view ST0 = "ST(0)";
								const std::string_view ST1 = "ST(1)";
								const std::string_view ST2 = "ST(2)";
								const std::string_view ST3 = "ST(3)";
								const std::string_view ST4 = "ST(4)";
								const std::string_view ST5 = "ST(5)";
								const std::string_view ST6 = "ST(6)";
								const std::string_view ST7 = "ST(7)";

								const std::string_view x87r0 = "x87r0";
								const std::string_view x87r1 = "x87r1";
								const std::string_view x87r2 = "x87r2";
								const std::string_view x87r3 = "x87r3";
								const std::string_view x87r4 = "x87r4";
								const std::string_view x87r5 = "x87r5";
								const std::string_view x87r6 = "x87r6";
								const std::string_view x87r7 = "x87r7";

								const std::vector<std::pair<std::string_view, std::string_view>> vST_X87
								{
									{ST0, x87r0},
									{ST1, x87r1},
									{ST2, x87r2},
									{ST3, x87r3},
									{ST4, x87r4},
									{ST5, x87r5},
									{ST6, x87r6},
									{ST7, x87r7},
								};

								const std::string_view x87TagWord = "x87TagWord";
								const std::string_view x87TW_0 = "x87TW_0";
								const std::string_view x87TW_1 = "x87TW_1";
								const std::string_view x87TW_2 = "x87TW_2";
								const std::string_view x87TW_3 = "x87TW_3";
								const std::string_view x87TW_4 = "x87TW_4";
								const std::string_view x87TW_5 = "x87TW_5";
								const std::string_view x87TW_6 = "x87TW_6";
								const std::string_view x87TW_7 = "x87TW_7";

								const std::vector<std::string_view> vX87TW
								{
									x87TW_0,
									x87TW_1,
									x87TW_2,
									x87TW_3,
									x87TW_4,
									x87TW_5,
									x87TW_6,
									x87TW_7
								};

								const std::string_view x87StatusWord = "x87StatusWord";
								const std::string_view x87SW_B = "x87SW_B";
								const std::string_view x87SW_C1 = "x87SW_C1";
								const std::string_view x87SW_SF = "x87SW_SF";
								const std::string_view x87SW_O = "x87SW_O";
								const std::string_view x87SW_I = "x87SW_I";
								const std::string_view x87SW_C3 = "x87SW_C3";
								const std::string_view x87SW_C0 = "x87SW_C0";
								const std::string_view x87SW_P = "x87SW_P";
								const std::string_view x87SW_Z = "x87SW_Z";
								const std::string_view x87SW_TOP = "x87SW_TOP";
								const std::string_view x87SW_C2 = "x87SW_C2";
								const std::string_view x87SW_ES = "x87SW_ES";
								const std::string_view x87SW_U = "x87SW_U";
								const std::string_view x87SW_D = "x87SW_D";

								const std::vector<std::string_view> vX87SW
								{
									x87SW_B,
									x87SW_C3,
									x87SW_C2,
									x87SW_C1,
									x87SW_C0,
									x87SW_ES,
									x87SW_SF,
									x87SW_P,
									x87SW_U,
									x87SW_O,
									x87SW_Z,
									x87SW_D,
									x87SW_I,
									x87SW_TOP
								};

								const std::string_view x87ControlWord = "x87ControlWord";
								const std::string_view x87CW_IC = "x87CW_IC";
								const std::string_view x87CW_UM = "x87CW_UM";
								const std::string_view x87CW_DM = "x87CW_DM";
								const std::string_view x87CW_ZM = "x87CW_ZM";
								const std::string_view x87CW_OM = "x87CW_OM";
								const std::string_view x87CW_IM = "x87CW_IM";
								const std::string_view x87CW_PM = "x87CW_PM";
								const std::string_view x87CW_PC = "x87CW_PC";
								const std::string_view x87CW_RC = "x87CW_RC";

								const std::vector<std::string_view> vX87CW
								{
									x87CW_IC,
									x87CW_ZM,
									x87CW_PM,
									x87CW_UM,
									x87CW_OM,
									x87CW_PC,
									x87CW_DM,
									x87CW_IM,
									x87CW_RC
								};
							};

							class AVX_SSE_
							{
							public:
								const std::string_view MxCsr = "MxCsr";
								const std::string_view MxCsr_FZ = "MxCsr_FZ";
								const std::string_view MxCsr_OM = "MxCsr_OM";
								const std::string_view MxCsr_UE = "MxCsr_UE";
								const std::string_view MxCsr_OE = "MxCsr_OE";
								const std::string_view MxCsr_IE = "MxCsr_IE";
								const std::string_view MxCsr_PM = "MxCsr_PM";
								const std::string_view MxCsr_ZM = "MxCsr_ZM";
								const std::string_view MxCsr_PE = "MxCsr_PE";
								const std::string_view MxCsr_ZE = "MxCsr_ZE";
								const std::string_view MxCsr_DM = "MxCsr_DM";
								const std::string_view MxCsr_UM = "MxCsr_UM";
								const std::string_view MxCsr_IM = "MxCsr_IM";
								const std::string_view MxCsr_DAZ = "MxCsr_DAZ";
								const std::string_view MxCsr_DE = "MxCsr_DE";
								const std::string_view MxCsr_RC = "MxCsr_RC";

								const std::vector<std::string_view> vMxCsr{
									MxCsr_FZ,
									MxCsr_PM,
									MxCsr_UM,
									MxCsr_OM,
									MxCsr_ZM,
									MxCsr_IM,
									MxCsr_UE,
									MxCsr_PE,
									MxCsr_DAZ,
									MxCsr_OE,
									MxCsr_ZE,
									MxCsr_DE,
									MxCsr_IE,
									MxCsr_DM,
									MxCsr_RC
								};

								const std::string_view Xmm0 = "XMM0";
								const std::string_view Xmm1 = "XMM1";
								const std::string_view Xmm2 = "XMM2";
								const std::string_view Xmm3 = "XMM3";
								const std::string_view Xmm4 = "XMM4";
								const std::string_view Xmm5 = "XMM5";
								const std::string_view Xmm6 = "XMM6";
								const std::string_view Xmm7 = "XMM7";
								const std::string_view Xmm8 = "XMM8";
								const std::string_view Xmm9 = "XMM9";
								const std::string_view Xmm10 = "XMM10";
								const std::string_view Xmm11 = "XMM11";
								const std::string_view Xmm12 = "XMM12";
								const std::string_view Xmm13 = "XMM13";
								const std::string_view Xmm14 = "XMM14";
								const std::string_view Xmm15 = "XMM15";

								const std::string_view Ymm0 = "YMM0";
								const std::string_view Ymm1 = "YMM1";
								const std::string_view Ymm2 = "YMM2";
								const std::string_view Ymm3 = "YMM3";
								const std::string_view Ymm4 = "YMM4";
								const std::string_view Ymm5 = "YMM5";
								const std::string_view Ymm6 = "YMM6";
								const std::string_view Ymm7 = "YMM7";
								const std::string_view Ymm8 = "YMM8";
								const std::string_view Ymm9 = "YMM9";
								const std::string_view Ymm10 = "YMM10";
								const std::string_view Ymm11 = "YMM11";
								const std::string_view Ymm12 = "YMM12";
								const std::string_view Ymm13 = "YMM13";
								const std::string_view Ymm14 = "YMM14";
								const std::string_view Ymm15 = "YMM15";

								const std::vector<std::string_view> x32XmmRegs{
									Xmm0,
									Xmm1,
									Xmm2,
									Xmm3,
									Xmm4,
									Xmm5,
									Xmm6,
									Xmm7
								};

								const std::vector<std::string_view> x64XmmRegs{
									Xmm0,
									Xmm1,
									Xmm2,
									Xmm3,
									Xmm4,
									Xmm5,
									Xmm6,
									Xmm7,
									Xmm8,
									Xmm9,
									Xmm10,
									Xmm11,
									Xmm12,
									Xmm13,
									Xmm14,
									Xmm15
								};
								
								const std::vector<std::string_view> x32YmmRegs {
									Ymm0,
									Ymm1,
									Ymm2,
									Ymm3,
									Ymm4,
									Ymm5,
									Ymm6,
									Ymm7,
								};

								const std::vector<std::string_view> x64YmmRegs{
									Ymm0,
									Ymm1,
									Ymm2,
									Ymm3,
									Ymm4,
									Ymm5,
									Ymm6,
									Ymm7,
									Ymm8,
									Ymm9,
									Ymm10,
									Ymm11,
									Ymm12,
									Ymm13,
									Ymm14,
									Ymm15,
								};
							};
							class Mmx_
							{
							public:
								const std::string_view Mm0 = "MM0";
								const std::string_view Mm1 = "MM1";
								const std::string_view Mm2 = "MM2";
								const std::string_view Mm3 = "MM3";
								const std::string_view Mm4 = "MM4";
								const std::string_view Mm5 = "MM5";
								const std::string_view Mm6 = "MM6";
								const std::string_view Mm7 = "MM7";

								const std::vector<std::string_view> vMmx
								{
									Mm0,
									Mm1,
									Mm2,
									Mm3,
									Mm4,
									Mm5,
									Mm6,
									Mm7
								};
							};
						public:

							const std::string_view LastError = "LastError";
							const std::string_view LastStatus = "LastStatus";

							Segments_ Segments;
							AVX_SSE_ AvxSse;
							Mmx_ Mmx;
							Debug_ Debug;
							BitFlags_ BitFlags;
							x87_ x87;
							x64_ x64;
							x32_ x32;
						};
					public:
						RegistersForWindow_ RegistersForWindow;
						const std::string_view RegisterWindowName = "CPURegisterWindow";
						const std::string_view CallConventionWindowName = "CPUCallConventionWindow";

						const std::string_view RegisterAndCallConventionSplitterName = "RegisterAndCallConventionSplitter";
					};

					class DisasmAndInfo_
					{
					public:
						const std::string_view DisassemblyWindowName = "CPUDisassemblyWindow";
						const std::string_view DisassemblyTableName = "DisassemblyTable";

						const std::string_view DisassemblyTableJmpsMapColumnName = "JmpsMap";
						const std::string_view DisassemblyTableAddressColumnName = "Address";
						const std::string_view DisassemblyTableOpcodeColumnName = "Opcode";
						const std::string_view DisassemblyTableDisassemblyColumnName = "Disassembly";
						const std::string_view DisassemblyTableCommentColumnName = "Comment";
						const std::string_view DisassemblyTableDisabledColumnName = "##";

						const std::string_view InfoWindowName = "CPUInfoWindow";

						const std::string_view DisasmAndInfoSplitterName = "DisassemblyAndInfoSplitter";
					};
				public:
					const std::string_view DisasmAndInfoWindowName = "CPUDisasmAndInfoWindow";
					DisasmAndInfo_ DisassemblyAndInfo;
					const std::string_view RegisterAndCallConventionWindowName = "CPURegisterAndCallConventionWindow";
					RegisterAndCallConvention_ RegisterAndCallConvention;
					const std::string_view StackAndDumpWindowName = "CPUStackAndDumpWindow";
					StackAndDump_ StackAndDump;

					const std::string_view CPUHorizontalSplitterName = "CPUHorizontalSplitter";
					const std::string_view CPUVerticalSplitterName = "CPUVerticalSplitter";
				};

				class StackMapTab_
				{
				private:
					class AddresssesTable_
					{
					public:
						const std::string_view StackMapTableAddressesColumnName = "Address";
					};

					class ReferencesToAddressesTable_
					{
					public:
						const std::string_view StackMapTableIdColumnName = "ID";
						const std::string_view StackMapTableOpcodeColumnName = "Opcode";
						const std::string_view StackMapTableMnemonicColumnName = "Mnemonic";
						const std::string_view StackMapTableActionColumnName = "Action";
						const std::string_view StackMapTableCommentColumnName = "Comment";
					};
				public:
					const std::string_view ReferencesToAddressesWindowName = "ReferencesOfAddressesWindow";
					const std::string_view RefsToAddrsTableName = "StackMapRefsToAddrsTable";
					ReferencesToAddressesTable_ RefsToAddrsTable;

					const std::string_view AddressesWindowName = "AddressesWindow";
					const std::string_view AddressesTableName = "AddrsTable";
					AddresssesTable_ AddrsTable;
					const std::string_view StackMapTableDisabledColumnName = "##";

					const std::string_view StackMapVerticalSplitterName = "StackMapVerticalSplitter";
				};

				//TODO: NotesTab

				class ThreadsTab_
				{
				private:
					class FindInThreads_
					{
					public:
						const std::string_view ComboBoxOfTypesWindowName = "ComboBoxOfTypes";

						const std::string_view InputFieldInFindThreadsWindowName = "Find";

						const std::string_view FindInThreadsVerticalSplitterName = "FindInThreadsVerticalSplitter";
					};
				public: 
					const std::string_view ThreadsWindowName = "ThreadsWindow";

					const std::string_view ThreadsTableName = "ThreadsTable";
					const std::string_view ThreadsTableNumberColumnName = "Number";
					const std::string_view ThreadsTableIDColumnName = "ID";
					const std::string_view ThreadsTableEntryColumnName = "Entry";
					const std::string_view ThreadsTableTEBColumnName = "TEB";
					const std::string_view ThreadsTableStackBaseColumnName = "StackBase";
					const std::string_view ThreadsTableStackLimitColumnName = "StackLimit";
					const std::string_view ThreadsTableRIPColumnName = "RIP";
					const std::string_view ThreadsTableSuspendCountColumnName = "Suspend Count";
					const std::string_view ThreadsTablePriorityColumnName = "Priority";
					const std::string_view ThreadsTableWaitReasonColumnName = "Wait Reason";
					const std::string_view ThreadsTableLastErrorColumnName = "Last Error";
					const std::string_view ThreadsTableUserTimeColumnName = "User Time";
					const std::string_view ThreadsTableKernelTimeColumnName = "Kernel Time";
					const std::string_view ThreadsTableCreationTimeColumnName = "Creation Time";
					const std::string_view ThreadsTableCPUCyclesColumnName = "CPU Cycles";
					const std::string_view ThreadsTableContextSwitchColumnName = "Context switch";
					const std::string_view ThreadsTableNameColumnName = "Name";
					const std::string_view ThreadsTableDisabledColumnName = "##";

					const std::string_view FindInThreadsWindowName = "FindInThreadsWindow";
					FindInThreads_ FindInThreads;

					const std::string_view ThreadsHorizontalSplitterName = "ThreadsHorizontalSplitter";
				};
				
				class CallStackTab_
				{
				private:
					class FindInCallStack_
					{
					public:
						const std::string_view ComboBoxOfTypesWindowName = "ComboBoxOfTypes";

						const std::string_view InputFieldInFindCallStackWindowName = "Find";

						const std::string_view FindInCallStackVerticalSplitterName = "FindInCallStackVerticalSplitter";
					};
				public:
					const std::string_view CallStackWindowName = "CallStackWindow";

					const std::string_view CallStackTableName = "CallStackTable";
					const std::string_view CallStackTableThreadIDColumnName = "Thread ID";
					const std::string_view CallStackTableAddressColumnName = "Address";
					const std::string_view CallStackTableToColumnName = "To";
					const std::string_view CallStackTableFromColumnName = "From";
					const std::string_view CallStackTableSizeColumnName = "Size";
					const std::string_view CallStackTablePartyColumnName = "Party";
					const std::string_view CallStackTableLabelColumnName = "Label";
					const std::string_view CallStackTableDisabledColumnName = "##";

					const std::string_view FindInCallStackWindowName = "FindInCallStackWindow";
					FindInCallStack_ FindInCallStack;

					const std::string_view CallStackHorizontalSplitterName = "CallStackHorizontalSplitter";
				};

				class MemoryMapTab_
				{
				private:
					class FindInMemoryMap_
					{
					public:
						const std::string_view ComboBoxOfTypesWindowName = "ComboBoxOfTypes";

						const std::string_view InputFieldInFindMemoryMapWindowName = "Find";

						const std::string_view FindInMemoryMapVerticalSplitterName = "FindInMemoryMapVerticalSplitter";
					};
				public:
					const std::string_view MemoryMapWindowName = "MemoryMapWindow";

					const std::string_view MemoryMapTableName = "MemoryMapTable";
					const std::string_view MemoryMapTableAddressColumnName = "Address";
					const std::string_view MemoryMapTableSizeColumnName = "Size";
					const std::string_view MemoryMapTablePartyColumnName = "Party";
					const std::string_view MemoryMapTableInfoColumnName = "Info";
					const std::string_view MemoryMapTableContentColumnName = "Content";
					const std::string_view MemoryMapTableTypeColumnName = "Type";
					const std::string_view MemoryMapTableProtectionColumnName = "Protection";
					const std::string_view MemoryMapTableInitialColumnName = "Initial";
					const std::string_view MemoryMapTableDisabledColumnName = "##";

					const std::string_view FindInMemoryMapWindowName = "FindInMemoryMapWindow";
					FindInMemoryMap_ FindInMemoryMap;

					const std::string_view MemoryMapHorizontalSplitterName = "MemoryMapHorizontalSplitter";
				};

				class SEHTab_
				{
				private:
					class FindInSEH_
					{
					public:
						const std::string_view ComboBoxOfTypesWindowName = "ComboBoxOfTypes";

						const std::string_view InputFieldInFindSEHWindowName = "Find";

						const std::string_view FindInSEHVerticalSplitterName = "FindInSEHVerticalSplitter";
					};
				public:
					const std::string_view SEHWindowName = "SEHWindow";

					const std::string_view SEHTableName = "SEHTable";
					const std::string_view SEHTableAddressColumnName = "Address";
					const std::string_view SEHTableHandlerColumnName = "Handler";
					const std::string_view SEHTableModuleLabelColumnName = "Module/Label";
					const std::string_view SEHTableCommentColumnName = "Comment";
					const std::string_view SEHTableDisabledColumnName = "##";

					const std::string_view FindInSEHWindowName = "FindInSEHWindow";
					FindInSEH_ FindInSEH;

					const std::string_view SEHHorizontalSplitterName = "SEHHorizontalSplitter";
				};

				class PETab_
				{
				private:
					class PENodes_
					{
					private:
						class FileInfo_
						{
						private:
							class FileInfoWindow_
							{
							public:
								const std::string_view FileNameFieldName = "File Name";
								const std::string_view FileTypeFieldName = "File Type";
								const std::string_view FileCreatedFieldName = "Created";
								const std::string_view FileModifiedFieldName = "Modified";
								const std::string_view FileAccessedFieldName = "Accessed";

								const std::string_view CompanyNameFieldName = "Company Name";
								const std::string_view FileDescriptionFieldName = "File Description";
								const std::string_view FileVersionFieldName = "File Version";
								const std::string_view FileInternalNameFieldName = "Internal Name";
								const std::string_view FileLegalCopyrightFieldName = "Legal Copyright";
								const std::string_view OriginalFileNameFieldName = "Original Filename";
								const std::string_view ProductFileNameFieldName = "Product Name";
							};

							class HashInfoWindow_
							{
							public:
								//const std::string_view CRC16FieldName = "CRC16";
								const std::string_view CRC32FieldName = "CRC32";
								const std::string_view CRC64FieldName = "CRC64";

								const std::string_view MD2FieldName = "MD2";
								const std::string_view MD4FieldName = "MD4";
								const std::string_view MD5FieldName = "MD5";
								//const std::string_view MD6FieldName = "MD6";

								//const std::string_view WhirlpoolFieldName = "Whirlpool";

								const std::string_view SHA1FieldName = "SHA-1";
								const std::string_view SHA2FieldName = "SHA-2(256)";
								//const std::string_view SHA3FieldName = "SHA-3";
							};

							class ProtectionInfoWindow_
							{
							private:
								class ProtectionButtons_
								{

								};
							public:
								const std::string_view ProtectionFieldWindowName = "ProtectionFieldWindow";
								const std::string_view ProtectionButtonsWindowName = "ProtectionButtonsWindow";
								const std::string_view RightProtectionButtonsRightWindowName = "RightProtectionButtonsRightWindow";

								const std::string_view EntryPointFieldName = "Entry point";
								const std::string_view BaseAddressFieldName = "Base address";
								const std::string_view SectionsFieldName = "Sections";
								const std::string_view EndiannessFieldName = "Endianness";
								const std::string_view UITypeFieldName = "UI Type";

								const std::string_view EngineComboBoxField = "Engine";
								const std::string_view ScanComboBoxName = "##ScanComboBox";
							};
						public:
							const std::string_view PEFileInfoWindowName = "FileInfoWindow";
							FileInfoWindow_ FileInfoWindow;
							const std::string_view PEFileInfoHashInfoWindowName = "HashInfoWindow";
							HashInfoWindow_ HashInfoWindow;
							const std::string_view PEFileInfoProtectionInfoWindowName = "ProtectionInfoWindow";
							ProtectionInfoWindow_ ProtectionInfoWindow;

							const std::string_view DefaultButtonsWindowName = "ConfigurationalButtonsWindowName";
						};

						class DoSHeader_
						{
						private:
							class DoSHeaderToolTip_
							{
							public:
								const std::string_view Emagic = "e_magic";
								const std::string_view Ecblp = "e_cblp";
								const std::string_view Ecp = "e_cp";
								const std::string_view Ecrlc = "e_crlc";
								const std::string_view Ecparhdr = "e_cparhdr";
								const std::string_view EminAlloc = "e_minalloc";
								const std::string_view EmaxAlloc = "e_maxalloc";
								const std::string_view Ess = "e_ss";
								const std::string_view Esp = "e_sp";
								const std::string_view Ecsum = "e_csum";
								const std::string_view Eip = "e_ip";
								const std::string_view Ecs = "e_cs";
								const std::string_view Eifarlc = "e_ifarlc";
								const std::string_view Eovno = "e_ovno";
								const std::string_view Eres = "e_res";
								const std::string_view EresArray1 = "";
								const std::string_view EresArray2 = "";
								const std::string_view EresArray3 = "";
								const std::string_view EoemId = "e_oemid";
								const std::string_view EoemInfo = "e_oeminfo";
								const std::string_view Eres2 = "e_res2";
								const std::string_view Eres2Array1 = "";
								const std::string_view Eres2Array2 = "";
								const std::string_view Eres2Array3 = "";
								const std::string_view Eres2Array4 = "";
								const std::string_view Eres2Array5 = "";
								const std::string_view Eres2Array6 = "";
								const std::string_view Eres2Array7 = "";
								const std::string_view Eres2Array8 = "";
								const std::string_view Eres2Array9 = "";
								const std::string_view Eifanew = "e_ifanew";
							};
						public:
							DoSHeaderToolTip_ DoSHeaderToolTip;

							const std::string_view Magic = "Magic";
							const std::string_view BytesOnLastPageOfFile = "Bytes on last page of file";
							const std::string_view PagesInFile = "Pages in file";
							const std::string_view Relocations = "Relocations";
							const std::string_view SizeOfHeader = "Size of header";
							const std::string_view MinExtraParagraphsNeeded = "Min extra paragraphs needed";
							const std::string_view MaxExtraParagraphsNeeded = "Max extra paragraphs needed";
							const std::string_view InitialSS = "Initial SS";
							const std::string_view InitialSP = "Initial SP";
							const std::string_view Checksum = "Checksum";
							const std::string_view InitialIP = "Initial IP";
							const std::string_view InitialCS = "Initial CS";
							const std::string_view OffsetOfRelocationTable = "Offset of relocation table";
							const std::string_view OverlayNumber = "Overlay number";
							const std::string_view Eres = "e_res";
							const std::string_view EresArray1 = "";
							const std::string_view EresArray2 = "";
							const std::string_view EresArray3 = "";
							const std::string_view OEMIdentifier = "OEM identifier";
							const std::string_view OEMInformation = "OEM information";
							const std::string_view Eres2 = "e_res2";
							const std::string_view Eres2Array1 = "";
							const std::string_view Eres2Array2 = "";
							const std::string_view Eres2Array3 = "";
							const std::string_view Eres2Array4 = "";
							const std::string_view Eres2Array5 = "";
							const std::string_view Eres2Array6 = "";
							const std::string_view Eres2Array7 = "";
							const std::string_view Eres2Array8 = "";
							const std::string_view Eres2Array9 = "";
							const std::string_view AddressOfNewEXEHeader = "Address Of New EXE Header";

							const std::vector<std::string_view> DosHeaderToopTipNames{
								DoSHeaderToolTip.Emagic,
								DoSHeaderToolTip.Ecblp,
								DoSHeaderToolTip.Ecp,
								DoSHeaderToolTip.Ecrlc,
								DoSHeaderToolTip.Ecparhdr,
								DoSHeaderToolTip.EminAlloc,
								DoSHeaderToolTip.EmaxAlloc,
								DoSHeaderToolTip.Ess,
								DoSHeaderToolTip.Esp,
								DoSHeaderToolTip.Ecsum,
								DoSHeaderToolTip.Eip,
								DoSHeaderToolTip.Ecs,
								DoSHeaderToolTip.Eifarlc,
								DoSHeaderToolTip.Eovno,
								DoSHeaderToolTip.Eres,
								DoSHeaderToolTip.EresArray1,
								DoSHeaderToolTip.EresArray2,
								DoSHeaderToolTip.EresArray3,
								DoSHeaderToolTip.EoemId,
								DoSHeaderToolTip.EoemInfo,
								DoSHeaderToolTip.Eres2,
								DoSHeaderToolTip.Eres2Array1,
								DoSHeaderToolTip.Eres2Array2,
								DoSHeaderToolTip.Eres2Array3,
								DoSHeaderToolTip.Eres2Array4,
								DoSHeaderToolTip.Eres2Array5,
								DoSHeaderToolTip.Eres2Array6,
								DoSHeaderToolTip.Eres2Array7,
								DoSHeaderToolTip.Eres2Array8,
								DoSHeaderToolTip.Eres2Array9,
								DoSHeaderToolTip.Eifanew,
							};

							

							const std::vector<std::string_view> DoSHeaderNames{
								Magic,
								BytesOnLastPageOfFile,
								PagesInFile,
								Relocations,
								SizeOfHeader,
								MinExtraParagraphsNeeded,
								MaxExtraParagraphsNeeded,
								InitialSS,
								InitialSP,
								Checksum,
								InitialIP,
								InitialCS,
								OffsetOfRelocationTable,
								OverlayNumber,
								Eres,
								EresArray1,
								EresArray2,
								EresArray3,
								OEMIdentifier,
								OEMInformation,
								Eres2,
								Eres2Array1,
								Eres2Array2,
								Eres2Array3,
								Eres2Array4,
								Eres2Array5,
								Eres2Array6,
								Eres2Array7,
								Eres2Array8,
								Eres2Array9,
								AddressOfNewEXEHeader
							};
						};

						class RichHeader_
						{
						public:
							const std::string_view LinkerImportLibraryutableName = "Linker, Import Library";
							const std::string_view IATEntryName = "IAT Entry";
							const std::string_view MasmAsmCoffName = "MASM, ASM COFF";
							const std::string_view CvtresResToCoff = "CVTRES, RES to COFF";
							const std::string_view LinkerLinkName = "Linker, Link";

							const std::vector<std::string_view> RichHeaderNames{
								LinkerImportLibraryutableName,
								IATEntryName,
								MasmAsmCoffName,
								CvtresResToCoff,
								LinkerLinkName
							};
						};

						class FileHeader_
						{
						private:
							class Machine_
							{
							private:
								class ToolTip_
								{
								public:
									const std::string_view AlphaAxp = "Alpha AXP, 32-bit address space";
									const std::string_view Alpha64 = "Alpha 64, 64-bit address space ";
									const std::string_view Am33 = "Matsushita AM33";
									const std::string_view Amd64 = "Amd x64";
									const std::string_view Arm = "ARM little endian";
									const std::string_view Arm64 = "ARM64 little endian";
									const std::string_view ArmNt = "ARM Thumb-2 little endian";
									const std::string_view EfiByteCode = "EFI byte code";
									const std::string_view Intel386 = "Intel 386 or later processors and compatible processors";
									const std::string_view Ia64 = "Intel Itanium processor family";
									const std::string_view LoongArch32 = "LoongArch 32-bit processor family";
									const std::string_view LoongArch64 = "LoongArch 64-bit processor family";
									const std::string_view M32R = "Mitsubishi M32R little endian";
									const std::string_view Mips16 = "MIPS16";
									const std::string_view MipsFpu = "MIPS with FPU";
									const std::string_view MipsFpu16 = "MIPS16 with FPU";
									const std::string_view PowerPc = "Power PC little endian";
									const std::string_view PowerPcFp = "Power PC with floating point support";
									const std::string_view R4000 = "MIPS little endian";
									const std::string_view RiscV32 = "RISC-V 32-bit address space";
									const std::string_view RiscV64 = "RISC-V 64-bit address space";
									const std::string_view RiscV128 = "RISC-V 128-bit address space";
									const std::string_view SH3 = "Hitachi SH3";
									const std::string_view SH3DSP = "Hitachi SH3 DSP";
									const std::string_view SH4 = "Hitachi SH4";
									const std::string_view SH5 = "Hitachi SH5";
									const std::string_view Thumb = "Thumb";
									const std::string_view WCEMIPSV2 = "MIPS little-endian WCE v2";
								};
							public:
								const std::string_view AlphaAxp = "Alpha Axp";
								const std::string_view Alpha64 = "Alpha 64";
								const std::string_view Am33 = "Am33";
								const std::string_view Amd64 = "Amd64";
								const std::string_view Arm = "Arm";
								const std::string_view Arm64 = "Arm64";
								const std::string_view ArmNt = "ArmNt";
								const std::string_view EfiByteCode = "Efi Byte Code";
								const std::string_view Intel386 = "Intel 386";
								const std::string_view Ia64 = "Ia 64";
								const std::string_view LoongArch32 = "LoongArch 32";
								const std::string_view LoongArch64 = "LoongArch 64";
								const std::string_view M32R = "M32R";
								const std::string_view Mips16 = "MIPS16";
								const std::string_view MipsFpu = "MIPSFPU";
								const std::string_view MipsFpu16 = "MIPS16FPU";
								const std::string_view PowerPc = "PowerPc";
								const std::string_view PowerPcFp = "PowerPcFp";
								const std::string_view R4000 = "R4000";
								const std::string_view RiscV32 = "RISC-V 32";
								const std::string_view RiscV64 = "RISC-V 64";
								const std::string_view RiscV128 = "RISC-V 128";
								const std::string_view SH3 = "SH3";
								const std::string_view SH3DSP = "SH3 DSP";
								const std::string_view SH4 = "SH4";
								const std::string_view SH5 = "SH5";
								const std::string_view Thumb = "Thumb";
								const std::string_view WCEMIPSV2 = "WCEMIPSV2";

								ToolTip_ ToolTip;
								/*
								const std::string_view Intel386 = "Intel386";
								const std::string_view R3000Mips = "R3000Mips";
								const std::string_view R4000Mips = "R4000Mips";
								const std::string_view R10000Mips = "R10000Mips";
								const std::string_view MipsWceV2 = "MipsWceV2";
								const std::string_view AlphaAxp = "AlphaAxp";
								const std::string_view SH3 = "SH3";
								const std::string_view SH3DSP = "SH3DSP";
								const std::string_view SH3E = "SH3E";
								const std::string_view SH4 = "SH4";
								const std::string_view SH5 = "SH5";
								const std::string_view Arm = "Arm";
								const std::string_view ArmThumb = "ArmThumb";
								const std::string_view ArmAm33 = "ArmAm33";
								const std::string_view IbmPowerPc = "IbmPowerPc";
								const std::string_view IbmPowerPcFp = "IbmPowerPcFp";
								const std::string_view Intel64 = "Intel64";
								const std::string_view Mips16 = "Mips16";
								const std::string_view MipsFpu = "MipsFpu";
								const std::string_view MipsFpu16 = "MipsFpu16";
								const std::string_view Alpha64 = "Alpha64";
								const std::string_view InfineonTriCore = "InfineonTriCore";
								const std::string_view InfineonCEF = "InfineonCEF";
								const std::string_view EfiByteCode = "EfiByteCode";
								const std::string_view Amd64K8 = "Amd64K8";
								const std::string_view M32R = "M32R";
								const std::string_view CEE = "CEE";
								*/
							};

							class Charateristics_
							{
							public:
								const std::string_view FileIsExecutable = "File is executable";
								const std::string_view FileIsDll = "File is dll";
								const std::string_view SystemFile = "System file";
								const std::string_view RelocationInfoStrippedFromFile = "Relocation info stripped from file";
								const std::string_view LineNumbersStrippedFromFile = "Line numbers stripped from file";
								const std::string_view AgressivelyTrimWorkingSet = "Agressively trim working set";
								const std::string_view AppCanHandle2GbAddressSpaceOrMore = "App can handle > 2Gb AddressSpace";
								const std::string_view BytesOfMachineWordAreReversedLow = "Bytes of machine word are reversed(Low)";
								const std::string_view Bit32WordMachine = "32 Bit word machine";
								const std::string_view DebuggingInfoStrippedFromFileInDgbFile = "Debugging info stripped from file in .dgb file";
								const std::string_view IfImageIsOnRemovableMediaCopyAndRunFromSwapFile = "If image is on removable media, copy and run from swap file";
								const std::string_view IfImageIsOnNetCopyAndRunFromSwapFile = "32 Bit word machine";
								const std::string_view FileShouldOnlyBeRunOnUpMachine = "File should only be run on up machine";
								const std::string_view BytesOfMachineWordAreReversedHigh = "Bytes of machine word are reversed(High)";
							};
						public:
							const std::string_view MachineName = "Machine";
							Machine_ Machine;
							const std::string_view NumberOfSectionsName = "Number of sections";
							const std::string_view TimeDataStampName = "Time data stamp";
							const std::string_view PointerToSymbolTableName = "Pointer to symbol table";
							const std::string_view NumberOfSymbolsName = "Number of symbols";
							const std::string_view SizeOfOptionalHeaderName = "Size of optional header";
							const std::string_view CharateristicsName = "Charateristics";
							Charateristics_ Charateristics;

							const std::vector<std::string_view> CharateristicsNames{
								Charateristics.FileIsExecutable,
								Charateristics.FileIsDll,
								Charateristics.SystemFile,
								Charateristics.RelocationInfoStrippedFromFile,
								Charateristics.LineNumbersStrippedFromFile,
								Charateristics.AgressivelyTrimWorkingSet,
								Charateristics.AppCanHandle2GbAddressSpaceOrMore,
								Charateristics.BytesOfMachineWordAreReversedLow,
								Charateristics.Bit32WordMachine,
								Charateristics.DebuggingInfoStrippedFromFileInDgbFile,
								Charateristics.IfImageIsOnRemovableMediaCopyAndRunFromSwapFile,
								Charateristics.IfImageIsOnNetCopyAndRunFromSwapFile,
								Charateristics.FileShouldOnlyBeRunOnUpMachine,
								Charateristics.BytesOfMachineWordAreReversedHigh
							};

							const std::vector<std::string_view> MachineNames{
								Machine.AlphaAxp,
								Machine.Alpha64,
								Machine.Am33,
								Machine.Amd64,
								Machine.Arm,
								Machine.Arm64,
								Machine.ArmNt,
								Machine.EfiByteCode,
								Machine.Intel386,
								Machine.Ia64,
								Machine.LoongArch32,
								Machine.LoongArch64,
								Machine.M32R,
								Machine.Mips16,
								Machine.MipsFpu,
								Machine.MipsFpu16,
								Machine.PowerPc,
								Machine.PowerPcFp,
								Machine.R4000,
								Machine.RiscV32,
								Machine.RiscV64,
								Machine.RiscV128,
								Machine.SH3,
								Machine.SH3DSP,
								Machine.SH4,
								Machine.SH5,
								Machine.Thumb,
								Machine.WCEMIPSV2,
							};

							const std::vector<std::string_view> MachineToolTips{
								Machine.ToolTip.AlphaAxp,
								Machine.ToolTip.Alpha64,
								Machine.ToolTip.Am33,
								Machine.ToolTip.Amd64,
								Machine.ToolTip.Arm,
								Machine.ToolTip.Arm64,
								Machine.ToolTip.ArmNt,
								Machine.ToolTip.EfiByteCode,
								Machine.ToolTip.Intel386,
								Machine.ToolTip.Ia64,
								Machine.ToolTip.LoongArch32,
								Machine.ToolTip.LoongArch64,
								Machine.ToolTip.M32R,
								Machine.ToolTip.Mips16,
								Machine.ToolTip.MipsFpu,
								Machine.ToolTip.MipsFpu16,
								Machine.ToolTip.PowerPc,
								Machine.ToolTip.PowerPcFp,
								Machine.ToolTip.R4000,
								Machine.ToolTip.RiscV32,
								Machine.ToolTip.RiscV64,
								Machine.ToolTip.RiscV128,
								Machine.ToolTip.SH3,
								Machine.ToolTip.SH3DSP,
								Machine.ToolTip.SH4,
								Machine.ToolTip.SH5,
								Machine.ToolTip.Thumb,
								Machine.ToolTip.WCEMIPSV2,
							};

							const std::vector<std::string_view> FileHeaderNames{
								MachineName,
								NumberOfSectionsName,
								TimeDataStampName,
								PointerToSymbolTableName,
								NumberOfSymbolsName,
								SizeOfOptionalHeaderName,
								CharateristicsName
							};
						};

						class OptionalHeader_
						{
						private:
							class Magic_
							{
							public:
								const std::string_view PE64 = "Pe64";
								const std::string_view PE32 = "Pe32";
								const std::string_view Rom = "Rom";
							};

							class Subsystem_
							{
							public:
								const std::string_view Native = "Native";
								const std::string_view WindowsGUI = "Windows GUI";
								const std::string_view WindowsConsole = "Windows Console";
								const std::string_view Os2Console = "OS/2 Console";
								const std::string_view PosixConsole = "Posix Console";
								const std::string_view NativeWin9xDriver = "Native Win9x Driver";
								const std::string_view EfiApplication = "Efi Application";
								const std::string_view EfiBootDriver = "Efi Boot Driver";
								const std::string_view EfiRuntimeDriver = "Efi Runtime Driver";
								const std::string_view EfiRom = "Efi Rom";
								const std::string_view XBox = "XBox";
								const std::string_view WindowsBootApplication = "Windows Boot Application";
							};

							class DllCharateristics_
							{
							public:
								const std::string_view DllCanMove = "DLL can move";
								const std::string_view CodeIntegrityImage = "Code integrity image";
								const std::string_view ImageIsNxCompatible = "Image is nx compatible";
								const std::string_view ImageUnderstandsIsolationAndDoesntWantIt = "Image understands isolation and doesn't want it";
								const std::string_view ImageDoesntUseSEH = "Image doesn't use SEH";
								const std::string_view DoNotBindThisImage = "Do not bind this image";
								const std::string_view DriverUsesWDMModel = "Driver uses WDM model";
							};
						public:
							const std::string_view MagicName = "Magic";
							Magic_ Magic;
							const std::string_view MajorLinkerVersionName = "Major Linker Version";
							const std::string_view MinorLinkerVersionName = "Minor Linker Version";
							const std::string_view SizeOfCodeName = "Size Of Code";
							const std::string_view SizeOfInitializedDataName = "Size Of Initialized Data";
							const std::string_view SizeOfUninitializedDataName = "Size Of Uninitialized Data";
							const std::string_view EntrypointName = "Entry point";
							const std::string_view BaseOfCodeName = "Base Of Code";
							const std::string_view ImageBaseName = "Image Base";
							const std::string_view SectionAlignmentName = "Section Alignment";
							const std::string_view FileAlignmentName = "File Alignment";
							const std::string_view MajorOSVersionName = "Major OS Version";
							const std::string_view MinorOSVersionName = "Minor OS Version";
							const std::string_view MajorImageVersionName = "Major Image Version";
							const std::string_view MinorImageVersionName = "Minor Image Version";
							const std::string_view MajorSubsystemVersionName = "Major Subsystem Version";
							const std::string_view MinorSubsystemVersionName = "Minor Subsystem Version";
							const std::string_view Win32VersionValueName = "Win32 Version Value";
							const std::string_view SizeOfImageName = "Size Of Image";
							const std::string_view SizeOfHeadersName = "Size Of Headers";
							const std::string_view CheckSumName = "CheckSum";
							const std::string_view SubsystemName = "Subsystem";
							Subsystem_ Subsystem;
							const std::string_view DLLCharacteristicsName = "DLL Characteristics";
							DllCharateristics_ DllCharateristics;
							const std::string_view SizeOfStackReserveName = "Size Of Stack Reserve";
							const std::string_view SizeOfStackCommitName = "Size Of Stack Commit";
							const std::string_view SizeOfHeapReserve = "Size Of Heap Reserve";
							const std::string_view SizeOfHeapCommit = "Size Of Heap Commit";
							const std::string_view LoaderFlags = "Loader Flags";
							const std::string_view NumberOfRVAAndSizes = "Number Of RVA And Sizes";

							const std::vector<std::string_view> OptionalHeaderNames{
								MagicName,
								MajorLinkerVersionName,
								MinorLinkerVersionName,
								SizeOfCodeName,
								SizeOfInitializedDataName,
								SizeOfUninitializedDataName,
								EntrypointName,
								BaseOfCodeName,
								ImageBaseName,
								SectionAlignmentName,
								FileAlignmentName,
								MajorOSVersionName,
								MinorOSVersionName,
								MajorImageVersionName,
								MinorImageVersionName,
								MajorSubsystemVersionName,
								MinorSubsystemVersionName,
								Win32VersionValueName,
								SizeOfImageName,
								SizeOfHeadersName,
								CheckSumName,
								SubsystemName,
								DLLCharacteristicsName,
								SizeOfStackReserveName,
								SizeOfStackCommitName,
								SizeOfHeapReserve,
								SizeOfHeapCommit,
								LoaderFlags,
								NumberOfRVAAndSizes
							};

							const std::vector<std::string_view> MagicNames{
								Magic.PE64,
								Magic.PE32,
								Magic.Rom
							};

							const std::vector<std::string_view> SubsystemNames{
								Subsystem.Native,
								Subsystem.WindowsGUI,
								Subsystem.WindowsConsole,
								Subsystem.Os2Console,
								Subsystem.PosixConsole,
								Subsystem.NativeWin9xDriver,
								Subsystem.EfiApplication,
								Subsystem.EfiBootDriver,
								Subsystem.EfiRuntimeDriver,
								Subsystem.EfiRom,
								Subsystem.XBox,
								Subsystem.WindowsBootApplication,
							};

							const std::vector<std::string_view> DllCharateristicsNames{
								DllCharateristics.DllCanMove,
								DllCharateristics.CodeIntegrityImage,
								DllCharateristics.ImageIsNxCompatible,
								DllCharateristics.ImageUnderstandsIsolationAndDoesntWantIt,
								DllCharateristics.ImageDoesntUseSEH,
								DllCharateristics.DoNotBindThisImage,
								DllCharateristics.DriverUsesWDMModel
							};
						};

						class DataDirectories_
						{
						public:
							const std::string_view ExportDirectoryRva = "Export Directory RVA";
							const std::string_view ExportDirectorySize = "Export Directory Size";

							const std::string_view ImportDirectoryRva = "Import Directory RVA";
							const std::string_view ImportDirectorySize = "Import Directory Size";

							const std::string_view ResourceDirectoryRva = "Resource Directory RVA";
							const std::string_view ResourceDirectorySize = "Resource Directory Size";

							const std::string_view ExceptionDirectoryRva = "Exception Directory RVA";
							const std::string_view ExceptionDirectorySize = "Exception Directory Size";

							const std::string_view SecurityDirectoryRva = "Security Directory RVA";
							const std::string_view SecurityDirectorySize = "Security Directory Size";

							const std::string_view RelocationDirectoryRVA = "Relocation Directory RVA";
							const std::string_view RelocationDirectorySize = "Relocation Directory Size";

							const std::string_view DebugDirectoryRVA = "Debug Directory RVA";
							const std::string_view DebugDirectorySize = "Debug Directory Size";

							const std::string_view ArchitectureDirectoryRVA = "Architecture Directory RVA";
							const std::string_view ArchitectureDirectorySize = "Architecture Directory Size";

							const std::string_view Reserved = "Reserved";

							const std::string_view TLSDirectoryRVA = "TLS Directory RVA";
							const std::string_view TLSDirectorySize = "TLS Directory Size";

							const std::string_view ConfigurationDirectoryRVA = "Configuration Directory RVA";
							const std::string_view ConfigurationDirectorySize = "Configuration Directory Size";

							const std::string_view BoundImportDirectoryRVA = "Bound Import Directory RVA";
							const std::string_view BoundImportDirectorySize = "Bound Import Directory Size";

							const std::string_view ImportAddressTableDirectoryRVA = "Import Address Table Directory RVA";
							const std::string_view ImportAddressTableDirectorySize = "Import Address Table Directory Size";

							const std::string_view DelayImportDirectoryRVA = "Delay Import Directory RVA";
							const std::string_view DelayImportDirectorySize = "Delay Import Directory Size";

							//const std::string_view COMRuntimeDescriptorRVA = "COM Runtime Descriptor RVA";
							//const std::string_view COMRuntimeDescriptorSize = "COM Runtime Descriptor Size";

							const std::string_view NetMetaDataDirectoryRVA = ".Net MetaData Directory RVA";
							const std::string_view NetMetaDataDirectorySize = ".Net MetaData Directory Size";

							const std::vector<std::string_view> DataDirectoriesNames{
								ExportDirectoryRva,
								ExportDirectorySize,
								ImportDirectoryRva,
								ImportDirectorySize,
								ResourceDirectoryRva,
								ResourceDirectorySize,
								ExceptionDirectoryRva,
								ExceptionDirectorySize,
								SecurityDirectoryRva,
								SecurityDirectorySize,
								RelocationDirectoryRVA,
								RelocationDirectorySize,
								DebugDirectoryRVA,
								DebugDirectorySize,
								ArchitectureDirectoryRVA,
								ArchitectureDirectorySize,
								Reserved,
								Reserved,
								TLSDirectoryRVA,
								TLSDirectorySize,
								ConfigurationDirectoryRVA,
								ConfigurationDirectorySize,
								BoundImportDirectoryRVA,
								BoundImportDirectorySize,
								ImportAddressTableDirectoryRVA,
								ImportAddressTableDirectorySize,
								DelayImportDirectoryRVA,
								DelayImportDirectorySize,
								//COMRuntimeDescriptorRVA,
								//COMRuntimeDescriptorSize,
								NetMetaDataDirectoryRVA,
								NetMetaDataDirectorySize
							};
						};

						class SectionHeader_
						{
						private: 
							class SectionCharateristic_
							{
							public:
								const std::string_view IsShareable = "Is shareable";
								const std::string_view IsExecutable = "Is executable";
								const std::string_view IsReadable = "Is readable";
								const std::string_view IsWriteable = "Is writeable";
								const std::string_view ContainsExtendedRelocations = "Contains extended relocations";
								const std::string_view CanBeDiscarded = "Can be discarded";
								const std::string_view IsNotCachable = "Is not cachable";
								const std::string_view IsNotPageable = "Is not pageable";
								const std::string_view NoPad = "No pad";
								const std::string_view ContainsCode = "Contains code";
								const std::string_view ContainsInitData = "Contains initialized data";
								const std::string_view ContainsUninitData = "Contains uninitialized data";
								const std::string_view ContainsInformation = "Contains information";
								const std::string_view ContentIsNotAPartOfImage = "Contains won't be become part of image";
								const std::string_view ContainsComData = "Contains com data";
								const std::string_view ResetBitsInTlbEntries = "Reset speculative exceptions handling bits in the TLB entries";
								const std::string_view ContentCanBeAccessedRelativeToGp = "Content can be accessed relative to GP";
							};
						public:
							const std::string_view TableWindowName = "Table Section Headers";
							const std::string_view InfoSectionWindowName = "Info Of Section";

							SectionCharateristic_ SectionCharateristic;

							std::vector<std::string_view> SectionCharateristics{
								SectionCharateristic.IsShareable,
								SectionCharateristic.IsExecutable,
								SectionCharateristic.IsReadable,
								SectionCharateristic.IsWriteable,
								SectionCharateristic.ContainsExtendedRelocations,
								SectionCharateristic.CanBeDiscarded,
								SectionCharateristic.IsNotCachable,
								SectionCharateristic.IsNotPageable,
								SectionCharateristic.NoPad,
								SectionCharateristic.ContainsCode,
								SectionCharateristic.ContainsInitData,
								SectionCharateristic.ContainsUninitData,
								SectionCharateristic.ContainsInformation,
								SectionCharateristic.ContentIsNotAPartOfImage,
								SectionCharateristic.ContainsComData,
								SectionCharateristic.ResetBitsInTlbEntries,
								SectionCharateristic.ContentCanBeAccessedRelativeToGp,
							};
						};
					public:
						const std::string_view PEFileNodeName = "File";
						FileInfo_ FileInfo;
						const std::string_view PEDoSHeaderNodeName = "DoS Header";
						DoSHeader_ DoSHeader;
						const std::string_view PERichHeaderNodeName = "Rich Header";
						RichHeader_ RichHeader;
						const std::string_view PENtHeaderNodeName = "Nt Headers";
						const std::string_view PEFileHeaderNodeName = "COFF File Header";
						FileHeader_ FileHeader;
						const std::string_view PEOptionalHeaderNodeName = "Optional Header";
						OptionalHeader_ OptionalHeader;
						const std::string_view PEDataDirectoriesNodeName = "Data Directories";
						DataDirectories_ DataDirectories;
						const std::string_view PESectionHeadersNodeName = "Section Headers";
						SectionHeader_ SectionHeader;
						const std::string_view PEImportDirectoryNodeName = "Import Directory";
						const std::string_view PEResourceDirectoryNodeName = "Resource Directory";
						const std::string_view PEExceptionDirectoryNodeName = "Exception Directory";
						const std::string_view PEDependencyWalkerNodeName = "Dependency Walker";
						const std::string_view PEImportAdderNodeName = "Import Adder";
						const std::string_view PEStructRebuilderNodeName = "Pe struct rebuilder";

						const std::string_view PENodeLeftWindowName = "LeftWindow";
						const std::string_view PENodeRightWindowName = "RightWindow";
					};
				public:
					const std::string_view PEWindowName = "PEWindow";
					PENodes_ PENodes;
					

					const std::string_view PEHorizontalSplitterName = "PEHorizontalSplitter";
					const std::string_view PEVerticalSplitterName = "PEVerticalSplitter";
				};
			public:
				const std::string_view MainCPUTabName = "CPU";
				const std::string_view MainStackMapTabName = "Stack Map";
				const std::string_view MainNotesTabName = "Notes";
				const std::string_view MainThreadsTabName = "Threads";
				const std::string_view MainCallStackTabName = "Call Stack";
				const std::string_view MainReferencesTabName = "References";
				const std::string_view MainLogTabName = "Log";
				const std::string_view MainBreakpointsTabName = "Breakpoints";
				const std::string_view MainMemoryMapTabName = "Memory Map";
				const std::string_view MainSEHTabName = "SEH";
				const std::string_view MainScriptTabName = "Script";
				const std::string_view MainSymbolsTabName = "Symbols";
				const std::string_view MainSourceTabName = "Source";
				const std::string_view MainHandlesTabName = "Handles";
				const std::string_view MainTraceTabName = "Trace";
				const std::string_view MainPETabName = "PE";
				const std::string_view MainLiftersTabName = "Lifters";
				const std::string_view MainDevirtualizersTabName = "Devirtualizers";
				const std::string_view MainDeobfuscatorsTabName = "Deobfuscators";

				const std::string_view MainNewTabName = "Tab - ";
			public:
				CPUTab_ CPUTab;
				StackMapTab_ StackMapTab;
				ThreadsTab_ ThreadsTab;
				CallStackTab_ CallStackTab;
				MemoryMapTab_ MemoryMapTab;
				SEHTab_ SEHTab;
				PETab_ PETab;
			};
		public:
			const std::string_view MainTabBarName = "##MainTabs";
		public:
			MainTabs_ MainTabs; 
		};
	public:
		const std::string_view TitleBarName = "##TitleBar";
		const std::string_view MenuBarOfTitleBarName = "##MenuBarOfTitleBar";
		const std::string_view MainDebuggerInterfaceName = "##Debugger Interface";
		const std::string_view RedDebuggerName = "RedDbg";
		const std::string_view HyperDebuggerName = "HyperDbg";
	public:
		TitleBarMenu_ TitleBarMenu;
		MainDebuggerInterface_ MainDebuggerInterface;
	};
public:
	Windowses_ Windowses;
};