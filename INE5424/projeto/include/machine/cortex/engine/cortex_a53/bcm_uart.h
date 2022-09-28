// EPOS ARM Cortex-A53 BCM UART (similar to 16550) Mediator Declarations

#ifndef __bcm_uart_h
#define __bcm_uart_h

#include <architecture/cpu.h>
#define __uart_common_only__
#include <machine/uart.h>
#undef __uart_common_only__

__BEGIN_SYS

class BCM_UART: public UART_Common
{
    // This is a hardware object
    // Use with something like "new (Memory_Map::UARTx_BASE) BCM_UART"

private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg32 Reg32;

    static const unsigned int CLOCK = Traits<UART>::CLOCK;

public:
    // Registers offsets from BASE (i.e. this)
    enum {                              // Description                          Type    Value after reset       size
        GPFSEL0         = 0x00000,      // GPIO Function Select 0               r/w                             32
        GPFSEL1         = 0x00004,      // GPIO Function Select 1               r/w                             32
        GPFSEL2         = 0x00008,      // GPIO Function Select 2               r/w                             32
        GPFSEL3         = 0x0000c,      // GPIO Function Select 3               r/w                             32
        GPFSEL4         = 0x00010,      // GPIO Function Select 4               r/w                             32
        GPFSEL5         = 0x00014,      // GPIO Function Select 5               r/w                             32
        GPSET0          = 0x0001c,      // GPIO Pin Output Set 0                wo                              32
        GPSET1          = 0x00020,      // GPIO Pin Output Set 1                wo                              32
        GPCLR0          = 0x00028,      // GPIO Pin Output Clear 0              wo                              32
        GPCLR1          = 0x0002c,      // GPIO Pin Output Clear 1              wo                              32
        GPLEV0          = 0x00034,      // GPIO Pin Level 0                     ro                              32
        GPLEV1          = 0x00038,      // GPIO Pin Level 1                     ro                              32
        GPEDS0          = 0x00040,      // GPIO Pin Event Detect State 0        r/w                             32
        GPEDS1          = 0x00044,      // GPIO Pin Event Detect State 1        r/w                             32
        GPREN0          = 0x0004c,      // GPIO Pin Rising Edge Detect Enable 0 r/w                             32
        GPREN1          = 0x00050,      // GPIO Pin Rising Edge Detect Enable 1 r/w                             32
        GPPUD           = 0x00094,      // GPIO Pin Pull-up/down Enable         r/w                             32
        GPPUDCLK0       = 0x00098,      // GPIO Pin Pull-up/down Enable Clock 0 r/w                             32
        GPPUDCLK1       = 0x0009c,      // GPIO Pin Pull-up/down Enable Clock 1 r/w                             32
        AUX_ENABLES     = 0x15004,      // Auxiliary enables                                                    3
        AUX_MU_IO_REG   = 0x15040,      // Mini Uart I/O Data                                                   8
        AUX_MU_IER_REG  = 0x15044,      // Mini Uart Interrupt Enable                                           8
        AUX_MU_IIR_REG  = 0x15048,      // Mini Uart Interrupt Identify                                         8
        AUX_MU_LCR_REG  = 0x1504c,      // Mini Uart Line Control                                               8
        AUX_MU_MCR_REG  = 0x15050,      // Mini Uart Modem Control                                              8
        AUX_MU_LSR_REG  = 0x15054,      // Mini Uart Line Status                                                8
        AUX_MU_MSR_REG  = 0x15058,      // Mini Uart Modem Status                                               8
        AUX_MU_SCRATCH  = 0x1505c,      // Mini Uart Scratch                                                    8
        AUX_MU_CNTL_REG = 0x15060,      // Mini Uart Extra Control                                              8
        AUX_MU_STAT_REG = 0x15064,      // Mini Uart Extra Status                                               8
        AUX_MU_BAUD_REG = 0x15068       // Mini Uart Baudrate                                                   8
    };

    // Useful Bits in the Flag Register
    enum {                              // Description                              Type    Value after reset
        RXF_LSR_DR      = 1 <<  0,      // Receive FIFO Data Ready                  r/w     0
        TXF_LSR_EMPTY   = 1 <<  5,      // Transmit FIFO Empty                      r/w     1
        TXF_LSR_IDLE    = 1 <<  6,      // Transmit FIFO Full                       r/w     0
        RXFF_LSR_OVERRUN= 1 <<  1,      // Receiver FIFO Overrun                    ro      0
        RX_STAT_IDLE    = 1 <<  2,      // Receiver is Idle stat on AUX_MU_STAT     ro      1
        TX_STAT_IDLE    = 1 <<  3,      // Transmitter is Idle stat on AUX_MU_STAT  ro      1
        RX_STAT_OVERRUN = 1 <<  4,      // Receiver Overrun stat on AUX_MU_STAT     ro      1
        TX_STAT_FULL    = 1 <<  5,      // Transmitter FIFO is Full                 ro      0
        TX_STAT_EMPTY   = 1 <<  8,      // Transmitter FIFO is Empty                ro      0
        RX_STAT_LEVEL_FULL = 1 <<  16,  // Receiver FIFO is Empty                   ro      0
        TX_STAT_LEVEL_FULL = 1 <<  24   // Transmitter FIFO is Empty                ro      0
    };

    // Useful Bits in the Control Register
    enum {                              // Description                              Type    Value after reset
        UEN             = 1 <<  0,      // Enable                                   r/w     0
        // LBE             = 1 <<  7,      // Loop Back Enable                      r/w     0
        RXEN            = 1 <<  0,      // Receiver Enable                          r/w     1
        TXEN            = 1 <<  1,      // Transmitter Enable                       r/w     1
        TX_INT_ENABLE   = 1 <<  0,      // Enable Transmit interrupt, if DLAB=0     ro      0
        RX_INT_ENABLE   = 1 <<  1,      // Enable Receive interrupt, if DLAB=0      ro      0
        GPIO_PIN_14     = 7 << 12,      // Maps TXD to GPIO_PIN_14                  r/w     0
        GPIO_PIN_15     = 7 << 15,      // Maps TXD to GPIO_PIN_15                  r/w     0
        ALT_FUNC_5_PIN14= 2 << 12,      // Defines GPIO Mapping according to ALT 5  r/w     0
        ALT_FUNC_5_PIN15= 2 << 15,      // Defines GPIO Mapping according to ALT 5  r/w     0
    };
public:
    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        unsigned int ra;

        // Initialize UART
        uart(AUX_ENABLES) = UEN;
        uart(AUX_MU_IER_REG) = 0;
        uart(AUX_MU_CNTL_REG) = 0;
        uart(AUX_MU_LCR_REG) = 3;       // 8 bits
        uart(AUX_MU_MCR_REG) = 0;


        uart(AUX_MU_IER_REG) = 0x5;

        uart(AUX_MU_IIR_REG) = 0xC6;    // disable interrupts
        uart(AUX_MU_BAUD_REG) = ((CLOCK/baud_rate)/8)-1;    // CLOCK is 250,000,000 / 115,200 / 8 = 271.27; int(271.27 - 1) = 270 
        
        // Map UART to GPIO pins
        ra = uart(GPFSEL1);
        ra &= ~(GPIO_PIN_14);       // GPIO 14
        ra |= ALT_FUNC_5_PIN14;     // alternate function 5
        ra &= ~(GPIO_PIN_15);       // GPIO 15
        ra |= ALT_FUNC_5_PIN15;     // alternate function 5
        
        uart(GPFSEL1) = ra;
        uart(GPPUD) = 0;
        for(ra = 0; ra < 150; ra++) asm volatile ("nop"); // 150 cycles to synchronize
        uart(GPPUDCLK0) = (1 << 14) | (1 << 15);
        for(ra = 0; ra < 150; ra++) asm volatile ("nop"); // 150 cycles to synchronize
        uart(GPPUDCLK0) = 0;                    // flush GPIO setup
        uart(AUX_MU_CNTL_REG) = TXEN | RXEN;      // enable Tx, Rx
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        *data_bits = ((uart(AUX_MU_LCR_REG) & 0x1) ? 8 : 7);
        *parity = 0; //by Manual definition
        *baud_rate = CLOCK/(uart(AUX_MU_BAUD_REG)+1)/8;
        *stop_bits = 1; //by Manual definition
    }

    Reg8 rxd() { return uart(AUX_MU_IO_REG); }
    void txd(Reg8 c) { uart(AUX_MU_IO_REG) = c; }

    bool rxd_ok() { return (uart(AUX_MU_LSR_REG) & RXF_LSR_DR); }
    bool txd_ok() { return (uart(AUX_MU_LSR_REG) & TXF_LSR_EMPTY); }

    bool rxd_full() { return (uart(AUX_MU_STAT_REG) & RX_STAT_LEVEL_FULL); }
    bool txd_empty() { return (uart(AUX_MU_LSR_REG) & TXF_LSR_IDLE); }

    bool busy() { return !(uart(AUX_MU_LSR_REG) & TXF_LSR_IDLE); }

    void enable() { uart(AUX_ENABLES) = UEN;}
    void disable() { uart(AUX_ENABLES) &= ~UEN; }

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        uart(AUX_MU_IER_REG) &= ((receive ? RX_INT_ENABLE : 0) | (transmit ? TX_INT_ENABLE : 0));
    }
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        uart(AUX_MU_IER_REG) &= ~((receive ? RX_INT_ENABLE : 0) | (transmit ? TX_INT_ENABLE : 0));
    }

    void reset() {
        unsigned int b, db, p, sb;
        config(&b, &db, &p, &sb);
        config(b, db, p, sb);
    }

private:
    volatile Reg32 & uart(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
