// EPOS Cortex-M I/O Control Module Mediator Declarations

#ifndef __cortex_m3_ioctrl_h
#define __cortex_m3_ioctrl_h

#include <architecture/cpu.h>

__BEGIN_SYS

class IOCtrl
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::IOC_BASE) IOCtrl".

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers' offsets in IOCtrl (not to be confused with System Control Space)
    enum {                              // Description                                          Type    Value after reset
        PA0_SEL        = 0x000,         //                                                      rw      0x00000000
        PA1_SEL        = 0x004,         //                                                      rw      0x00000000
        PA2_SEL        = 0x008,         //                                                      rw      0x00000000
        PA3_SEL        = 0x00c,         //                                                      rw      0x00000000
        PA4_SEL        = 0x010,         //                                                      rw      0x00000000
        PA5_SEL        = 0x014,         //                                                      rw      0x00000000
        PA6_SEL        = 0x018,         //                                                      rw      0x00000000
        PA7_SEL        = 0x01c,         //                                                      rw      0x00000000
        PB0_SEL        = 0x020,         //                                                      rw      0x00000000
        PB1_SEL        = 0x024,         //                                                      rw      0x00000000
        PB2_SEL        = 0x028,         //                                                      rw      0x00000000
        PB3_SEL        = 0x02c,         //                                                      rw      0x00000000
        PB4_SEL        = 0x030,         //                                                      rw      0x00000000
        PB5_SEL        = 0x034,         //                                                      rw      0x00000000
        PB6_SEL        = 0x038,         //                                                      rw      0x00000000
        PB7_SEL        = 0x03c,         //                                                      rw      0x00000000
        PC0_SEL        = 0x040,         //                                                      rw      0x00000000
        PC1_SEL        = 0x044,         //                                                      rw      0x00000000
        PC2_SEL        = 0x048,         //                                                      rw      0x00000000
        PC3_SEL        = 0x04c,         //                                                      rw      0x00000000
        PC4_SEL        = 0x050,         //                                                      rw      0x00000000
        PC5_SEL        = 0x054,         //                                                      rw      0x00000000
        PC6_SEL        = 0x058,         //                                                      rw      0x00000000
        PC7_SEL        = 0x05c,         //                                                      rw      0x00000000
        PD0_SEL        = 0x060,         //                                                      rw      0x00000000
        PD1_SEL        = 0x064,         //                                                      rw      0x00000000
        PD2_SEL        = 0x068,         //                                                      rw      0x00000000
        PD3_SEL        = 0x06c,         //                                                      rw      0x00000000
        PD4_SEL        = 0x070,         //                                                      rw      0x00000000
        PD5_SEL        = 0x074,         //                                                      rw      0x00000000
        PD6_SEL        = 0x078,         //                                                      rw      0x00000000
        PD7_SEL        = 0x07c,         //                                                      rw      0x00000000
        PA0_OVER       = 0x080,         //                                                      rw      0x00000004
        PA1_OVER       = 0x084,         //                                                      rw      0x00000004
        PA2_OVER       = 0x088,         //                                                      rw      0x00000004
        PA3_OVER       = 0x08c,         //                                                      rw      0x00000004
        PA4_OVER       = 0x090,         //                                                      rw      0x00000004
        PA5_OVER       = 0x094,         //                                                      rw      0x00000004
        PA6_OVER       = 0x098,         //                                                      rw      0x00000004
        PA7_OVER       = 0x09c,         //                                                      rw      0x00000004
        PB0_OVER       = 0x0a0,         //                                                      rw      0x00000004
        PB1_OVER       = 0x0a4,         //                                                      rw      0x00000004
        PB2_OVER       = 0x0a8,         //                                                      rw      0x00000004
        PB3_OVER       = 0x0ac,         //                                                      rw      0x00000004
        PB4_OVER       = 0x0b0,         //                                                      rw      0x00000004
        PB5_OVER       = 0x0b4,         //                                                      rw      0x00000004
        PB6_OVER       = 0x0b8,         //                                                      rw      0x00000004
        PB7_OVER       = 0x0bc,         //                                                      rw      0x00000004
        PC0_OVER       = 0x0c0,         //                                                      rw      0x00000004
        PC1_OVER       = 0x0c4,         //                                                      rw      0x00000004
        PC2_OVER       = 0x0c8,         //                                                      rw      0x00000004
        PC3_OVER       = 0x0cc,         //                                                      rw      0x00000004
        PC4_OVER       = 0x0d0,         //                                                      rw      0x00000004
        PC5_OVER       = 0x0d4,         //                                                      rw      0x00000004
        PC6_OVER       = 0x0d8,         //                                                      rw      0x00000004
        PC7_OVER       = 0x0dc,         //                                                      rw      0x00000004
        PD0_OVER       = 0x0e0,         //                                                      rw      0x00000004
        PD1_OVER       = 0x0e4,         //                                                      rw      0x00000004
        PD2_OVER       = 0x0e8,         //                                                      rw      0x00000004
        PD3_OVER       = 0x0ec,         //                                                      rw      0x00000004
        PD4_OVER       = 0x0f0,         //                                                      rw      0x00000004
        PD5_OVER       = 0x0f4,         //                                                      rw      0x00000004
        PD6_OVER       = 0x0f8,         //                                                      rw      0x00000004
        PD7_OVER       = 0x0fc,         //                                                      rw      0x00000004
        UARTRXD_UART0  = 0x100,         //                                                      rw      0x00000000
        UARTCTS_UART1  = 0x104,         //                                                      rw      0x00000000
        UARTRXD_UART1  = 0x108,         //                                                      rw      0x00000000
        CLK_SSI_SSI0   = 0x10c,         //                                                      rw      0x00000000
        SSIRXD_SSI0    = 0x110,         //                                                      rw      0x00000000
        SSIFSSIN_SSI0  = 0x114,         //                                                      rw      0x00000000
        CLK_SSIIN_SSI0 = 0x118,         //                                                      rw      0x00000000
        CLK_SSI_SSI1   = 0x11c,         //                                                      rw      0x00000000
        SSIRXD_SSI1    = 0x120,         //                                                      rw      0x00000000
        SSIFSSIN_SSI1  = 0x124,         //                                                      rw      0x00000000
        CLK_SSIIN_SSI1 = 0x128,         //                                                      rw      0x00000000
        I2CMSSDA       = 0x12c,         //                                                      rw      0x00000000
        I2CMSSCL       = 0x130,         //                                                      rw      0x00000000
        GPT0OCP1       = 0x134,         //                                                      rw      0x00000000
        GPT0OCP2       = 0x138,         //                                                      rw      0x00000000
        GPT1OCP1       = 0x13c,         //                                                      rw      0x00000000
        GPT1OCP2       = 0x140,         //                                                      rw      0x00000000
        GPT2OCP1       = 0x144,         //                                                      rw      0x00000000
        GPT2OCP2       = 0x148,         //                                                      rw      0x00000000
        GPT3OCP1       = 0x14c,         //                                                      rw      0x00000000
        GPT3OCP2       = 0x150          //                                                      rw      0x00000000
    };

    // Peripheral Signal Select Values (same for all Pxx_SEL registers)
    enum {
        UART0_TXD       =  0x00,
        UART1_RTS       =  0x01,
        UART1_TXD       =  0x02,
        SSI0_TXD        =  0x03,
        SSI0_CLK_OUT    =  0x04,
        SSI0_FSS_OUT    =  0x05,
        SSI0_TX_SER_OUT =  0x06,
        SSI1_TXD        =  0x07,
        SSI1_CLK_OUT    =  0x08,
        SSI1_FSS_OUT    =  0x09,
        SSI1_TX_SER_OUT =  0x0a,
        I2C_SDA         =  0x0b,
        I2C_SCL         =  0x0c,
        GPT0CP1         =  0x0d,
        GPT0CP2         =  0x0e,
        GPT1CP1         =  0x0f,
        GPT1CP2         =  0x10,
        GPT2CP1         =  0x11,
        GPT2CP2         =  0x12,
        GPT3CP1         =  0x13,
        GPT3CP2         =  0x14
    };

    // Valid values for Pxx_OVER registers
    enum {
        OE  = 0x8, // output enable
        PUE = 0x4, // pull-up enable
        PDE = 0x2, // pull-down enable
        ANA = 0x1, // analog enable
        DIS = 0x0  // override disable
    };

public:
    void enable_external_clock() {
        ioc(PD6_OVER) = ANA;
        ioc(PD7_OVER) = ANA;
    }

    void enable_uart(unsigned int unit) {
        // assume UART has been powered on before getting here (see UART::power())
        if(unit == 0) {
            ioc(PA1_SEL) = UART0_TXD;           // configure pin A1 to UART0_TXD
            ioc(PA1_OVER) = OE;                 // enable IO pads to drive outputs
            ioc(PA0_OVER) = 0;
            ioc(UARTRXD_UART0) = (0 << 3) + 0;  // connect input signals to the UART module (port << 3) + pin

            // calling function must invoke PL061::select_pin_function(PIN0 | PIN1, FUN_ALTERNATE) on GPIOA afterwards to connect the pins
        } else {
            ioc(PC4_SEL) = UART1_TXD;
            ioc(PC4_OVER) = OE;
            ioc(PC3_OVER) = 0;
            ioc(UARTRXD_UART1) = (2 << 3) + 3;

            // calling function must invoke PL061::select_pin_function(PIN3 | PIN4, FUN_ALTERNATE) on GPIOC afterwards to connect the pins
        }
    }

    void enable_modem_on_uart1() {
        // The M95 GPRS board uses UART1 in non-standard pins
        ioc(PD1_SEL) = UART1_TXD;
        ioc(PD1_OVER) = OE;
        ioc(PD0_OVER) = 0;
        ioc(UARTRXD_UART1) = (3 << 3) + 0;

        // calling function must invoke PL061::select_pin_function(PIN0 | PIN1, FUN_ALTERNATE) on GPIOD afterwards to connect the pins
        // PL061 * pl061 = new(reinterpret_cast<void *>(Memory_Map::GPIOD_BASE)) PL061;
        // pl061->select_pin_function(PL061::PIN0 | PL061::PIN1, PL061::FUN_ALTERNATE);
    }

    void enable_spi(unsigned int unit, bool master) {
        // assume SPI has been powered on before getting here (see SPI::power())

        if(unit == 0) {
            if(master)
                ioc(PA2_SEL) = SSI0_CLK_OUT;
            else
                ioc(CLK_SSI_SSI0) = 0x02; // PA2

            ioc(PA3_SEL) = SSI0_FSS_OUT;
            ioc(PA5_SEL) = SSI0_TXD;
            ioc(SSIRXD_SSI0) = 0x04; // PA4
        } else {
            if(master)
                ioc(PA2_SEL) = SSI1_CLK_OUT;
            else
                ioc(CLK_SSI_SSI1) = 0x02; // PA2

            ioc(PA3_SEL) = SSI1_FSS_OUT;
            ioc(PA5_SEL) = SSI1_TXD;
            ioc(SSIRXD_SSI1) = 0x04; // PA4
        }

        // enable IO pads to drive outputs making the corresponding IPxx_OVER bits in IOC_Pxx_OVER register 0x8 (OE - Output Enable)
        if(master)
            ioc(PA2_OVER) = OE;
        ioc(PA3_OVER) = OE;
        ioc(PA5_OVER) = OE;

        // calling function must invoke PL061::select_pin_function(PIN2 | PIN3 | PIN4 | PIN5, FUN_ALTERNATE) afterwards to connect the pins
    }

    void enable_i2c(unsigned int port_sda, unsigned int pin_sda, unsigned int port_scl, unsigned int pin_scl) {
        auto sda = PA0_SEL + 0x20 * port_sda + 0x4 * pin_sda;
        auto scl = PA0_SEL + 0x20 * port_scl + 0x4 * pin_scl;

        ioc(sda + 0x80) = DIS;
        ioc(scl + 0x80) = DIS;

        ioc(sda) = I2C_SDA;
        ioc(scl) = I2C_SCL;

        ioc(I2CMSSDA) = (port_sda << 3) + pin_sda;
        ioc(I2CMSSCL) = (port_scl << 3) + pin_scl;
    }

    void gpio_pull_up(unsigned int port, unsigned int pin) { ioc(PA0_OVER + 0x20 * port + 0x4 * pin) = PUE; }
    void gpio_pull_down(unsigned int port, unsigned int pin) { ioc(PA0_OVER + 0x20 * port + 0x4 * pin) = PDE; }
    void gpio_floating(unsigned int port, unsigned int pin) { ioc(PA0_OVER + 0x20 * port + 0x4 * pin) = ANA; }

private:
    volatile Reg32 & ioc(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
