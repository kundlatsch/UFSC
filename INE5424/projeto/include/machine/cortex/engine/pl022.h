// EPOS ARM PrimeCell PL022 SPI Mediator Declarations

#ifndef __cortex_m3_pl022_h
#define __cortex_m3_pl022_h

#include <architecture/cpu.h>
#define __spi_common_only__
#include <machine/spi.h>
#undef __spi_common_only__

__BEGIN_SYS

class PL022: public SPI_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::SPIx_BASE) PL022".

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers offsets from BASE (i.e. this)
    enum {
        CR0    = 0x000,
        CR1    = 0x004,
        DR     = 0x008,
        SR     = 0x00c,
        CPSR   = 0x010,
        IM     = 0x014,
        RIS    = 0x018,
        MIS    = 0x01c,
        ICR    = 0x020,
        DMACTL = 0x024,
        CC     = 0xfc8,
    };

    // Useful bits in the SSI CR0 register
    enum {                         // Description                   Type    Value after reset
        CR0_SCR         = 1 << 8,  // serial clock rate              RW      0x00
        SPH             = 1 << 7,  // serial clock phase high        RW      0x0
                                   // applicable only to the Motorola SPI format
        SPO             = 1 << 6,  // serial clock phase low         RW      0x0
                                   // applicable only to the Motorola SPI format
        FRF             = 1 << 4,  // frame format selection         RW      0x0
                                   // 00: Motorola SPI format
                                   // 01: TI synchronous serial frame format
                                   // 10: National microwave frame format
                                   // 11: Reserved
        DSS             = 1 << 0,  // data size select              RW      0x0
                                   // 0000-0010: Reserved
                                   // 0011: 4-bit data
                                   // 0100: 5-bit data
                                   // 0101: 6-bit data
                                   // 0110: 7-bit data
                                   // 0111: 8-bit data
                                   // 1000: 9-bit data
                                   // 1001: 10-bit data
                                   // 1010: 11-bit data
                                   // 1011: 12-bit data
                                   // 1100: 13-bit data
                                   // 1101: 14-bit data
                                   // 1110: 15-bit data
                                   // 1111: 16-bit data
    };

    // Frame format
    enum {
        FORMAT_MOTO0    = 0x00, // Moto fmt, polarity 0, phase 0
        FORMAT_MOTO1    = 0x02, // Moto fmt, polarity 0, phase 1
        FORMAT_MOTO2    = 0x01, // Moto fmt, polarity 1, phase 0
        FORMAT_MOTO3    = 0x03, // Moto fmt, polarity 1, phase 1
        FORMAT_TI       = 0x10, // TI synchronous serial frame format
        FORMAT_NMW      = 0x20, // National microwave frame format
    };

    // SSI Operation Mode
    using SPI_Common::Mode;

    // Useful bits in the SSI CR1 register
    enum {                          // Description                          Type    Value after reset
        SOD             = 1 << 3,   // slave mode output disable              RW      0x00
                                    // 0: SSI can drive SSITXD in slave output mode
                                    // 1: SSI must not drive the SSITXD output in slave mode
        MS              = 1 << 2,   // master and slave select                RW      0x0
                                    // 0: Device configured as a master (default)
                                    // 1: Device configured as a slave
        SSE             = 1 << 1,   // synchronous serial port enable         RW      0x0
                                    // 0: SSI operation is disabled.
                                    // 1: SSI operation is enabled.
        LBM             = 1 << 0,   // loop-back mode                        RW      0x0
                                    // 0: Normal serial port operation is enabled.
                                    // 1: The output of the transmit serial shifter is connected to the input
                                    // of the receive serial shift register internally.
    };

    // Useful bits in the SSI DR register
    enum {                          // Description                          Type    Value after reset
        DATA       = 1 << 15,       // receive/transmit data register        RW         0x0000
                                    // A read operation reads the receive FIFO. A write operation writes the transmit FIFO.
    };

    // Useful bits in the SSI SR register
    enum {                          // Description                          Type    Value after reset
        BSY             = 1 << 4,   // busy bit                               RO      0x0
        RFF             = 1 << 3,   // receive FIFO full                      RO      0x0
                                    // 0: Receive FIFO is not full.
                                    // 1: Receive FIFO is full.
        RNE             = 1 << 2,   // receive FIFO not empty                 RO      0x0
                                    // 0: Receive FIFO is empty.
                                    // 1: Receive FIFO is not empty.
        TNF             = 1 << 1,   // transmit FIFO not full                 RO      0x0
                                    // 0: Transmit FIFO is full.
                                    // 1: Transmit FIFO is not full.
        TFE             = 1 << 0,   // transmit FIFO empty
                                    // 0: Transmit FIFO is not empty.
                                    // 1: Transmit FIFO is empty
    };

    // Useful bits in the SSI CPSR register
    enum {                          // Description                          Type    Value after reset
        CPSDVSR         = 1 << 7,   // clock prescale divisor                  RW        0x00
                                    // This value must be an even number from 2 to 254, depending on
                                    // the frequency of SSICLK. The LSB always returns zero on reads.
    };

    // Useful bits in the SSI IM register
    enum {                          // Description                          Type    Value after reset
        TXIM            = 1 << 3,   // transmit FIFO interrupt mask                  RW        0x00
                                    // 0: TX FIFO half empty or condition interrupt is masked.
                                    // 1: TX FIFO half empty or less condition interrupt is not masked.
        RXIM            = 1 << 2,   // receive FIFO interrupt mask                   RW        0x00
                                    // 0: RX FIFO half empty or condition interrupt is masked
                                    // 1: RX FIFO half empty or less condition interrupt is not masked
        RTIM            = 1 << 1,   // receive time-out interrupt mask               RW        0x00
                                    // 0: RX FIFO time-out interrupt is masked
                                    // 1: RX FIFO time-out interrupt is not masked
        RORIM           = 1 << 0,   // receive overrun interrupt mask                RW        0x00
                                    // RX FIFO Overrun interrupt is masked
                                    // RX FIFO Overrun interrupt is not masked
    };

    // Useful bits in the SSI RIS register
    enum {                          // Description                              Type    Value after reset
        TXRIS            = 1 << 3,  // SSITXINTR raw state                              RO        0x01
                                    // Gives the raw interrupt state (before masking) of SSITXINTR
        RXRIS            = 1 << 2,  // SSIRXINTR raw state                              RO        0x00
                                    // Gives the raw interrupt state (before masking) of SSIRXINTR
        RTRIS            = 1 << 1,  // SSIRTINTR raw state                             RO        0x00
                                    // Gives the raw interrupt state (before masking) of SSIRTINTR
        RORIS            = 1 << 0,  // SSIRORINTR raw state                            RO        0x00
                                    // Gives the raw interrupt state (before masking) of SSIRORINTR
    };

    // Useful bits in the SSI MIS register
    enum {                          // Description                              Type    Value after reset
        TXMIS            = 1 << 3,  // SSITXINTR masked state                              RO        0x01
                                    // Gives the raw interrupt state (after masking) of SSITXINTR
        RXMIS            = 1 << 2,  // SSIRXINTR masked state                              RO        0x00
                                    // Gives the raw interrupt state (after masking) of SSIRXINTR
        RTMIS            = 1 << 1,  // SSIRTINTR masked state                             RO        0x00
                                    // Gives the raw interrupt state (after masking) of SSIRTINTR
        ROMIS            = 1 << 0,  // SSIRORINTR masked state                            RO        0x00
                                    // Gives the raw interrupt state (after masking) of SSIRORINTR
    };

    // Useful bits in the SSI ICR register
    enum {                          // Description                              Type    Value after reset
        TXDMAE            = 1 << 1, // Transmit DMA enable                             RW        0x00
                                    // 0: uDMA for the transmit FIFO is disabled.
                                    // 1: uDMA for the transmit FIFO is enabled.
        RXDMAE            = 1 << 0, // Receive DMA enable                              RW        0x00
                                    // 0: uDMA for the receive FIFO is disabled.
                                    // 1: uDMA for the receive FIFO is enabled.
    };

    // Useful bits in the SSI CC register
    enum {                          // Description                              Type    Value after reset
        CS                = 1 << 0, // baud and system clock source               RW        0x00
                                    /* bit0 (PIOSC):
                                    1: The SSI baud clock is determined by the IO DIV setting in the
                                    system controller.
                                    0: The SSI baud clock is determined by the SYS DIV setting in the
                                    system controller.
                                    bit1: Unused
                                    bit2: (DSEN) Only meaningful when the system is in deep sleep
                                    mode. This bit is a don't care when not in sleep mode.
                                    1: The SSI system clock is running on the same clock as the baud
                                    clock, as per PIOSC setting above.
                                    0: The SSI system clock is determined by the SYS DIV setting in
                                    the system controller.
                                    */
    };

public:
    void config(Reg32 clock, const Protocol & protocol, const Mode & mode, Reg32 bit_rate, Reg32 data_width) {
        Reg32 pre_div, max_bit_rate, value, format;

        switch(protocol) {
        case MOTO0: format = FORMAT_MOTO0; break;
        case MOTO1: format = FORMAT_MOTO1; break;
        case MOTO2: format = FORMAT_MOTO2; break;
        case MOTO3: format = FORMAT_MOTO3; break;
        case TI:    format = FORMAT_TI; break;
        case NMW:   format = FORMAT_NMW; break;
        }

        // Set the mode
        value = (mode == SLAVE_OD) ? SOD : 0;
        value |= (mode == MASTER) ? 0 : MS;
        ssi(CR1) = value;

        // Set the Clock sources
        ssi(CC) = 0;

        // Set the clock predivider
        max_bit_rate = clock / bit_rate;
        pre_div = 0;
        do
        {
            pre_div += 2;
            value = (max_bit_rate / pre_div) - 1;
        } while(value > 255);

        ssi(CPSR) = pre_div;

        // Set protocol and clock rate
        // logical & with 0x30 (protocol & 0x30) because it contains all the 3 formats (00 | 01 | 10)
        int tmp1 = (format & 3) << 6;
        int tmp2 = format & 0x30;
        value = (value << 8) | tmp1 | tmp2 | (data_width - 1);
        ssi(CR0) = value;
    }

    Reg32 get() {
        // Wait until there is data to be read
        while(!(ssi(SR) & RNE)) ;

        return ssi(DR);
    }

    bool try_get(Reg32 * data) {
        // Return true if the data was read and false otherwise
        if(ssi(SR) & RNE) {
            *data = ssi(DR);
            return true;
        }

        return false;
    }

    void put(Reg32 data) {
        // Wait until there is space
        while(!(ssi(SR) & TNF)) ;

        // Write the data to the SSI
        ssi(DR) = data;
    }

    bool tyr_put(Reg32 data) {
        // Return true if the data has been written and false otherwise
        // Check for space to write.
        if(ssi(SR) & TNF) {
            ssi(DR) = data;
            return true;
        }
        return false;
    }

    bool busy() { return (ssi(SR) & BSY); }
    bool ready_to_get() { return (ssi(SR) & RNE); }
    bool ready_to_put() { return (ssi(SR) & TNF); }

    void int_enable(Reg32 flag) {
        ssi(IM) |= flag;
    }

    void int_disable(Reg32 flag) {
        ssi(IM) &= ~flag;
    }

    void enable() {
        // Read-modify-write the enable bit
        ssi(CR1) |= SSE;
    }

    void disable() {
        // Read-modify-write the enable bit
        ssi(CR1) &= ~SSE;
    }

    void clock_source(Reg32 source) {
        ssi(CC) = source;
    }

    Reg32 clock_source() {
        return ssi(CC);
    }

private:
    volatile Reg32 & ssi(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
