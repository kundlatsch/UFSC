// EPOS ARM Cortex I2C Mediator Declarations

#ifndef __cortex_i2c_h
#define __cortex_i2c_h

#include <machine/i2c.h>
#include __HEADER_MMOD(i2c)

__BEGIN_SYS

class I2C: private I2C_Engine
{
    friend Machine;

private:
    typedef I2C_Engine Engine;

public:
    using I2C_Common::MASTER;
    using I2C_Common::SLAVE;

public:
    I2C(unsigned int unit = 0, const Role & role = MASTER): I2C_Engine(unit, role) {}

    using Engine::get;
    using Engine::put;

    using Engine::read;
    using Engine::write;

    using Engine::flush;
    using Engine::ready_to_get;
    using Engine::ready_to_put;

    using Engine::int_enable;
    using Engine::int_disable;

    using Engine::reset;

private:
    using Engine::init;
};

__END_SYS

#endif
