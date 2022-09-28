// EPOS ARM Cortex SPI Mediator Declarations

#ifndef __cortex_spi_h
#define __cortex_spi_h

#include <machine/spi.h>
#include __HEADER_MMOD(spi)

__BEGIN_SYS

class SPI: private SPI_Engine
{
    friend Machine;

private:
    typedef SPI_Engine Engine;

public:
    SPI(unsigned int unit, unsigned int clock, const Protocol & protocol, const Mode & mode, unsigned int bit_rate, unsigned int data_bits)
    : Engine(unit, clock, protocol, mode, bit_rate, data_bits) {}

    using Engine::config;

    using Engine::get;
    using Engine::try_get;
    using Engine::put;
    using Engine::try_put;

    using Engine::flush;
    using Engine::ready_to_get;
    using Engine::ready_to_put;

    using Engine::int_enable;
    using Engine::int_disable;

    using Engine::power;

private:
    using Engine::init;

};

__END_SYS

#endif
