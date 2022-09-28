// EPOS LM3S811 (ARM Cortex-M3) System Control Module Mediator Declarations

#ifndef __lm3s811_sysctrl_h
#define __lm3s811_sysctrl_h

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
    enum {                               // Description                                          Type    Value after reset
         DID0            = 0x000,        // Device Identification 0                              ro      -
         DID1            = 0x004,        // Device Identification 1                              ro      -
         DC0             = 0x008,        // Device Capabilities 0                                ro      0x001f001f
         DC1             = 0x010,        // Device Capabilities 1                                ro      0x001132bf
         DC2             = 0x014,        // Device Capabilities 2                                ro      0x01071013
         DC3             = 0x018,        // Device Capabilities 3                                ro      0xbf0f01ff
         DC4             = 0x01c,        // Device Capabilities 4                                ro      0x0000001f
         PBORCTL         = 0x030,        // Power-On and Brown-Out Reset Control                 rw      0x00007ffd
         LDOPCTL         = 0x034,        // LDO Power Control                                    rw      0x00000000
         SRCR0           = 0x040,        // Software Reset Control 0                             rw      0x00000000
         SRCR1           = 0x044,        // Software Reset Control 1                             rw      0x00000000
         SRCR2           = 0x048,        // Software Reset Control 2                             rw      0x00000000
         RIS             = 0x050,        // Raw Interrupt Status                                 ro      0x00000000
         IMC             = 0x054,        // Interrupt Mask Control                               rw      0x00000000
         MISC            = 0x058,        // Masked Interrupt Status and Clear                    rw 1C   0x00000000
         RESC            = 0x05c,        // Reset Cause  rw      -
         RCC             = 0x060,        // Run-Mode Clock Configuration                         rw      0x078e3ac0
         PLLCFG          = 0x064,        // XTAL to PLL Translation                              ro      -
         RCGC0           = 0x100,        // Run Mode Clock Gating Control Register 0             rw      0x00000040
         RCGC1           = 0x104,        // Run Mode Clock Gating Control Register 1             rw      0x00000000
         RCGC2           = 0x108,        // Run Mode Clock Gating Control Register 2             rw      0x00000000
         SCGC0           = 0x110,        // Sleep Mode Clock Gating Control Register 0           rw      0x00000040
         SCGC1           = 0x114,        // Sleep Mode Clock Gating Control Register 1           rw      0x00000000
         SCGC2           = 0x118,        // Sleep Mode Clock Gating Control Register 2           rw      0x00000000
         DCGC0           = 0x120,        // Deep Sleep Mode Clock Gating Control Register 0      rw      0x00000040
         DCGC1           = 0x124,        // Deep Sleep Mode Clock Gating Control Register 1      rw      0x00000000
         DCGC2           = 0x128,        // Deep Sleep Mode Clock Gating Control Register 2      rw      0x00000000
         DSLPCLKCFG      = 0x144,        // Sleep Clock Configuration                            rw      0x07800000
         CLKVCLR         = 0x150,        // Clock Verification Clear                             rw      0x00000000
         LDOARST         = 0x160         // Allow Unregulated LDO to Reset the Part              rw      0x00000000
     };

    // Useful Bits in the Run Mode Clock Gating Control Register 0
    enum RCGC0 {                        // Description                                          Type    Value after reset
        RCGC0_WDT       = 1 <<  3,      // Watch Dog Timer                                      rw      0
        RCGC0_ADC_125K  = 0 <<  8,      // ADC Max Speed = 125K samp/s                          rw      0
        RCGC0_ADC_250K  = 1 <<  8,      // ADC Max Speed = 250K samp/s                          rw      0
        RCGC0_ADC_500K  = 2 <<  8,      // ADC Max Speed = 500K samp/s                          rw      0
        RCGC0_ADC       = 1 << 16,      // ADC                                                  rw      0
        RCGC0_PWM       = 1 << 20       // PWM                                                  rw      0
    };

    // Useful Bits in the Run Mode Clock Gating Control Register 1
    enum RCGC1 {                        // Description                                          Type    Value after reset
        RCGC1_UART0     = 1 <<  0,      // UART0                                                rw      0
        RCGC1_UART1     = 1 <<  1,      // UART1                                                rw      0
        RCGC1_SSI       = 1 <<  4,      // Synchronous Serial Interface                         rw      0
        RCGC1_I2C       = 1 << 12,      // I2C                                                  rw      0
        RCGC1_TIMER0    = 1 << 16,      // Timer 0                                              rw      0
        RCGC1_TIMER1    = 1 << 17,      // Timer 1                                              rw      0
        RCGC1_TIMER2    = 1 << 18,      // Timer 2                                              rw      0
        RCGC1_COMP0     = 1 << 24       // Analog Comparator 0                                  rw      0
    };

    // Useful Bits in the Run Mode Clock Gating Control Register 2
    enum RCGC2 {                        // Description                                          Type    Value after reset
        RCGC2_GPIOA     = 1 <<  0,      // GPIOA                                                rw      0
        RCGC2_GPIOB     = 1 <<  1,      // GPIOB                                                rw      0
        RCGC2_GPIOC     = 1 <<  2,      // GPIOC                                                rw      0
        RCGC2_GPIOD     = 1 <<  3,      // GPIOD                                                rw      0
        RCGC2_GPIOE     = 1 <<  4       // GPIOE                                                rw      0
    };

    // Useful Bits in the Run-Mode Clock Configuration
    enum RCC {                          // Description                                          Type    Value after reset
        RCC_MOSCDIS     = 1 <<  0,      // Main Oscillator Disable                              rw      0
        RCC_IOSCDIS     = 1 <<  1,      // Internal Oscillator Disable                          rw      0
        RCC_MOSCVER     = 1 <<  2,      // Main Oscillator Verification Timer                   rw      0
        RCC_IOSCVER     = 1 <<  3,      // Internal Oscillator Verification Timer               rw      0
        RCC_OSCSRC      = 0 <<  4,      // Oscillator Source                                    rw      0
        RCC_MOSC        = 0 <<  4,      // Oscillator Source = Main oscillator                  rw      0
        RCC_IOSC        = 1 <<  4,      // Oscillator Source = Internal oscillator              rw      0
        RCC_IOSC4       = 2 <<  4,      // Oscillator Source = Internal oscillator / 4          rw      0
        RCC_XTAL        = 0x0 << 6,     // Crystal Frequency                                    rw      0xb
        RCC_XTAL_1000   = 0x0 << 6,     // Crystal Frequency = 1 MHz (no PLL)
        RCC_XTAL_1843   = 0x1 << 6,     // Crystal Frequency = 1.8432 MHz (no PLL)
        RCC_XTAL_2000   = 0x2 << 6,     // Crystal Frequency = 2 MHz (no PLL)
        RCC_XTAL_2457   = 0x3 << 6,     // Crystal Frequency = 2.4576 MHz (no PLL)
        RCC_XTAL_3579   = 0x4 << 6,     // Crystal Frequency = 3.579545 MHz
        RCC_XTAL_3686   = 0x5 << 6,     // Crystal Frequency = 3.6864 MHz
        RCC_XTAL_4000   = 0x6 << 6,     // Crystal Frequency = 4 MHz
        RCC_XTAL_4096   = 0x7 << 6,     // Crystal Frequency = 4.096 MHz
        RCC_XTAL_4915   = 0x8 << 6,     // Crystal Frequency = 4.9152 MHz
        RCC_XTAL_5000   = 0x9 << 6,     // Crystal Frequency = 5 MHz
        RCC_XTAL_5120   = 0xa << 6,     // Crystal Frequency = 5.12 MHz
        RCC_XTAL_6000   = 0xb << 6,     // Crystal Frequency = 6 MHz
        RCC_XTAL_6144   = 0xc << 6,     // Crystal Frequency = 6.144 MHz
        RCC_XTAL_7378   = 0xd << 6,     // Crystal Frequency = 7.3728 MHz
        RCC_XTAL_8000   = 0xe << 6,     // Crystal Frequency = 8 MHz
        RCC_XTAL_8192   = 0xf << 6,     // Crystal Frequency = 8.129 MHz
        RCC_PLLVER      = 1 << 10,      // PLL Verification                                     rw      0
        RCC_BYPASS      = 1 << 11,      // PLL Bypass                                           rw      1
        RCC_OEN         = 1 << 12,      // PLL Output Enable                                    rw      1
        RCC_PWRDN       = 1 << 13,      // PLL Power Down                                       rw      1
        RCC_PWMDIV      = 1 << 17,      // PWM Unit Clock Divisor                               rw      0x7
        RCC_USEPWMDIV   = 1 << 20,      // Enable PWM Clock Divisor                             rw      0
        RCC_USESYSDIV   = 1 << 22,      // Enable System Clock Divider                          rw      0
        RCC_SYSDIV      = 0x0 << 23,    // System Clock Divisor                                 rw      0xf
        RCC_SYSDIV_1    = 0x0 << 23,    // System Clock Divisor = 2
        RCC_SYSDIV_4    = 0x3 << 23,    // System Clock Divisor = 4 -> 50 MHz
        RCC_SYSDIV_5    = 0x4 << 23,    // System Clock Divisor = 5 -> 40 HMz
        RCC_SYSDIV_8    = 0x7 << 23,    // System Clock Divisor = 8 -> 25 MHz
        RCC_SYSDIV_10   = 0x9 << 23,    // System Clock Divisor = 10 -> 20 MHz
        RCC_SYSDIV_16   = 0xf << 23,    // System Clock Divisor = 16 -> 12.5 HMz
        RCC_ACG         = 1 << 27       // ACG                                                  rw      0
    };

    // Useful Bits in the Raw Interrupt Status
    enum RIS {                          // Description                                          Type    Value after reset
        RIS_PLLLRIS     = 1 <<  6       // PLL Lock Raw Interrupt Status                        ro      0
    };

    enum POWER_MODE {
        ACTIVE = 0,
        PMSLEEP,
        POWER_MODE_0,
        POWER_MODE_1,
        POWER_MODE_2,
        POWER_MODE_3,
    };

    enum WAKE_UP_EVENT {
        EVENT_SLEEP_MODE_TIMER = 1 << 5,
        EVENT_USB = 1 << 4,
        EVENT_GPIO_D = 1 << 3,
        EVENT_GPIO_C = 1 << 2,
        EVENT_GPIO_B = 1 << 1,
        EVENT_GPIO_A = 1 << 0,
    };

public:
    SysCtrl() {}

    void clock_init() {
        // Initialize the clock
        CPU::Reg32 rcc = scr(RCC);

        // bypass PLL and system clock divider while initializing
        rcc |= RCC_BYPASS;
        rcc &= ~RCC_USESYSDIV;
        scr(RCC) = rcc;

        // select the crystal value and oscillator source
        rcc &= ~RCC_XTAL_8192;
        rcc |= RCC_XTAL_6000;
        rcc &= ~RCC_IOSC4;
        rcc |= RCC_MOSC;

        // activate PLL by clearing PWRDN and OEN
        rcc &= ~RCC_PWRDN;
        rcc &= ~RCC_OEN;

        // set the desired system divider and the USESYSDIV bit
        rcc &= ~RCC_SYSDIV_16;
        // Clock setup
        Reg32 sys_div;
        switch(Traits<CPU>::CLOCK) {
        default:
        case 50000000: sys_div = RCC_SYSDIV_4; break;
        case 40000000: sys_div = RCC_SYSDIV_5; break;
        case 25000000: sys_div = RCC_SYSDIV_8; break;
        case 20000000: sys_div = RCC_SYSDIV_10; break;
        case 12500000: sys_div = RCC_SYSDIV_16; break;
        }
        rcc |= sys_div;
        rcc |= RCC_USESYSDIV;
        scr(RCC) = rcc;

        // wait for the PLL to lock by polling PLLLRIS
        while(!(scr(RIS) & RIS_PLLLRIS));

        // enable use of PLL by clearing BYPASS
        rcc &= ~RCC_BYPASS;
        scr(RCC) = rcc;
    }

    void clock_timer(unsigned int unit) {}
    void unclock_timer(unsigned int unit) {}

private:
    volatile Reg32 & scr(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
