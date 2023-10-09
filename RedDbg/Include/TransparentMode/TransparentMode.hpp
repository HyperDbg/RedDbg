#pragma once 
#include "HyperVisor/CommonTypes/CPUID.hpp"

class Transparent {
public:
	void InvisibleCpuid(CPUID::FEATURE_INFORMATION Regs);
	void InvisibleRdtscARdtscp();
};