/*!
    @file HookVmmCommon.hpp

    @brief VMM code to support hooking.

    @author Satoshi Tanda

    @copyright  Copyright (c) 2018, Satoshi Tanda. All rights reserved.
 */
#pragma once
#include "HookCommon.hpp"
#include "HyperVisor/CommonTypes/SVM.hpp"

VOID
HandleNestedPageFault (
    _Inout_ SVM::VMCB* GuestVmcb,
    _Inout_ PHOOK_DATA HookData
    );

VOID
HandleBreakPointException (
    _Inout_ SVM::VMCB* GuestVmcb,
    _Inout_ PHOOK_DATA HookData
    );

VOID
EnableHooks (
    _Inout_ PHOOK_DATA HookData
    );

VOID
DisableHooks (
    _Inout_ PHOOK_DATA HookData
    );
