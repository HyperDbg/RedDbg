#pragma once
#include "Debugger/Driver/Common.hpp"
#include "TransparentMode/TransparentMode.hpp"
#include <sal.h>

void CpuIDHandler(_Inout_ VMMData_& VMMData, SVM::VMM_STATUS& Status);
void MsrHandler(_Inout_ VMMData_& VMMData);