// EPOS eMote3 (ARM Cortex-M3) Mediator Initialization

#include <machine/machine.h>

#ifdef __mmod_emote3__

#include <machine/ic.h>
#include <machine/gpio.h>
#include <synchronizer.h>

__BEGIN_SYS

void eMote3::pre_init()
{
    // Set pins PD6 and PD7 to enable external oscillator
    Reg32 pin_mask = (1 << 6) | (1 << 7);
    PL061 * pl061 = new(reinterpret_cast<void *>(Memory_Map::GPIOD_BASE)) PL061;
    pl061->select_pin_function(PL061::PIN6 | PL061::PIN7, PL061::FUN_ALTERNATE);
    pl061->direction(pin_mask, PL061::OUT);
    ioc()->enable_external_clock();
    scr()->config_clock();
}

void eMote3::init()
{
    db<Init, Machine>(TRC) << "eMote3::init()" << endl;

    scb()->enter_thread_state(); // The processor can enter thread mode from any level

    scr()->enable_alternate_interrupt_mapping();

    scb()->vector_table_offset(); // Set the vector table offset (must be 512-byte aligned)
}

__END_SYS

#endif
