// EPOS PC FPGA Mediator Declarations

#ifndef __pc_fpga_h
#define __pc_fpga_h

#include <architecture/cpu.h>
#include <architecture/mmu.h>
#include <machine/ic.h>
#include <machine/fpga.h>

__BEGIN_SYS

class XAP1052
{
protected:
    // Type Imports
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::IO_Port IO_Port;
    typedef CPU::IO_Irq IO_Irq;
    typedef MMU::DMA_Buffer DMA_Buffer;

    // PCI ID
    static const unsigned int PCI_VENDOR_ID = 0x10ee;
    static const unsigned int PCI_DEVICE_ID = 0x6024;
    static const unsigned int PCI_REG_CTRL = 0;

public:
    // Offsets from base I/O address (PCI region 0)
    enum {
        DCSR       = 0x00,
        DDMACR     = 0x04,
        WDMATLPA   = 0x08,
        WDMATLPS   = 0x0c,
        WDMATLPC   = 0x10,
        WDMATLPP   = 0x14,
        RDMATLPP   = 0x18,
        RDMATLPA   = 0x1c,
        RDMATLPS   = 0x20,
        RDMATLPC   = 0x24,
        WDMAPERF   = 0x28,
        RDMAPERF   = 0x2c,
        RDMASTAT   = 0x30,
        NRDCOMP    = 0x34,
        RCOMPDSIZW = 0x38,
        DLWSTAT    = 0x3c,
        DLTRSSTAT  = 0x40,
        DMISCCONT  = 0x44,
    };

public:
    static void reset() {
        reg(DCSR) |= 1;                     // Write: DCSR (offset 0) with value of 1 (Reset Device)
        reg(DCSR) |= 0;                     // Write: DCSR (offset 0) with value of 0 (Make Active)

        reg(WDMATLPA) = _dma_buf->log_address(); // Write: Write DMA TLP Address register with starting address
        reg(WDMATLPS) = 0x20;               // Write: Write DMA TLP Size register with default value (32dwords)
        reg(WDMATLPC) = 0x2000;             // Write: Write DMA TLP Count register with default value (2000)
        reg(WDMATLPP) = 0x00000000;         // Write: Write DMA TLP Pattern register with default value (0x0)

        reg(RDMATLPP) = 0xfeedbeef;         // Write: Read DMA Expected Data Pattern with default value (feedbeef)
        reg(RDMATLPA) = _dma_buf->log_address() + 4 * 1024; // Write: Read DMA TLP Address register with starting address.
        reg(RDMATLPS) = 0x20;               // Write: Read DMA TLP Size register with default value (32dwords)
        reg(RDMATLPC) = 0x2000;             // Write: Read DMA TLP Count register with default value (2000)
    }

protected:
    static Log_Addr & reg(unsigned int o) { return _base[o / sizeof(Log_Addr)]; }

protected:
    static Log_Addr _base;
    static DMA_Buffer * _dma_buf;
};


// PC FPGA
class FPGA: private FPGA_Common, private XAP1052
{
    friend class Machine;

private:
    // Traits
    static const unsigned int DMA_BUFFER_SIZE = Traits<FPGA>::DMA_BUFFER_SIZE;

public:
    typedef XAP1052 Engine;

public:
    FPGA() {}

    static void reset() { return Engine::reset(); }

    static void int_handler(IC::Interrupt_Id interrupt);

private:
    static void init();
};

__END_SYS

#endif
