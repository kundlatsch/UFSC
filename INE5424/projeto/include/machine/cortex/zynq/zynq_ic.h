// EPOS Zynq-7000 (Cortex-A9) IC Mediator Declarations

#ifndef __zynq_ic_h
#define __zynq_ic_h

#include <machine/ic.h>
#include <machine/cortex/engine/cortex_a9/gic.h>
#include <system/memory_map.h>

__BEGIN_SYS

class IC_Engine: public IC_Common
{
public:
    // Interrupts
    static const unsigned int INTS = Traits<IC>::INTS;
    enum {
        INT_SYS_TIMER   = GIC::IRQ_PRIVATE_TIMER,
        INT_USER_TIMER0 = GIC::IRQ_GLOBAL_TIMER,
        INT_USER_TIMER1 = 0,
        INT_USER_TIMER2 = 0,
        INT_USER_TIMER3 = 0,
        INT_GPIOA       = GIC::IRQ_GPIO,
        INT_GPIOB       = GIC::IRQ_GPIO,
        INT_GPIOC       = GIC::IRQ_GPIO,
        INT_GPIOD       = GIC::IRQ_GPIO,
        INT_NIC0_RX     = GIC::IRQ_ETHERNET0,
        INT_NIC0_TX     = GIC::IRQ_ETHERNET0,
        INT_NIC0_ERR    = GIC::IRQ_ETHERNET0,
        INT_NIC0_TIMER  = 0,
        INT_USB0        = GIC::IRQ_USB0,
        INT_FIRST_HARD  = GIC::HARD_INT,
        INT_LAST_HARD   = GIC::IRQ_PARITY,
        INT_RESCHEDULER = GIC::IRQ_SOFTWARE0,
        LAST_INT        = INT_RESCHEDULER
    };

    // Exceptions
    // Exceptions are hard configured by SETUP, since they run in different mode with a different context layout.
    static const unsigned int EXC_INT = 0;

public:
    static void enable() { gic_distributor()->enable(); }
    static void enable(Interrupt_Id id)  { gic_distributor()->enable(id); }
    static void disable() { gic_distributor()->disable(); }
    static void disable(Interrupt_Id id) { gic_distributor()->disable(); }

    static Interrupt_Id int_id() { return gic_cpu()->int_id(); }
    static Interrupt_Id irq2int(Interrupt_Id id) { return gic_distributor()->irq2int(id); }
    static Interrupt_Id int2irq(Interrupt_Id irq) { return gic_distributor()->int2irq(irq); }

    static void ipi(unsigned int cpu, Interrupt_Id id) { gic_distributor()->send_sgi(cpu, id); }

    static void init() {
        gic_distributor()->init();
        gic_cpu()->init();
    };

private:
    static GIC_CPU * gic_cpu() { return reinterpret_cast<GIC_CPU *>(Memory_Map::GIC_CPU_BASE); }
    static GIC_Distributor * gic_distributor() { return reinterpret_cast<GIC_Distributor *>(Memory_Map::GIC_DIST_BASE); }
};

__END_SYS

#endif
