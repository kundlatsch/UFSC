// EPOS Zynq (ARM Cortex-A9) Initialization

#include <machine.h>

#ifdef __mmod_zynq__

__BEGIN_SYS

void Zynq::init()
{
    unlock_slcr();
    fpga_reset();
}

__END_SYS

#endif
