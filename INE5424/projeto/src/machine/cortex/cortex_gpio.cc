// EPOS Cortex-M GPIO Mediator Implementation

#include <machine/ic.h>
#include <machine/gpio.h>

#ifdef __GPIO_H

__BEGIN_SYS

GPIO * GPIO::_gpios[PORTS][8];

void GPIO::int_handler(IC::Interrupt_Id id)
{
    // GPIO interrupt handling is done at eoi, because some devices generate interrupts faster than the processor can handle
}

void GPIO::eoi(IC::Interrupt_Id id)
{
    unsigned int port = id - IC::INT_GPIOA;
    PL061 * pl061 = new(reinterpret_cast<void *>(Memory_Map::GPIOA_BASE + port * 0x1000)) PL061;
    unsigned int mis = pl061->pending_regular_interrupts();
    unsigned int irq_detect_ack = pl061->pending_powerup_interrupts();

    for(unsigned int i = 0; i < 8; ++i) {
        bool regular_interrupt = mis & (1 << i);
        bool power_up_interrupt = irq_detect_ack & ((1 << i) << (8 * port));
        if(regular_interrupt || power_up_interrupt)
            _gpios[port][i]->notify();
    }

    // Clear regular interrupts even if no handler is available
    pl061->clear_interrupts(port, 0xff);
}

__END_SYS

#endif
