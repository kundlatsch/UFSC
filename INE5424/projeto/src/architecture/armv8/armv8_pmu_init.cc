// EPOS ARMv8 PMU Mediator Initialization
/*
#include <architecture/pmu.h>

__BEGIN_SYS

void ARMv8_A_PMU::init()
{
    db<Init, PMU>(TRC) << "PMU::init()" << endl;

    // Set global enable, reset all counters including cycle counter, and
    // export the events
    pmcr(pmcr() | PMCR_E | PMCR_P | PMCR_C | PMCR_X);

    // Clear all overflows:
    pmovsr(~0);

    // Enable cycle counter
    pmcntenset(pmcntenset() | PMCNTENSET_C);
}

__END_SYS
*/