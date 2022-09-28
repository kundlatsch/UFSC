// EPOS Cortex-A9 Private Timer Mediator Declarations

#ifndef __cortex_a9_private_timer_h
#define __cortex_a9_private_timer_h

#include <architecture/cpu.h>
#define __timer_common_only__
#include <machine/timer.h>
#undef __timer_common_only__

__BEGIN_SYS

class A9_Private_Timer: public Timer_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::PRIVATE_TIMER_BASE) A9_Private_Timer".

private:
    typedef CPU::Reg32 Reg32;
    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef CPU::Reg32 Count;

    static const Hertz CLOCK = Traits<CPU>::CLOCK / 2;

    // Registers' offsets from BASE
    enum {                                      // Description
        PTLR                        = 0x00,     // Load
        PTCTR                       = 0x04,     // Counter
        PTCLR                       = 0x08,     // Control
        PTISR                       = 0x0C      // Interrupt Status
    };

    // Useful bits in PTCLR
    enum {                                      // Description                  Type    Value after reset
        TIMER_ENABLE                = 1 << 0,   // Enable                       r/w     0
        AUTO_RELOAD                 = 1 << 1,   // Auto reload                  r/w     0
        IRQ_EN                      = 1 << 2    // Enable interrupt             r/w     0
    };

    // Useful bits in PTISR
    enum {                                      // Description                  Type    Value after reset
        INT_CLR                     = 1 << 0    // Interrupt clear bit          r/w     0
    };

public:
    void config(const Count & count) {
        // TODO: interrupt and periodic are not being handled!
        pt(PTCLR) = 0;
        pt(PTISR) = INT_CLR;
        pt(PTLR) = count;
        pt(PTCLR) = IRQ_EN | AUTO_RELOAD;
    }

    Count count() { return pt(PTCTR); }

    void enable() { pt(PTCLR) |= TIMER_ENABLE; }
    void disable() { pt(PTCLR) &= ~TIMER_ENABLE; }

    void delay(const Count & offset) {
        // TODO: consider overflow!
        Count initial = count();
        Count target = initial + offset;
        while(count() < target);
    }

    Hertz clock() { return CLOCK; }

    void eoi(IC::Interrupt_Id int_id) { pt(PTISR) = INT_CLR; }

    void power(const Power_Mode & mode);

private:
    volatile Reg32 & pt(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
