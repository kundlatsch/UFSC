// EPOS EPOSMoteIII (ARM Cortex-M3) System Control Module Mediator Declarations

#ifndef __emote3_sysctrl_h
#define __emote3_sysctrl_h

#include <architecture/cpu.h>

__BEGIN_SYS

class SysCtrl
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::SCR_BASE) SysCtrl".

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers' offsets in SysCtrl (not to be confused with System Control Space)
    enum {                              // Description                                          Type    Value after reset
        CLOCK_CTRL      = 0x00,
        CLOCK_STA       = 0x04,
        RCGCGPT         = 0x08,
        SCGCGPT         = 0x0c,
        DCGCGPT         = 0x10,
        SRGPT           = 0x14,
        RCGCSSI         = 0x18,
        SCGCSSI         = 0x1c,
        DCGCSSI         = 0x20,
        SRSSI           = 0x24,
        RCGCUART        = 0x28,
        SCGCUART        = 0x2c,
        DCGCUART        = 0x30,
        SRUART          = 0x34,
        RCGCI2C         = 0x38,
        SCGCI2C         = 0x3c,
        DCGCI2C         = 0x40,
        SRI2C           = 0x44,
        RCGCSEC         = 0x48,
        SCGCSEC         = 0x4c,
        DCGCSEC         = 0x50,
        SRSEC           = 0x54,
        PMCTL           = 0x58,
        SRCRC           = 0x5c,
        PWRDBG          = 0x74,
        CLD             = 0x80,
        IWE             = 0x94,
        I_MAP           = 0x98,
        RCGCRFC         = 0xa8,
        SCGCRFC         = 0xac,
        DCGCRFC         = 0xb0,
        EMUOVR          = 0xb4
    };

    // Useful Bits in the Clock Control Register (CLOCK_CTRL)
    enum {                        // Description                                      Type    Value after reset
        OSC32K_CALDIS = 1 << 25,  // Disable calibration 32-kHz RC oscillator.        rw      0
                                  // 0: Enable calibration
                                  // 1: Disable calibration
        OSC32K        = 1 << 24,  // 32-kHz clock oscillator selection                rw      1
                                  // 0: 32-kHz crystal oscillator
                                  // 1: 32-kHz RC oscillator
        AMP_DET       = 1 << 21,  // Amplitude detector of XOSC during power up       rw      0
                                  // 0: No action
                                  // 1: Delay qualification of XOSC until amplitude
                                  //    is greater than the threshold.
        OSC_PD        = 1 << 17,  // 0: Power up both oscillators
                                  // 1: Power down oscillator not selected by
                                  //    OSC bit (hardware-controlled when selected).
        OSC           = 1 << 16,  // System clock oscillator selection                rw      1
                                  // 0: 32-MHz crystal oscillator
                                  // 1: 16-MHz HF-RC oscillator
        IO_DIV        = 1 <<  8,  // I/O clock rate setting                           rw      1
                                  // Cannot be higher than OSC setting
                                  // 000: 32 MHz
                                  // 001: 16 MHz
                                  // 010: 8 MHz
                                  // 011: 4 MHz
                                  // 100: 2 MHz
                                  // 101: 1 MHz
                                  // 110: 0.5 MHz
                                  // 111: 0.25 MHz
        SYS_DIV       = 1 <<  0,  // System clock rate setting                        rw      1
                                  // Cannot be higher than OSC setting
                                  // 000: 32 MHz
                                  // 001: 16 MHz
                                  // 010: 8 MHz
                                  // 011: 4 MHz
                                  // 100: 2 MHz
                                  // 101: 1 MHz
                                  // 110: 0.5 MHz
                                  // 111: 0.25 MHz
    };

    // Useful Bits in the Clock Status Register (CLOCK_STA)
    enum {                           // Description                                      Type    Value after reset
        STA_SYNC_32K      = 1<<26,   // 32-kHz clock source synced to undivided          ro      0
                                     // system clock (16 or 32 MHz).
        STA_OSC32K_CALDIS = 1<<25,   // Disable calibration 32-kHz RC oscillator.        ro      0
        STA_OSC32K        = 1<<24,   // Current 32-kHz clock oscillator selected.        ro      1
                                     // 0: 32-kHz crystal oscillator
                                     // 1: 32-kHz RC oscillator
        STA_RST           = 1<<22,   // Returns last source of reset                     ro      0
                                     // 00: POR
                                     // 01: External reset
                                     // 10: WDT
                                     // 11: CLD or software reset
        STA_SOURCE_CHANGE = 1<<20,   // 0: System clock is not requested to change.      ro      0
                                     // 1: A change of system clock source has been
                                     //    initiated and is not finished. Same as when
                                     //    OSC bit in CLOCK_STA and CLOCK_CTRL register
                                     //    are not equal
        STA_XOSC_STB      = 1<<19,   // whether crystal oscillator (XOSC) is stable      ro      0
        STA_HSOSC_STB     = 1<<18,   // whether HSOSC is stable                          ro      0
        STA_OSC_PD        = 1<<17,   // 0: Both oscillators powered up and stable and    ro      0
                                     //    OSC_PD_CMD = 0.
                                     // 1: Oscillator not selected by CLOCK_CTRL. OSC
                                     //    bit is powered down.
        STA_OSC           = 1<<16,   // Current clock source selected                    ro      1
                                     // 0: 32-MHz crystal oscillator
                                     // 1: 16-MHz HF-RC oscillator
        STA_IO_DIV        = 1<<8,    // Returns current functional frequency for IO_CLK  ro      1
                                     // (may differ from setting in the CLOCK_CTRL register)
                                     // 000: 32 MHz
                                     // 001: 16 MHz
                                     // 010: 8 MHz
                                     // 011: 4 MHz
                                     // 100: 2 MHz
                                     // 101: 1 MHz
                                     // 110: 0.5 MHz
                                     // 111: 0.25 MHz
        STA_SYS_DIV       = 1<<0,    // Returns current functional frequency for         ro     1
                                     // system clock
                                     // (may differ from setting in the CLOCK_CTRL register)
                                     // 000: 32 MHz
                                     // 001: 16 MHz
                                     // 010: 8 MHz
                                     // 011: 4 MHz
                                     // 100: 2 MHz
                                     // 101: 1 MHz
                                     // 110: 0.5 MHz
                                     // 111: 0.25 MHz
    };

    // Useful Bits in the Interrupt Map Register (I_MAP)
    enum I_MAP {
        I_MAP_ALTMAP = 1 << 0,
    };

    enum Power_Mode {
        ACTIVE,
        PMSLEEP,
        POWER_MODE_0,
        POWER_MODE_1,
        POWER_MODE_2,
        POWER_MODE_3,
    };

    enum Wakeup_Event {
        EVENT_SLEEP_MODE_TIMER  = 1 << 5,
        EVENT_USB               = 1 << 4,
        EVENT_GPIO_D            = 1 << 3,
        EVENT_GPIO_C            = 1 << 2,
        EVENT_GPIO_B            = 1 << 1,
        EVENT_GPIO_A            = 1 << 0,
    };

public:
    void config_clock() {
        Reg32 sys_div;
        switch(Traits<CPU>::CLOCK) {
            default:
            case 32000000: sys_div = 0; break;
            case 16000000: sys_div = 1; break;
            case  8000000: sys_div = 2; break;
            case  4000000: sys_div = 3; break;
            case  2000000: sys_div = 4; break;
            case  1000000: sys_div = 5; break;
            case   500000: sys_div = 6; break;
            case   250000: sys_div = 7; break;
        }

        Reg32 clock_ctrl = scr(CLOCK_CTRL) & ~(SYS_DIV * 7);
        clock_ctrl |= sys_div * SYS_DIV; // Set system clock rate
        clock_ctrl |= AMP_DET; // Enable AMP detect to make sure XOSC starts correctly
        clock_ctrl |= OSC_PD; // Power down unused oscillator
        clock_ctrl &= ~OSC; // Select 32Mhz oscillator
        clock_ctrl &= ~OSC32K; // Select 32Khz crystal oscillator

        scr(CLOCK_CTRL) = clock_ctrl; // Write back to register

        // Wait until oscillators stabilize
        while((scr(CLOCK_STA) & (STA_OSC | STA_OSC32K)));

        clock_ctrl = scr(CLOCK_CTRL) & ~(IO_DIV * 7);
        scr(CLOCK_CTRL) = clock_ctrl | (sys_div * IO_DIV); // Set IO clock rate

        while(!(scr(CLOCK_STA) & (STA_SYNC_32K)));
    }

    void clock_uart(unsigned int unit) {
        // Enable the UART module using the SYS_CTRL_RCGCUART register
        scr(RCGCUART) |= 1 << unit; // Enable clock for UART while in Running mode
        scr(SCGCUART) |= 1 << unit; // Enable clock for UART while in Sleep mode
        scr(DCGCUART) |= 1 << unit; // Enable clock for UART while in Deep Sleep mode
    }

    void unclock_uart(unsigned int unit) {
        // Disable the UART module using the SYS_CTRL_RCGCUART register
        scr(RCGCUART) &= ~(1 << unit); // Disable clock for UART while in Running mode
        scr(SCGCUART) &= ~(1 << unit); // Disable clock for UART while in Sleep mode
        scr(DCGCUART) &= ~(1 << unit); // Disable clock for UART while in Deep Sleep mode
    }

    void clock_spi(unsigned int unit) {
        // Enable the SSI module using the SYS_CTRL_RCGCSSI register
        scr(RCGCSSI) |= 1 << unit; // Enable clock for SSI while in Running mode
        scr(SCGCSSI) |= 1 << unit; // Enable clock for SSI while in Sleep mode
        scr(DCGCSSI) |= 1 << unit; // Enable clock for SSI while in Deep Sleep mode
    }

    void unclock_spi(unsigned int unit) {
        // Disable the SSI module using the SYS_CTRL_RCGCSSI register
        scr(RCGCSSI) &= ~(1 << unit); // Disable clock for SSI while in Running mode
        scr(SCGCSSI) &= ~(1 << unit); // Disable clock for SSI while in Sleep mode
        scr(DCGCSSI) &= ~(1 << unit); // Disable clock for SSI while in Deep Sleep mode
    }

    void clock_i2c() {
        // Enable the I2C clock using the SYS_CTRL_RCGCI2C register
        scr(RCGCI2C) |= 0x1; // When the CPU is in active (run) mode
        scr(SCGCI2C) |= 0x1; // When the CPU is in Sleep mode
        scr(DCGCI2C) |= 0x1; // When the CPU is in Deep Sleep mode
    }

    void unclock_i2c(){
        // Disable the I2C clock using the SYS_CTRL_RCGCI2C register
        scr(RCGCI2C) &= ~0x1; // When the CPU is in active (run) mode
        scr(SCGCI2C) &= ~0x1; // When the CPU is in Sleep mode
        scr(DCGCI2C) &= ~0x1; // When the CPU is in Deep Sleep mode
    }

    void clock_usb() {
        // TODO: is there a clock gating for USB?
    }

    void clock_timer(unsigned int unit) {
        scr(RCGCGPT) |= 1 << unit;
        scr(SCGCGPT) |= 1 << unit;
        scr(DCGCGPT) |= 1 << unit;
    }

    void unclock_timer(unsigned int unit) {
        scr(RCGCGPT) &= ~(1 << unit);
        scr(SCGCGPT) &= ~(1 << unit);
        scr(DCGCGPT) &= ~(1 << unit);
    }

    void clock_ieee802_15_4() {
        scr(RCGCRFC) |= 1 << 0;
        scr(SCGCRFC) |= 1 << 0;
        scr(DCGCRFC) |= 1 << 0;
    }

    void unclock_ieee802_15_4() {
        scr(RCGCRFC) &= ~(1 << 0);
        scr(SCGCRFC) &= ~(1 << 0);
        scr(DCGCRFC) &= ~(1 << 0);
    }

    void power_mode(const Power_Mode & mode) {
        scr(PMCTL) = mode;
    }

    void wake_on(const Wakeup_Event & event) {
        scr(IWE) = event;
    }

    void enable_alternate_interrupt_mapping() {
        scr(I_MAP) |= I_MAP_ALTMAP; // Enable alternate interrupt mapping
    }
private:
    volatile Reg32 & scr(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
