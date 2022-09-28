// EPOS Cortex Watchdog Mediator Declarations

#ifndef __cortex_watchdog_h
#define __cortex_watchdog_h

#include <architecture/cpu.h>
#include <machine/watchdog.h>
#include __HEADER_MMOD(watchdog)

__BEGIN_SYS

class Watchdog: private Watchdog_Engine
{
    friend Machine;

 private:
     typedef Watchdog_Engine Engine;
     typedef CPU::Reg32 Reg32;

public:
     Watchdog() {}

     using Engine::kick;

     using Engine::enable;
     using Engine::disable;

private:
    using Engine::init;
};

__END_SYS

#endif
