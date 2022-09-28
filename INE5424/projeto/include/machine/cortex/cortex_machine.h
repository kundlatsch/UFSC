// EPOS ARM Cortex Mediator Declarations

#ifndef __cortex_machine_h
#define __cortex_machine_h

#include <architecture.h>
#include <machine/machine.h>
#include <machine/ic.h>
#include <system/info.h>
#include <system/memory_map.h>
#include <system.h>
#include __HEADER_MMOD(machine)

__BEGIN_SYS

class Machine: private Machine_Model
{
    friend class Init_System;
    friend class Init_Begin;

private:
    static const bool smp = Traits<System>::multicore;

    typedef Machine_Model Engine;

public:
    Machine() {}

    using Engine::delay;
    using Engine::clear_bss;

    static void panic();

    static void reboot() {
        db<Machine>(WRN) << "Machine::reboot()" << endl;
        Engine::reboot();
    }
    static void poweroff() {
        db<Machine>(WRN) << "Machine::poweroff()" << endl;
        Engine::poweroff();
    }

    static const UUID & uuid() { return System::info()->bm.uuid; }

private:
    static void pre_init(System_Info * si);
    static void init();
};

__END_SYS

#endif
