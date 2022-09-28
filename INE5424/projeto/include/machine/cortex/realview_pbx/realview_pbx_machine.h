// EPOS Realview PBX (ARM Cortex-A9) Mediator Declarations

#ifndef __realview_pbx_machine_h
#define __realview_pbx_machine_h

#include <machine/machine.h>
#include <machine/cortex/engine/cortex_a9/gic.h>
#include <machine/cortex/engine/cortex_a9/scu.h>
#include <system/memory_map.h>
#include <system.h>

__BEGIN_SYS

class Realview_PBX: private Machine_Common
{
    friend Machine; // for pre_init() and init()

private:
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

public:
    Realview_PBX() {}

    using Machine_Common::delay;
    using Machine_Common::clear_bss;

    static void reboot();
    static void poweroff() { reboot(); }

    static const UUID & uuid() { return System::info()->bm.uuid; }

public:
    static void smp_barrier_init(unsigned int n_cpus) { _cores = n_cpus; }

private:
    static void pre_init() {}
    static void init() {}

private:
    static GIC_CPU * gic_cpu() { return reinterpret_cast<GIC_CPU *>(Memory_Map::GIC_CPU_BASE); }
    static GIC_Distributor * gic_distributor() { return reinterpret_cast<GIC_Distributor *>(Memory_Map::GIC_DIST_BASE); }
    static SCU * scu() { return reinterpret_cast<SCU *>(Memory_Map::SCU_BASE); }

private:
    static volatile unsigned int _cores;
};

typedef Realview_PBX Machine_Model;

__END_SYS

#endif
