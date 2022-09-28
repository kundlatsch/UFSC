// EPOS LM3S811 (ARM Cortex-M3) MCU Initialization

#include <system/config.h>
#include <machine/machine.h>

__BEGIN_SYS

void LM3S811::pre_init()
{
    db<Init, Machine>(TRC) << "LM3S811::pre_init()" << endl;

    scr()->clock_init();
}

void LM3S811::init()
{
    db<Init, Machine>(TRC) << "LM3S811::init()" << endl;
    scb()->enter_thread_state();
}

__END_SYS
