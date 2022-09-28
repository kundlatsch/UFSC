// EPOS Cortex-A9 Global Timer Mediator Declarations

#ifndef __cortex_a9_global_timer_h
#define __cortex_a9_global_timer_h

#include <architecture/cpu.h>
#define __timer_common_only__
#include <machine/timer.h>
#undef __timer_common_only__


__BEGIN_SYS

class A9_Global_Timer:  public Timer_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::PRIVATE_TIMER_BASE) A9_Private_Timer".

private:
    typedef CPU::Reg32 Reg32;
    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef CPU::Reg64 Count;

    static const Hertz CLOCK = Traits<CPU>::CLOCK / 2;

    // Registers' offsets from BASE
    enum {                                      // Description
        GTCTRL                      = 0x00,     // Low Counter
        GTCTRH                      = 0x04,     // High Counter
        GTCLR                       = 0x08,     // Control
        GTISR                       = 0x0C      // Interrupt Status
    };

public:
    void config(const Count & count, bool interrupt, bool periodic);
    Count count() {
        CPU::Reg32 high, low;

        do {
            high = gt(GTCTRH);
            low = gt(GTCTRL);
        } while(gt(GTCTRH) != high);

        return static_cast<Count>(high) << 32 | low;
    }

    void enable();
    void disable();

    void set(const Count & count) {
        // Disable counting before programming
        gt(GTCLR) = 0;

        gt(GTCTRL) = count & 0xffffffff;
        gt(GTCTRH) = count >> 32;

        // Re-enable counting
        gt(GTCLR) = 1;
    }

    Hertz clock() { return CLOCK; }

    void eoi(IC::Interrupt_Id int_id) {}

protected:
    volatile Reg32 & gt(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
