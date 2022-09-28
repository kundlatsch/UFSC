// EPOS ARM Cortex Mediator Implementation

#include <machine/machine.h>
#include <machine/display.h>

__BEGIN_SYS

void Machine::panic()
{
    CPU::int_disable();
    if(Traits<Display>::enabled)
        Display::puts("PANIC!\n");
    if(Traits<System>::reboot)
        reboot();
    else
        CPU::halt();
}

__END_SYS
