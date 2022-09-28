// EPOS Zynq-7000 (Cortex-A9) UART Mediator Declarations

#ifndef __zynq_uart_h
#define __zynq_uart_h

#include <architecture/cpu.h>
#include <machine/uart.h>
#include <system/memory_map.h>

__BEGIN_SYS

class Zynq_UART_Engine: public UART_Common
{
    // This is a hardware object
    // Use with something like "new (Memory_Map::UARTx_BASE) Zynq_UART_Engine"

private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg32 Reg32;

    static const unsigned int CLOCK = Traits<UART>::CLOCK;

public:
    // Registers offsets from BASE (i.e. this)
    enum {                                      // Description              Type    Value after reset
        CONTROL_REG0                = 0x00,     // Control                  r/w     0x00000128
        MODE_REG0                   = 0x04,     // Mode                     r/w     0x00000000
        INTRPT_EN_REG0              = 0x08,     // Interrupt enable         r/w     0x00000000
        INTRPT_DIS_REG0             = 0x0c,     // Interrupt disable        r/w     0x00000000
        BAUD_RATE_GEN_REG0          = 0x18,     // Baud rate generator      r/w     0x0000028B
        RCVR_FIFO_TRIGGER_LEVEL0    = 0x20,     // Receive trigger level    r/w     0x00000020
        CHANNEL_STS_REG0            = 0x2c,     // Channel status           ro      0x00000000
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

public:
    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        Reg32 mode = uart(MODE_REG0) & ~0xff;

        mode |= stop_bits == 2 ? NBSTOP2 : NBSTOP1;
        mode |= data_bits == 8 ? CHRL8 : data_bits == 7 ? CHRL7 : CHRL6;
        mode |= parity == 2 ? PAREVEN : parity == 1 ? PARODD : PARNONE;
        uart(MODE_REG0) = mode;

        // Set the baud rate
        Reg32 br = CLOCK / (7 * baud_rate);
        uart(BAUD_RATE_DIVIDER_REG0) = 6;
        uart(BAUD_RATE_GEN_REG0) = br;

        // Set the receiver trigger level to 1
        uart(RCVR_FIFO_TRIGGER_LEVEL0) = 1;

        // Enable and reset RX and TX data paths
        uart(CONTROL_REG0) = RXRES | TXRES | RXEN | TXEN;
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        Reg32 mode = uart(MODE_REG0);

        *baud_rate = CLOCK/(7*uart(BAUD_RATE_GEN_REG0));
        *data_bits = (mode & 0x6) == CHRL8 ? 8 : (mode & 0x6) == CHRL7 ? 7 : 6;
        *parity = (mode & 0x38) == PAREVEN ? 2 : (mode & 0x38) == PARODD ? 1 : 0;
        *stop_bits = (mode & 0xC0) == NBSTOP2 ? 2 : 1;
    }

    unsigned char rxd() { return uart(TX_RX_FIFO0); }
    void txd(unsigned char ch) { uart(TX_RX_FIFO0) = ch; }

    bool rxd_ok() { return uart(CHANNEL_STS_REG0) & STS_RTRIG; }
    bool txd_ok() { return !(uart(CHANNEL_STS_REG0) & STS_TFUL); }

    bool rxd_full();
    bool txd_empty();

    bool busy();

    void enable() {}
    void disable() {}

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        uart(INTRPT_EN_REG0) |= (receive ? INTRPT_RTRIG : 0) | (transmit ? INTRPT_TTRIG : 0);
        uart(INTRPT_DIS_REG0) &= ~(receive ? INTRPT_RTRIG : 0) & ~(transmit ? INTRPT_TTRIG : 0);
    }
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        uart(INTRPT_EN_REG0) &= ~(receive ? INTRPT_RTRIG : 0) & ~(transmit ? INTRPT_TTRIG : 0);
        uart(INTRPT_DIS_REG0) |= (receive ? INTRPT_RTRIG : 0) | (transmit ? INTRPT_TTRIG : 0);
    }

    void reset() {
        unsigned int b, db, p, sb;
        config(&b, &db, &p, &sb);
        config(b, db, p, sb);
    }

    void loopback(bool flag) {
        Reg32 mode = uart(MODE_REG0) & ~0x300;

        if(flag)
            mode |= CHMODELB;
        else
            mode |= CHMODENORM;

        uart(MODE_REG0) = mode;
    }

private:
    volatile Reg32 & uart(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

class UART_Engine: public UART_Common
{
private:
    static const unsigned int UNITS = Traits<UART>::UNITS;

public:
    UART_Engine(unsigned int unit, unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits): _unit(unit), _uart(new(reinterpret_cast<void *>(Memory_Map::UART0_BASE + 0x1000 * unit)) Zynq_UART_Engine) {
        assert(unit < UNITS);
        config(baud_rate, data_bits, parity, stop_bits);
    }

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        _uart->disable();
        _uart->config(baud_rate, data_bits, parity, stop_bits);
        _uart->enable();
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        _uart->config(baud_rate, data_bits, parity, stop_bits);
    }

    char rxd() { return _uart->rxd(); }
    void txd(char c) { return _uart->txd(c); }

    bool rxd_ok() { return _uart->rxd_ok(); }
    bool txd_ok() { return _uart->txd_ok(); }

    bool rxd_full() { return _uart->rxd_full(); }
    bool txd_empty() { return _uart->txd_empty(); }

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        _uart->int_enable(receive, transmit, line, modem);
    }
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        _uart->int_disable(receive, transmit, line, modem);
    }

    void reset() { _uart->reset(); }
    void loopback(bool flag) { _uart->loopback(flag); }

    void power(const Power_Mode & mode) {}

    static void init() {}

private:
    unsigned int _unit;
    Zynq_UART_Engine * _uart;
};

__END_SYS

#endif
