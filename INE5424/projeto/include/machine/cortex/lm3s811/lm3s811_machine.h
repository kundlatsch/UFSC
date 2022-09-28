// EPOS LM3S811 (ARM Cortex-M3) Mediator Declarations

#ifndef __lm3s811_machine_h
#define __lm3s811_machine_h

#include <machine/machine.h>
#include <machine/cortex/engine/cortex_m3/scb.h>
#include <machine/cortex/engine/cortex_m3/systick.h>
#include "lm3s811_sysctrl.h"
#include <system/memory_map.h>
#include <system.h>

__BEGIN_SYS

class LM3S811: private Machine_Common
{
    friend Machine;

private:
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

public:
    LM3S811() {}

    using Machine_Common::delay;
    using Machine_Common::clear_bss;

    static void reboot() { scb()->reboot(); }
    static void poweroff() { reboot(); }

    static const UUID & uuid() { return System::info()->bm.uuid; } // TODO: System_Info is not populated in this machine

    static void smp_barrier() {}

    static void smp_barrier_init(unsigned int n_cpus) { assert(n_cpus == 1); }

    static void power(const Power_Mode & mode) {}
    
private:
    static void pre_init();
    static void init();

private:
    static SCB * scb() { return reinterpret_cast<SCB *>(Memory_Map::SCB_BASE); }
    static SysCtrl * scr() { return reinterpret_cast<SysCtrl *>(Memory_Map::SCR_BASE); }
};

typedef LM3S811 Machine_Model;

__END_SYS

#endif
