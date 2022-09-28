// EPOS PC Mediator Declarations

#ifndef __pc_machine_h
#define __pc_machine_h

#include <architecture.h>
#include <machine/machine.h>
#include <machine/ic.h>
#include <machine/rtc.h>
#include <machine/display.h>
#include <system/info.h>
#include <system/memory_map.h>
#include <system.h>

__BEGIN_SYS

class Machine: private Machine_Common
{
    friend class Init_System;
    friend class Init_Begin;

private:
    static const bool smp = Traits<System>::multicore;

    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

public:
    Machine() {}

    using Machine_Common::delay;
    using Machine_Common::clear_bss;

    static void panic();
    static void reboot();
    static void poweroff();

    static const UUID & uuid() { return System::info()->bm.uuid; }

private:
    static void pre_init(System_Info * si);
    static void init();
};

__END_SYS

#endif
