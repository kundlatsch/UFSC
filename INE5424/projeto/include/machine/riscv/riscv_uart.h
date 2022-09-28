// EPOS RISC-V UART Mediator Declarations

#ifndef __riscv_uart_h
#define __riscv_uart_h

#include <architecture/cpu.h>
#include <machine/uart.h>
#include <system/memory_map.h>

__BEGIN_SYS

class UART: private UART_Common
{
private:

    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg32 Reg32;

    static const unsigned int UNIT = Traits<UART>::DEF_UNIT;
    static const unsigned int BAUD_RATE = Traits<UART>::DEF_BAUD_RATE;
    static const unsigned int DATA_BITS = Traits<UART>::DEF_DATA_BITS;
    static const unsigned int PARITY = Traits<UART>::DEF_PARITY;
    static const unsigned int STOP_BITS = Traits<UART>::DEF_STOP_BITS;

    // UART registers offsets from UART_BASE
    enum {
        DIV_LSB         = 0,
        TXD             = 0,
        RXD             = 0,
        IER             = 1,
        DIV_MSB         = 1,
        FCR             = 2,
        ISR             = 2,
        LCR             = 3,
        MCR             = 4,
        LSR             = 5
    };

    // Useful bits from multiple registers
    enum {
        DATA_READY          = 1 << 0,
        THOLD_REG           = 1 << 5,
        TEMPTY_REG          = 1 << 6,
        DATA_BITS_MASK      = 1 << 1 | 1 << 0,
        PARITY_MASK         = 1 << 3,
        DLAB_ENABLE         = 1 << 7,
        STOP_BITS_MASK      = 1 << 2,
        LOOPBACK_MASK       = 1 << 4,
        FIFO_ENABLE         = 1 << 0,
        DEFAULT_DATA_BITS   = 5
    };

public:
    using UART_Common::NONE;
    using UART_Common::EVEN;
    using UART_Common::ODD;

public:
    UART(unsigned int unit = UNIT, unsigned int baud_rate = BAUD_RATE, unsigned int data_bits = DATA_BITS, unsigned int parity = PARITY, unsigned int stop_bits = STOP_BITS) {
        config(baud_rate, data_bits, parity, stop_bits);
    }

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        // 0x3 for 8, 0x2 for 7, 0x1 for 6 and default (0x0) is 5 bits
        Reg8 word_length = Reg8(data_bits - DEFAULT_DATA_BITS);
        reg(LCR) = word_length;
        reg(FCR) = FIFO_ENABLE;

        // If we cared about the divisor, the code below would set the divisor
        // from a global clock rate of 22.729 MHz (22,729,000 cycles per second)
        // to a signaling rate of 115200 (BAUD). We usually have much faster signalling
        // rates nowadays, but this demonstrates what the divisor actually does.
        // The formula given in the NS16500A specification for calculating the divisor
        // is:
        // divisor = ceil( (clock_hz) / (baud_sps x 16) )
        Reg32 br = Traits<UART>::CLOCK / BAUD_RATE;
        if (Traits<UART>::CLOCK / br > BAUD_RATE) {
            br += 1;
        } // get ceiling
        Reg8 div_least = Reg8(br & 0xff);
        Reg8 div_most = Reg8(br >> 8);

        // Notice that the divisor register DLL (divisor latch least) and DLM (divisor
        // latch most) have the same base address as the receiver/transmitter and the
        // interrupt enable register. To change what the base address points to, we
        // open the "divisor latch" by writing 1 into the Divisor Latch Access Bit
        // (DLAB), which is bit index 7 of the Line Control Register (LCR) which
        // is at base_address + 3.
        reg(LCR) = (word_length | DLAB_ENABLE);

        // Now, base addresses 0 and 1 point to DLL and DLM, respectively.
        // Put the lower 8 bits of the divisor into DLL
        reg(DIV_LSB) = div_least;
        reg(DIV_MSB) = div_most;

        // Now that we've written the divisor, we never have to touch this again. In 
        // hardware, this will divide the global clock (22.729 MHz) into one suitable
        // for 115200 signals per second. So, to once again get access to the 
        // RBR/THR/IER registers, we need to close the DLAB bit by clearing it to 0.
        reg(LCR) = word_length;
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        *data_bits = Reg32(reg(LCR) & DATA_BITS_MASK) + DEFAULT_DATA_BITS;

        reg(LCR) = (*data_bits | DLAB_ENABLE);
        unsigned int div = Reg32(reg(DIV_MSB) << 8) | Reg32(reg(DIV_LSB));
        reg(LCR) = *data_bits;
        *baud_rate = Traits<UART>::CLOCK / div;

        *parity = (Reg32(reg(LCR)) & PARITY_MASK) >> PARITY_MASK;

        *stop_bits = (Reg32(reg(LCR) & STOP_BITS_MASK) >> STOP_BITS_MASK) + 1;
    }

    Reg8 rxd() { 
        return reg(RXD);
    }
    void txd(Reg8 c) { 
        reg(TXD) = c;
    }

    bool rxd_ok() { 
        return (reg(LSR) & DATA_READY); 
    }

    bool txd_ok() { 
        return (reg(LSR) & THOLD_REG);
    }

    bool rxd_full() { return false; } //still not defined
    
    bool txd_empty() { 
        return (reg(LSR) & TEMPTY_REG); 
    }

    bool busy() {
        return false; // not applicable
    }

    char get() { while(!rxd_ok()); return rxd(); }
    void put(char c) { while(!txd_ok()); txd(c); }

    void flush() { while(!txd_empty()); }
    bool ready_to_get() { return rxd_ok(); }
    bool ready_to_put() { return txd_ok(); }

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        reg(IER) = receive | (transmit << 1) | (line << 2) | (modem << 3);
    }
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        reg(IER) = reg(IER) & ~(receive | (transmit << 1) | (line << 2) | (modem << 3));
    }

    void reset() {
        // Reconfiguring the UART implicitly resets it
        unsigned int b, db, p, sb;
        config(&b, &db, &p, &sb);
        config(b, db, p, sb);
    }

    void loopback(bool flag) {
        Reg8 mask = 0xff;
        mask -= LOOPBACK_MASK;
        mask += (flag << 4); // if 1, restore flag, else make it disable loopback
        reg(MCR) = reg(MCR) & mask; 
    }

    void power(const Power_Mode & mode) {}

private:
    static void init() {}

    static volatile CPU::Reg8 & reg(unsigned int o) { return reinterpret_cast<volatile CPU::Reg8 *>(Memory_Map::UART_BASE)[o / sizeof(CPU::Reg8)]; }
};

__END_SYS

#endif
