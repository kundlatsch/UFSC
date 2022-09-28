// EPOS Watchdog Mediator Common Package

#ifndef __watchdog_h
#define __watchdog_h

#include <utility/handler.h>
#include <machine/machine.h>

__BEGIN_SYS

class Watchdog_Common
{
protected:
    Watchdog_Common() {}

public:
    void kick();
    void enable();
    void disable();
};

__END_SYS

#endif

#if defined(__WATCHDOG_H) && !defined(__watchdog_common_only__)
#include __WATCHDOG_H
#endif
