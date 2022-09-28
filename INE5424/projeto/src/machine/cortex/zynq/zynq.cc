// EPOS Zynq (ARM Cortex-A9) Mediator Implementation

#include <machine/machine.h>
#include <machine/display.h>

__BEGIN_SYS

volatile unsigned int Zynq::_cores;

void Zynq::reboot()
{
    db<Machine>(WRN) << "Machine::reboot()" << endl;

    // This will mess with qemu but works on real hardware, possibly a bug
    // in qemu. Note that the asserting reset will clear the RAM where the
    // application is stored.
    slcr(PSS_RST_CTRL) = 1;
}

__END_SYS
