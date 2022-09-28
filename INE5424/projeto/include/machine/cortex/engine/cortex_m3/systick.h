// EPOS Cortex-M SysTick Timer Mediator Declarations

#ifndef __cortex_systick_timer_h
#define __cortex_systick_timer_h

#include <architecture/cpu.h>
#define __timer_common_only__
#include <machine/timer.h>
#undef __timer_common_only__

__BEGIN_SYS

class SysTick: public Timer_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::SCB_BASE) SysTick".

private:
    typedef CPU::Reg32 Reg32;
    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef Reg32 Count;

    static const Hertz CLOCK = Traits<CPU>::CLOCK;

    // Registers' offsets in System Control Space
    enum {                              // Description                                          Type    Value after reset
        STCTRL          = 0x010,        // SysTick Control and Status Register                  rw      0x00000004
        STRELOAD        = 0x014,        // SysTick Reload Value Register                        rw      0x00000000
        STCURRENT       = 0x018,        // SysTick Current Value Register                       rwc     0x00000000
    };

    // Useful Bits in the SysTick Control and Status Register (STCTRL)
    enum STCTRL {                       // Description                                          Type    Value after reset
        ENABLE          = 1 << 0,       // Enable / disable                                     rw      0x00000000
        INTEN           = 1 << 1,       // Interrupt pending                                    rw      0x00000000
        CLKSRC          = 1 << 2,       // Clock source (0 -> external, 1 -> core)              rw      0x00000000
        COUNT           = 1 << 16       // Count underflow                                      ro      0x00000000
    };

public:
    void config(const Count & count, bool interrupt, bool periodic) {
        assert(periodic); // SysTick is always auto-retrigger
        scs(STCTRL) = 0;
        scs(STCURRENT) = 0;
        scs(STRELOAD) = count;
        scs(STCTRL) = CLKSRC | (interrupt ? INTEN : 0);
    }

    Count count() { return scs(STCURRENT); }

    void enable() { scs(STCTRL) |= ENABLE; }
    void disable() { scs(STCTRL) &= ~ENABLE; }

    void delay(const Count & offset) {
        // TODO: consider overflow!
        Count initial = count();
        Count target = initial + offset;
        while(count() < target);
    }

    Hertz clock() { return CLOCK; }

    void eoi(Interrupt_Id id) {};

private:
    volatile Reg32 & scs(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
