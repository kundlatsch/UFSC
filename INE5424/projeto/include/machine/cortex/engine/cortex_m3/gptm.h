// EPOS Cortex-M3 General Purpose Timer Mediator Declarations

#ifndef __cortex_m3_gptm_h
#define __cortex_m3_gptm_h

#include <architecture/cpu.h>
#define __timer_common_only__
#include <machine/timer.h>
#undef __timer_common_only__
#include <utility/convert.h>

__BEGIN_SYS

class GPTM: public Timer_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::TIMERx_BASE) GPTM".

private:
    typedef CPU::Reg32 Reg32;
    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef CPU::Reg32 Count;

    const static Hertz CLOCK = Traits<CPU>::CLOCK;

    // Registers' offsets from BASE (i.e. this)
    enum {                              // Description                                          Type    Value after reset
        GPTMCFG            = 0x00,      //                                                      rw      0x00000000
        GPTMTAMR           = 0x04,      //                                                      rw      0x00000000
        GPTMTBMR           = 0x08,      //                                                      rw      0x00000000
        GPTMCTL            = 0x0c,      //                                                      rw      0x00000000
        GPTMIMR            = 0x18,      //                                                      rw      0x00000000
        GPTMRIS            = 0x1c,      //                                                      ro      0x00000000
        GPTMMIS            = 0x20,      //                                                      ro      0x00000000
        GPTMICR            = 0x24,      //                                                      rw      0x00000000
        GPTMTAILR          = 0x28,      //                                                      rw      0xffffffff
        GPTMTBILR          = 0x2c,      //                                                      rw      0x0000ffff
        GPTMTAMATCHR       = 0x30,      //                                                      rw      0xffffffff
        GPTMTBMATCHR       = 0x34,      //                                                      rw      0x0000ffff
        GPTMTAPR           = 0x38,      //                                                      rw      0x00000000
        GPTMTBPR           = 0x3c,      //                                                      rw      0x00000000
        GPTMTAPMR          = 0x40,      //                                                      rw      0x00000000
        GPTMTBPMR          = 0x44,      //                                                      rw      0x00000000
        GPTMTAR            = 0x48,      //                                                      ro      0xffffffff
        GPTMTBR            = 0x4c,      //                                                      ro      0x0000ffff
        GPTMTAV            = 0x50,      //                                                      rw      0xffffffff
        GPTMTBV            = 0x54,      //                                                      rw      0x0000ffff
        GPTMTAPS           = 0x5c,      //                                                      ro      0x00000000
        GPTMTBPS           = 0x60,      //                                                      ro      0x00000000
        GPTMTAPV           = 0x64,      //                                                      ro      0x00000000
        GPTMTBPV           = 0x68,      //                                                      ro      0x00000000
        GPTMPP             = 0xfc0,     //                                                      ro      0x00000000
    };

    enum {                              // Description
        TAEN            = 1 << 0,       // Timer A enable
        TASTALL         = 1 << 1,       // Timer A stall enable (0 -> continues counting while the processor is halted by the debugger, 1 -> freezes)
        TAEVENT         = 1 << 2,       // Timer A event mode (0 -> positive edge, 1 -> negative edge, 2 -> reserved, 3 -> both edges)
        TAOTE           = 1 << 5,       // Timer A output ADC trigger enable
        TAPWML          = 1 << 6,       // Timer A PWM output level (0 -> direct, 1 -> inverted)
        TBEN            = 1 << 8,       // Timer B enable
        TBSTALL         = 1 << 9,       // Timer B stall enable (0 -> continues counting while the processor is halted by the debugger, 1 -> freezes)
        TBEVENT         = 1 << 10,      // Timer B event mode (0 -> positive edge, 1 -> negative edge, 2 -> reserved, 3 -> both edges)
        TBOTE           = 1 << 13,      // Timer B output ADC trigger enable
        TBPWML          = 1 << 14,      // Timer B PWM output level (0 -> direct, 1 -> inverted)
    };

    enum {                              // Description
        TMR             = 1 << 0,       // Timer A mode (0 -> reserved, 1 -> one-shot, 2 -> periodic, 3 -> capture)
        TCMR            = 1 << 2,       // Timer A capture mode (0 -> edge-count, 1 -> edge-time)
        TAMS            = 1 << 3,       // Timer A alternate mode (0 -> capture, 1 -> PWM) mode is enabled.
        TCDIR           = 1 << 4,       // Timer A count direction (0 -> descending, 1 -> ascending from 0)
        TMIE            = 1 << 5,       // Timer A match interrupt enable (GPTMTAMATCHR register is reached)
        TWOT            = 1 << 6,       // Timer A wait-on-trigger (wait for a trigger from the Timer in the previous position in the daisy-chain to start counting; must be clear for Timer A0)
        TSNAPS          = 1 << 7,       // Timer A snap-shot mode enable (in periodic mode, the actual free-running value of Timer A is loaded at the time-out event into GPTMTAR
        TILD            = 1 << 8,       // Timer A PWM interval load write (0 -> next cycle, 1 -> next time-out)
        TPWMIE          = 1 << 9,       // Timer A PWM interrupt enable (valid only in PWM mode)
        TMRSU           = 1 << 10,      // Timer A match register update mode (0 -> next cycle, 1 -> next time-out)
        TPLO            = 1 << 11,      // Legacy PWM operation (0 -> legacy operation, 1 -> CCP is set to 1 on time-out)
    };

    enum {                      // Description                    Type Reset value
        TATO_INT = 1 << 0,      // Timer A time-out interrupt       RW 0
        CAM_INT  = 1 << 1,      // Timer A capture match interrupt  RW 0
        CAE_INT  = 1 << 2,      // Timer A capture event Interrupt  RW 0
        TAM_INT  = 1 << 4,      // Timer A match interrupt          RW 0
        TBTO_INT = 1 << 8,      // Timer B time-out interrupt       RW 0
        CBM_INT  = 1 << 9,      // Timer B capture match interrupt  RW 0
        CBE_INT  = 1 << 10,     // Timer B capture event Interrupt  RW 0
        TBM_INT  = 1 << 11,     // Timer B match interrupt          RW 0
        WUE_INT  = 1 << 16,     // write update error interrupt     RW 0
    };

public:
    void config(const Count & count, bool interrupt, bool periodic) {
        disable();
        gptm(GPTMCFG)   = 0;                     // 32-bit timer
        gptm(GPTMTAMR)  = TCDIR | ((periodic) ? 2 : 1);      // 2 -> Periodic, 1 -> One-shot
        gptm(GPTMTAILR) = count;
        gptm(GPTMTAPR)  = (count >> 31) >> 1;    // Avoid compiler warning "shift >= width of type"
        if(interrupt)
            gptm(GPTMIMR) |= TATO_INT;
        else
            gptm(GPTMIMR) &= ~TATO_INT;
        enable();
    }

    Count count() { return gptm(GPTMTAR); } // LM3S811 on QEMU (v2.7.50) does not support reading the value of general purpose timers

    void enable()  { gptm(GPTMCTL) |= TAEN; }
    void disable() { gptm(GPTMCTL) &= ~TAEN; }

    void delay(const Count & offset) {
        gptm(GPTMTAILR) = offset;
        gptm(GPTMTAPR) = 0;
        gptm(GPTMCTL) |= TAEN;
        while(!(gptm(GPTMRIS) & TATO_INT));
        gptm(GPTMCTL) &= ~TAEN;
        gptm(GPTMICR) |= TATO_INT;
    }

    Count read() { return count(); }

    void pwm(const Percent & duty_cycle) {
        disable();

        Count count;
        if(gptm(GPTMCFG) == 4) {
            count = gptm(GPTMTAILR) + (gptm(GPTMTAPR) << 16);
        } else {
            count = gptm(GPTMTAILR);
            gptm(GPTMCFG) = 4; // 4 -> 16-bit, only possible value for PWM
            gptm(GPTMTAPR) = count >> 16;
        }

        gptm(GPTMTAMR) = TCMR | TAMS | 2; // 2 -> Periodic, 1 -> One-shot
        gptm(GPTMCTL) &= ~TBPWML; // never inverted

        count = Convert::percent2count(duty_cycle, count & 0x00ffffff);
        gptm(GPTMTAPMR) = count >> 16;
        gptm(GPTMTAMATCHR) = count;
        enable();
    }

    Hertz clock() { return CLOCK; }

    void eoi(IC::Interrupt_Id id) { gptm(GPTMICR) = -1; }

private:
    volatile Reg32 & gptm (unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
