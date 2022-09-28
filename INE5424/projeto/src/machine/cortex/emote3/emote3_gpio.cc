// EPOS eMote3 (ARM Cortex-M3) GPIO Mediator Initialization

#include <system/config.h>

#ifdef __GPIO_H

#include <machine/gpio.h>

__BEGIN_SYS

void GPIO_Engine::int_enable(Edge edge, bool power_up, Edge power_up_edge)
{
    _gpio->int_enable(_port, _pin_mask, edge, power_up, power_up_edge);
}

__END_SYS

#endif
