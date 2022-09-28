// EPOS Zynq-7000 (Cortex-A9) Mediator Declarations

#ifndef __zynq_machine_h
#define __zynq_machine_h

#include <machine/machine.h>
#include <machine/cortex/engine/cortex_a9/gic.h>
#include <machine/cortex/engine/cortex_a9/scu.h>
#include <system/memory_map.h>
#include <system.h>

__BEGIN_SYS

class Zynq: private Machine_Common
{
    friend Machine; // for pre_init() and init()

private:
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

public:
    static const unsigned int IRQS = 96;
    static const unsigned int TIMERS = 3;
    static const unsigned int UARTS = 2;
    static const unsigned int GPIO_PORTS = 5;
    static const bool supports_gpio_power_up = false;

    // Base address for memory-mapped System Control Registers
    enum {
        UART0_BASE                  = 0xE0000000,
        UART1_BASE                  = 0xE0001000,
        SLCR_BASE                   = 0xF8000000,
        PPS_BASE                    = 0xf8f00000, // A9 Private Peripheral Space
        GIC_CPU_BASE                = 0xF8F00100,
        GLOBAL_TIMER_BASE           = 0xF8F00200,
        PRIVATE_TIMER_BASE          = 0XF8F00600,
        GIC_DIST_BASE               = 0xF8F01000
    };

    // Register addresses relative to base
    enum {                                      // Description              Type    Value after reset
        CONTROL_REG0                = 0x00,     // Control                  r/w     0x00000128
        MODE_REG0                   = 0x04,     // Mode                     r/w     0x00000000
        INTRPT_EN_REG0              = 0x08,     // Interrupt enable         r/w     0x00000000
        INTRPT_DIS_REG0             = 0x0C,     // Interrupt disable        r/w     0x00000000
        BAUD_RATE_GEN_REG0          = 0x18,     // Baud rate generator      r/w     0x0000028B
        RCVR_FIFO_TRIGGER_LEVEL0    = 0x20,     // Receive trigger level    r/w     0x00000020
        CHANNEL_STS_REG0            = 0x2C,     // Channel status           ro      0x00000000
        TX_RX_FIFO0                 = 0x30,     // Transmit/receive FIFO    r/w     0x00000000
        BAUD_RATE_DIVIDER_REG0      = 0x34      // Baud rate divider        r/w     0x0000000F
    };

    // Useful bits in the CONTROL_REG0 register
    enum {                                      // Description              Type    Value after reset
        RXRES                       = 1 << 0,   // Reset Rx data path       r/w     0
        TXRES                       = 1 << 1,   // Reset Tx data path       r/w     0
        RXEN                        = 1 << 2,   // Receive enable           r/w     0
        TXEN                        = 1 << 4    // Transmit enable          r/w     0
    };

    // Useful bits in the MODE_REG0 register
    enum {                                      // Description              Type    Value after reset
        CHRL8                       = 0 << 1,   // Character Length 8 bits  r/w     0
        CHRL7                       = 2 << 1,   // Character Length 7 bits  r/w     0
        CHRL6                       = 3 << 1,   // Character Length 6 bits  r/w     0
        PAREVEN                     = 0 << 3,   // Even parity              r/w     0
        PARODD                      = 1 << 3,   // Odd parity               r/w     0
        PARNONE                     = 4 << 3,   // No parity                r/w     0
        NBSTOP2                     = 2 << 6,   // 2 stop bits              r/w     0
        NBSTOP1                     = 0 << 6,   // 1 stop bit               r/w     0
        CHMODENORM                  = 0 << 8,   // Normal mode              r/w     0
        CHMODELB                    = 2 << 8    // Loopback mode            r/w     0
    };

    // Useful bits in the INTRPT_EN_REG0, and INTRPT_DIS_REG0 registers
    enum {                                      // Description              Type    Value after reset
        INTRPT_RTRIG                = 1 << 0,   // Receiver FIFO empty      wo      0
        INTRPT_TTRIG                = 1 << 10   // Transmitter FIFO trigger wo      0
    };

    // Useful bits in the CHANNEL_STS_REG0 register
    enum {                                      // Description              Type    Value after reset
        STS_RTRIG                   = 1 << 0,   // Receiver FIFO trigger    ro      0
        STS_TFUL                    = 1 << 4    // Transmitter FIFO full    ro      0
    };

    // SLCR Registers offsets
    enum {                                      // Description
        SLCR_LOCK                   = 0x004,    // Lock the SLCR
        SLCR_UNLOCK                 = 0x008,    // Unlock the SLCR
        UART_CLK_CTRL               = 0x154,    // UART Ref Clock Control
        FPGA0_CLK_CTRL              = 0x170,    // PL Clock 0 Output control
        PSS_RST_CTRL                = 0x200,    // PS Software Reset Control
        FPGA_RST_CTRL               = 0x240     // FPGA Software Reset Control
    };

    // Useful bits in SLCR_LOCK
    enum {                                      // Description                  Type    Value after reset
        LOCK_KEY                    = 0x767B    // Lock key                     wo      0
    };

    // Useful bits in SLCR_UNLOCK
    enum {                                      // Description                  Type    Value after reset
        UNLOCK_KEY                  = 0xDF0D    // Unlock key                   wo      0
    };

    // Useful bits in FPGAN_CLK_CTRL
    enum {                                      // Description                  Type    Value after reset
        DIVISOR0                    = 1 << 8,   // First cascade divider        r/w     0x18
        DIVISOR1                    = 1 << 20   // Second cascade divider       r/w     0x1
    };

    // CPU Interface Registers offsets
    enum {                                      // Description
        ICCICR                      = 0x000,    // CPU Interface Control
        ICCPMR                      = 0x004,    // Interrupt Priority Mask
        ICCIAR                      = 0x00C,    // Interrupt Ack
        ICCEOIR                     = 0x010     // End Of Interrupt
    };

    // Useful bits in ICCICR
    enum {                                      // Description                  Type    Value after reset
        ITF_EN_S                    = 1 << 0,   // Enable secure signaling      r/w     0
        ITF_EN_NS                   = 1 << 1,   // Enable non-secure signaling  r/w     0
        ACK_CTL                     = 1 << 2    // Acknowledge control          r/w     0
    };

    // Global Timer Registers offsets
    enum {                                      // Description
        GTCTRL                      = 0x00,     // Low Counter
        GTCTRH                      = 0x04,     // High Counter
        GTCLR                       = 0x08,     // Control
        GTISR                       = 0x0C      // Interrupt Status
    };

    // Private Timer Registers offsets
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

    // Distributor Registers offsets
    enum {                                      // Description
        ICDDCR                      = 0x000,    // Distributor Control
        ICDISER0                    = 0x100,    // Interrupt Set-Enable
        ICDISER1                    = 0x104,    // Interrupt Set-Enable
        ICDISER2                    = 0x108,    // Interrupt Set-Enable
        ICDICER0                    = 0x180,    // Interrupt Clear-Enable
        ICDICER1                    = 0x184,    // Interrupt Clear-Enable
        ICDICER2                    = 0x188     // Interrupt Clear-Enable
    };

    // Useful bits in ICDDCR
    enum {                                      // Description                  Type    Value after reset
        DIST_EN_S                   = 1 << 0    // Enable secure interrupts     r/w     0
    };

protected:
    Zynq() {}

    using Machine_Common::delay;
    using Machine_Common::clear_bss;

    static void reboot();
    static void poweroff() { reboot(); }

    static const UUID & uuid() { return System::info()->bm.uuid; } // TODO: System_Info is not populated in this machine

    static unsigned int cpu_id() {
        int id;
        ASM("mrc p15, 0, %0, c0, c0, 5" : "=r"(id) : : );
        return id & 0x3;
    }

    static unsigned int n_cpus() {
        int n;
        ASM("mrc p15, 4, %0, c15, c0, 0 \t\n\
             ldr %0, [%0, #0x004]" : "=r"(n) : : );
        return (n & 0x3) + 1;
    }

    static void smp_barrier_init(unsigned int n_cpus) { _cores = n_cpus; }

    static void enable_uart(unsigned int unit) {}

// PM
    static void power_uart(unsigned int unit, const Power_Mode & mode) {
        assert(unit < UARTS);
        switch(mode) {
        case ENROLL:
            break;
        case DISMISS:
            break;
        case SAME:
            break;
        case FULL:
            slcr(UART_CLK_CTRL) |= 1 << unit;
            break;
        case LIGHT:
            break;
        case SLEEP:
            break;
        case OFF:
            slcr(UART_CLK_CTRL) &= ~(1 << unit);
            break;
        }
    }

    // Zynq's Global Timer clock cannot be powered off
    static void power_user_timer(unsigned int unit, const Power_Mode & mode) {
        assert(unit < UARTS);
        switch(mode) {
        case ENROLL:
            break;
        case DISMISS:
            break;
        case SAME:
            break;
        case FULL:
            break;
        case LIGHT:
        case SLEEP:
        case OFF:
            break;
        }
    }

    // Returns the frequency set, -1 if frequency can't be set
    static int fpga0_clk_freq(unsigned int freq) {
        const unsigned int div_max = 63, tol = 20;
        unsigned int div0 = 0, div1 = 0,
                io_pll_clock = Traits<Machine>::IO_PLL_CLOCK;
        Reg32 tmp;

        while(++div1 <= div_max) {
            div0 = 1;
            while(++div0 <= div_max)
                if((io_pll_clock/(div0*div1) < (freq + freq/tol)) &&
                        (io_pll_clock/(div0*div1) > (freq - freq/tol)))
                    goto set_clk_ctrl;
        }

        return -1;

        set_clk_ctrl:
        tmp = slcr(FPGA0_CLK_CTRL);
        tmp &= ~((DIVISOR0 * 0x3f) | (DIVISOR1 * 0x3f));
        slcr(FPGA0_CLK_CTRL) = tmp | (DIVISOR0 * div0) | (DIVISOR1 * div1);

        return io_pll_clock/(div0*div1);
    }

    // PL logic connecting to the PS must not be reset when active transactions
    // exist, since uncompleted transactions could be left pending in the PS
    static void fpga_reset(int n) {
        assert(n < 4);
        slcr(FPGA_RST_CTRL) |= 1 << n;
        // FPGA peripherals will reset on the rising/falling edge of their
        // clocks if reset is asserted. This "rough" 1 us delay will ensure all
        // peripherals operating on clocks higher than 1 MHz will be correctly
        // reseted.
        for(unsigned int i = 0; i < Traits<CPU>::CLOCK/1000000; i++)
            ASM("nop");
        slcr(FPGA_RST_CTRL) &= ~(1 << n);
    }

    static void fpga_reset() { for(int i = 0; i < 4; i++) fpga_reset(i); }

    static void unlock_slcr() { slcr(SLCR_UNLOCK) = UNLOCK_KEY; }
    static void lock_slcr() { slcr(SLCR_LOCK) = LOCK_KEY; }

public:
    static volatile Reg32 & slcr(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(SLCR_BASE)[o / sizeof(Reg32)]; }
    static volatile Reg32 & gic_cpu(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(GIC_CPU_BASE)[o / sizeof(Reg32)]; }
    static volatile Reg32 & gic_dist(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(GIC_DIST_BASE)[o / sizeof(Reg32)]; }
    static volatile Reg32 & global_timer(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(GLOBAL_TIMER_BASE)[o / sizeof(Reg32)]; }
    static volatile Reg32 & private_timer(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(PRIVATE_TIMER_BASE)[o / sizeof(Log_Addr)]; }

    static void pre_init() {}
    static void init();

private:
    static volatile unsigned int _cores;
};

typedef Zynq Machine_Model;

__END_SYS

#endif
