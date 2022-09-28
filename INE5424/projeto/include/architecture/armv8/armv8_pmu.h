// EPOS ARMv8 PMU Mediator Declarations

#ifndef __armv8_pmu_h
#define __armv8_pmu_h

#include <architecture/cpu.h>
#define __common_only__
#include <architecture/pmu.h>
#undef __common_only__

__BEGIN_SYS

class ARMv8_A_PMU;

class PMU
{
    friend class CPU;

private:
    typedef ARMv8_A_PMU Engine;

public:
    PMU() {}

private:
    static void init() {}
};

__END_SYS

#endif
