// EPOS PC UART Mediator Declarations

#ifndef __pc_uart_h
#define __pc_uart_h

#include <machine/uart.h>
#include <architecture/cpu.h>

__BEGIN_SYS

// National Semiconductors NS16550AF (PC16550D) UART
class NS16550AF
{
private:
    typedef CPU::IO_Port IO_Port;
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;

    static const unsigned int UNITS = Traits<UART>::UNITS;
    static const unsigned int CLOCK = Traits<UART>::CLOCK / 16;

public:
    // Register Addresses (relative to base I/O port)
    typedef Reg8 Address;
    enum {
        THR = 0, // Transmit Holding	W,   DLAB = 0
        RBR = 0, // Receive Buffer 	R,   DLAB = 0
        IER = 1, // Interrupt Enable	R/W, DLAB = 0 [0=DR,1=THRE,2=LI,3=MO]
        FCR = 2, // FIFO Control	W   [0=EN,1=RC,2=XC,3=RDY,67=TRG]
        IIR = 2, // Interrupt Id	R   [0=PEN,12=ID,3=FIFOTO,67=1]
        LCR = 3, // Line Control	R/W [01=DL,2=SB,345=P,6=BRK,7=DLAB]
        MCR = 4, // Modem Control	R/W [0=DTR,1=RTS,2=OUT1,3=OUT2,4=LB]
        LSR = 5, // Line Status		R/W [0=DR,1=OE,2=PE,3=FE,4=BI,5=THRE,
                 //			     6=TEMT,7=FIFOE]
        MSR = 6, // Modem Status	R/W [0=CTS,1=DSR,2=RI,3=DCD,4=LBCTS,
                 // 			     5=LBDSR,6=LBRI,7=LBDCD]
        SCR = 7, // Scratch 		R/W
        DLL = 0, // Divisor Latch LSB	R/W, DLAB = 1
        DLH = 1  // Divisor Latch MSB	R/W, DLAB = 1
    };

public:
    NS16550AF(unsigned int unit, unsigned int brate, unsigned int dbits, unsigned int par, unsigned int sbits):
        _port((unit == 0) ? Traits<UART>::COM1 : (unit == 1) ? Traits<UART>::COM2 : (unit == 2) ?Traits<UART>::COM3 : Traits<UART>::COM4) {
        assert(unit < UNITS);
        config(brate, dbits, par, sbits);
    }

    void config(unsigned int brate, unsigned int dbits, unsigned int par, unsigned int sbits) {
        // Disable all interrupts
        reg(IER, 0);

        // Set clock divisor
        unsigned int div = CLOCK / brate;
        dlab(true);
        reg(DLL, div);
        reg(DLH, div >> 8);
        dlab(false);

        // Set data word length (5, 6, 7 or 8)
        Reg8 lcr = dbits - 5;

        // Set parity (0 [no], 1 [odd], 2 [even])
        if(par) {
            lcr |= 1 << 3;
            lcr |= (par - 1) << 4;
        }

        // Set stop-bits (1, 2 or 3 [1.5])
        lcr |= (sbits > 1) ? (1 << 2) : 0;

        reg(LCR, lcr);

        // Enables Tx and Rx FIFOs, clear them, set trigger to 14 bytes
        reg(FCR, 0xc7);

        // Set DTR, RTS and OUT2 of MCR
        reg(MCR, reg(MCR) | 0x0b);
    }

    void config(unsigned int * brate, unsigned int * dbits, unsigned int * par, unsigned int * sbits) {
        // Get clock divisor
        dlab(true);
        *brate = CLOCK * ((reg(DLH) << 8) | reg(DLL));
        dlab(false);

        Reg8 lcr = reg(LCR);

        // Get data word length (LCR bits 0 and 1)
        *dbits = (lcr & 0x03) + 5;

        // Get parity (LCR bits 3 [enable] and 4 [odd/even])
        *par = (lcr & 0x08) ? ((lcr & 0x10) ? 2 : 1 ) : 0;

        // Get stop-bits  (LCR bit 2 [0 - >1, 1&D5 -> 1.5, 1&!D5 -> 2)
        *sbits = (lcr & 0x04) ? ((*dbits == 5) ? 3 : 2 ) : 1;
    }

    Reg8 rxd() { return reg(RBR); }
    void txd(Reg8 c) { reg(THR, c); }

    void int_enable(bool receive = true, bool send = true, bool line = true, bool modem = true) {
        reg(IER, receive | (send << 1) | (line << 2) | (modem << 3));
    }
    void int_disable(bool receive = true, bool send = true, bool line = true, bool modem = true) {
        reg(IER, reg(IER) & ~(receive | (send << 1) | (line << 2) | (modem << 3)));
    }

    void reset() {
        // Reconfiguring the UART implicitly resets it
        unsigned int b, db, p, sb;
        config(&b, &db, &p, &sb);
        config(b, db, p, sb);
    }

    void loopback(bool flag) { reg(MCR, reg(MCR) | (flag << 4)); }

    bool rxd_ok() { return reg(LSR) & (1 << 0); }
    bool txd_ok() { return reg(LSR) & (1 << 5); }

    void dtr() { reg(MCR, reg(MCR) | (1 << 0)); }
    void rts() { reg(MCR, reg(MCR) | (1 << 1)); }
    bool cts() { return reg(MSR) & (1 << 0); }
    bool dsr() { return reg(MSR) & (1 << 1); }
    bool dcd() { return reg(MSR) & (1 << 3); }
    bool ri()  { return reg(MSR) & (1 << 2); }

    bool overrun_error() { return reg(LSR) & (1 << 1); }
    bool parity_error()  { return reg(LSR) & (1 << 2); }
    bool framing_error() { return reg(LSR) & (1 << 3); }

private:
    Reg8 reg(Address addr) { return CPU::in8(_port + addr); }
    void reg(Address addr, Reg8 value) { CPU::out8(_port + addr, value); }

    void dlab(bool f) { reg(LCR, (reg(LCR) & 0x7f) | (f << 7)); }

private:
    IO_Port _port;
};

class UART: private UART_Common, private NS16550AF
{
private:
    typedef NS16550AF Engine;

    static const unsigned int BAUD_RATE = Traits<UART>::DEF_BAUD_RATE;
    static const unsigned int DATA_BITS = Traits<UART>::DEF_DATA_BITS;
    static const unsigned int PARITY = Traits<UART>::DEF_PARITY;
    static const unsigned int STOP_BITS = Traits<UART>::DEF_STOP_BITS;

public:
    UART(unsigned int unit = 0, unsigned int baud_rate = BAUD_RATE, unsigned int data_bits = DATA_BITS, unsigned int parity = PARITY, unsigned int stop_bits = STOP_BITS)
    : Engine(unit, baud_rate, data_bits, parity, stop_bits) {}

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        Engine::config(baud_rate, data_bits, parity, stop_bits);
    }
    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        Engine::config(*baud_rate, *data_bits, *parity, *stop_bits);
    }

    char get() { while(!rxd_ok()); return rxd(); }
    void put(char c) { while(!txd_ok()); txd(c); }

    bool ready_to_get() { return rxd_ok(); }
    bool ready_to_put() { return txd_ok(); }

    void int_enable(bool receive = true, bool send = true, bool line = true, bool modem = true) {
        Engine::int_enable(receive, send, line, modem);
    }
    void int_disable(bool receive = true, bool send = true, bool line = true, bool modem = true) {
        Engine::int_disable(receive, send, line, modem);
    }

    void loopback(bool flag) { Engine::loopback(flag); }

    void power(const Power_Mode & mode);
};

__END_SYS

#endif
