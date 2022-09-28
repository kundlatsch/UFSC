// EPOS EPOSMoteIII (ARM Cortex-M3) Watchdog Mediator Declarations

#ifndef __emote3_watchdog_h
#define __emote3_watchdog_h

#include <architecture/cpu.h>
#include <machine/watchdog.h>

__BEGIN_SYS

class Watchdog_Engine: public Watchdog_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::WATCHDOG_BASE) CC2538_Watchdog"

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers offsets from BASE (i.e. this)
    enum {
     // Name    Offset   Type  Width   Reset Value    Physical Address
        WDCTL = 0x00, //   RW   32     0x0000 0000    0x400d5000
    };

    // WDCTL useful bits
    enum {
     // Name  Offset     Description                                                           Type Reset Value
        CLR = 1 << 4, // Clear timer                                                             RW 0
                      // When 0xA followed by 0x5 is written to these bits, the timer is
                      // loaded with 0x0000. Note that 0x5 must be written within one
                      // watchdog clock period Twdt after 0xA was written for the clearing to
                      // take effect (ensured).
                      // If 0x5 is written between Twdt and 2Twdt after 0xA was written, the
                      // clearing may take effect, but there is no guarantee. If 0x5 is written
                      // > 2Twdt after 0xA was written, the timer will not be cleared.
                      // If a value other than 0x5 is written after 0xA has been written, the
                      // clear sequence is aborted. If 0xA is written, this starts a new clear
                      // sequence.
                      // Writing to these bits when EN = 0 has no effect.
        EN  = 1 << 3, // Enable timer                                                            RW 0
                      // When 1 is written to this bit the timer is enabled and starts
                      // incrementing. The interval setting specified by INT[1:0] is used.
                      // Writing 0 to this bit have no effect.
        INT = 1 << 0, // Timer interval select                                                   RW 0
                      // These bits select the timer interval as follows:
                      // 00: Twdt x 32768
                      // 01: Twdt x 8192
                      // 10: Twdt x 512
                      // 11: Twdt x 64
                      // Writing these bits when EN = 1 has no effect.
    };

    enum {
        MS_1_9    = 3, // 1.9ms
        MS_15_625 = 2, // 15.625ms
        S_0_25    = 1, // 0.25s
        S_1       = 0, // 1s
    };

public:
    void kick() {
        wd(WDCTL) = 0xa * CLR;
        wd(WDCTL) = 0x5 * CLR;
    }

    void enable() {
        if(Traits<Watchdog>::PERIOD <= 10) wd(WDCTL) = MS_1_9;
        else if(Traits<Watchdog>::PERIOD <= 50) wd(WDCTL) = MS_15_625;
        else if(Traits<Watchdog>::PERIOD <= 500) wd(WDCTL) = S_0_25;
        else wd(WDCTL) = S_1;

        wd(WDCTL) |= EN;
    }

    void disable() { wd(WDCTL) &= ~EN; }

    static void init() {}

private:
    volatile Reg32 & wd(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
