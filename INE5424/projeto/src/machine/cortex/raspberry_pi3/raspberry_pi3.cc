// EPOS Raspberry Pi3 (ARM Cortex-A53) Mediator Implementation

#include <machine/machine.h>
#include <machine/display.h>
#include <system/memory_map.h>

__BEGIN_SYS

volatile unsigned int Raspberry_Pi3::_cores;

void Raspberry_Pi3::reboot()
{
    db<Machine>(WRN) << "Machine::reboot()" << endl;

    Reg32 * PM_RSTC = reinterpret_cast<Reg32 *>(Memory_Map::PM_BASE + 0x1c);
    Reg32 * PM_WDOG = reinterpret_cast<Reg32 *>(Memory_Map::PM_BASE + 0x24);
    const int PM_PASSWORD = 0x5a000000;
    const int PM_RSTC_WRCFG_FULL_RESET = 0x00000020;
    // Watch Dog will reboot if it could not be runned, thus the while true
    *(PM_WDOG) = PM_PASSWORD | 1;
    *(PM_RSTC) = PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET;

    while(true);
}

__END_SYS
