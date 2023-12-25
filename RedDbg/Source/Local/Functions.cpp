#include "Local/Functions.hpp"

UnexportedFunctions_::UnexportedFunctions_()
{
	for (unsigned char Index = 0; Index < Names.size(); ++Index)
	{
		UNICODE_STRING Temp;
		PVOID TempFunc;
		RtlInitUnicodeString(&Temp, Names[Index].c_str());

		TempFunc = MmGetSystemRoutineAddress(&Temp);
		switch (Index)
		{
		case FunctionIndexes::PsGetProcessPebIndex:
			PsGetProcessPeb = reinterpret_cast<PsGetProcessPeb_>(TempFunc); break;
		case  FunctionIndexes::PsGetProcessWow64ProcessIndex:
			PsGetProcessWow64Process = reinterpret_cast<PsGetProcessWow64Process_>(TempFunc); break;
		}
	}

	return;
}