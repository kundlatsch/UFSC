// EPOS ARM Cortex GIC Mediator Declarations

#ifndef __cortex_gic_h
#define __cortex_gic_h

#include <architecture/cpu.h>
#define __ic_common_only__
#include <machine/ic.h>
#undef __ic_common_only__

__BEGIN_SYS

class GIC: public IC_Common
{
protected:
    typedef CPU::Reg32 Reg32;

    static const unsigned int INT_ID_MASK = 0x3ff;

public:
    // IRQs
    static const unsigned int IRQS = Traits<IC>::IRQS;
    static const unsigned int HARD_INT = 0;
    typedef Interrupt_Id IRQ;
    enum {
        IRQ_SOFTWARE0           = 0,
        IRQ_SOFTWARE1           = 1,
        IRQ_SOFTWARE2           = 2,
        IRQ_SOFTWARE3           = 3,
        IRQ_SOFTWARE4           = 4,
        IRQ_SOFTWARE5           = 5,
        IRQ_SOFTWARE6           = 6,
        IRQ_SOFTWARE7           = 7,
        IRQ_SOFTWARE8           = 8,
        IRQ_SOFTWARE9           = 9,
        IRQ_SOFTWARE10          = 10,
        IRQ_SOFTWARE11          = 11,
        IRQ_SOFTWARE12          = 12,
        IRQ_SOFTWARE13          = 13,
        IRQ_SOFTWARE14          = 14,
        IRQ_SOFTWARE15          = 15,
        IRQ_GLOBAL_TIMER        = 27,
        IRQ_NFIQ                = 28,
        IRQ_PRIVATE_TIMER       = 29,
        IRQ_AWDT                = 30,
        IRQ_NIRQ                = 31,
        IRQ_APU0                = 32,
        IRQ_APU1                = 33,
        IRQ_L2                  = 34,
        IRQ_OCM                 = 35,
        IRQ_PMU0                = 37,
        IRQ_PMU1                = 38,
        IRQ_XADC                = 39,
        IRQ_DEVC                = 40,
        IRQ_SWDT                = 41,
        IRQ_TTC0_0              = 42,
        IRQ_TTC0_1              = 43,
        IRQ_TTC0_2              = 44,
        IRQ_DMAC_ABORT          = 45,
        IRQ_DMAC0               = 46,
        IRQ_DMAC1               = 47,
        IRQ_DMAC2               = 48,
        IRQ_DMAC3               = 49,
        IRQ_SMC                 = 50,
        IRQ_QSPI                = 51,
        IRQ_GPIO                = 52,
        IRQ_USB0                = 53,
        IRQ_ETHERNET0           = 54,
        IRQ_ETHERNET0_WAKEUP    = 55,
        IRQ_SDIO0               = 56,
        IRQ_I2C0                = 57,
        IRQ_SPI0                = 58,
        IRQ_UART0               = 59,
        IRQ_CAN0                = 60,
        IRQ_PL0                 = 61,
        IRQ_PL1                 = 62,
        IRQ_PL2                 = 63,
        IRQ_PL3                 = 64,
        IRQ_PL4                 = 65,
        IRQ_PL5                 = 66,
        IRQ_PL6                 = 67,
        IRQ_PL7                 = 68,
        IRQ_TTC1_0              = 69,
        IRQ_TTC1_1              = 70,
        IRQ_TTC1_2              = 71,
        IRQ_DMAC4               = 72,
        IRQ_DMAC5               = 73,
        IRQ_DMAC6               = 74,
        IRQ_DMAC7               = 75,
        IRQ_USB1                = 76,
        IRQ_ETHERNET1           = 76,
        IRQ_ETHERNET1_WAKEUP    = 78,
        IRQ_SDIO1               = 79,
        IRQ_I2C1                = 80,
        IRQ_SPI1                = 81,
        IRQ_UART1               = 82,
        IRQ_CAN1                = 83,
        IRQ_PL8                 = 84,
        IRQ_PL9                 = 85,
        IRQ_PL10                = 86,
        IRQ_PL11                = 87,
        IRQ_PL12                = 88,
        IRQ_PL13                = 89,
        IRQ_PL14                = 90,
        IRQ_PL15                = 91,
        IRQ_PARITY              = 92
    };

    // GIC offsets from private peripheral space
    enum {
        GICCPU                      = 0x0100,
        GICDIST                     = 0x1000
    };

    // CPU Interface Registers offsets
    enum {                                      // Description                  Type    Value after reset
        ICCICR                      = 0x000,    // CPU Interface Control        r/w     0x00000000
        ICCPMR                      = 0x004,    // Interrupt Priority Mask      r/w     0x00000000
        ICCIAR                      = 0x00c,    // Interrupt Acknowledge        r/o     0x000003ff
        ICCEOIR                     = 0x010     // End Of Interrupt             w/o     -
    };

    // Useful bits in ICCICR
    enum {                                      // Description                  Type    Value after reset
        ITF_EN_S                    = 1 << 0,   // Enable secure signaling      r/w     0
        ITF_EN_NS                   = 1 << 1,   // Enable non-secure signaling  r/w     0
        ACK_CTL                     = 1 << 2    // Acknowledge control          r/w     0
    };

    // Distributor Registers offsets
    enum {                                      // Description                  Type    Value after reset
        ICDDCR                      = 0x000,    // Distributor Control          r/w     0
        ICDISER0                    = 0x100,    // Interrupt Set-Enable         r/w     0x0000ffff
        ICDISER1                    = 0x104,    // Interrupt Set-Enable         r/w     0x00000000
        ICDISER2                    = 0x108,    // Interrupt Set-Enable         r/w     0x00000000
        ICDISERn                    = 0x11c,    // Interrupt Set-Enable         r/w     0x00000000
        ICDICER0                    = 0x180,    // Interrupt Clear-Enable       r/w     0x0000ffff
        ICDICER1                    = 0x184,    // Interrupt Clear-Enable       r/w     0x00000000
        ICDICER2                    = 0x188,    // Interrupt Clear-Enable       r/w     0x00000000
        ICDICERn                    = 0x19c,    // Interrupt Clear-Enable       r/w     0x00000000
        ICDSGIR                     = 0xf00     // Software Generated Interrupt
    };

    // Useful bits in ICDDCR
    enum {                                      // Description                  Type    Value after reset
        DIST_EN_S                   = 1 << 0    // Enable secure interrupts     r/w     0
    };
};


class GIC_Distributor: public GIC
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::GIC_DIST_BASE) GIC_Distributor".

public:
    void enable() {
        gic_dist(ICDISER0) = ~0;
        gic_dist(ICDISER1) = ~0;
        gic_dist(ICDISER2) = ~0;
    }

    void enable(Interrupt_Id id) {
        gic_dist(ICDISER0 + (id / 32) * 4) = 1 << (id % 32);
    }

    void disable() {
        gic_dist(ICDICER0) = ~0;
        gic_dist(ICDICER1) = ~0;
        gic_dist(ICDICER1) = ~0;
    }

    void disable(Interrupt_Id id) {
        gic_dist(ICDICER0 + (id / 32) * 4) = 1 << (id % 32);
    }

    int irq2int(int i) { return i; }
    int int2irq(int i) { return i; }

    void send_sgi(unsigned int cpu, Interrupt_Id id) {
        Reg32 target_list = 1 << cpu;
        Reg32 filter_list = 0;
        gic_dist(ICDSGIR) = ((filter_list << 24) | (target_list << 16) | (id & 0x0f));
    }

    void send_sgi(Interrupt_Id id, Reg32 target_list, Reg32 filter_list) {
        Reg32 aux = id & 0x0f;
        target_list = target_list & 0x0f;
        filter_list = filter_list & 0x0f;
        aux = aux | (target_list << 16);
        aux = aux | (filter_list << 24);
        gic_dist(ICDSGIR) = aux;
    }

    void smp_init(unsigned int cores) {
        Reg32 interrupt_id = 0x0; // reset id

        Reg32 target_list = 0x0;
        if(cores <= CPU::cores())
            for(unsigned int i = 0; i < cores; i++)
                target_list |= 1 << (i);
        else
            for(unsigned int i = 0; i < CPU::cores(); i++)
                target_list |= 1 << (i);

        Reg32 filter_list = 0x01; // except the current

        send_sgi(interrupt_id, target_list, filter_list);
    }

    void init() {
        // Enable distributor
        gic_dist(ICDDCR) = DIST_EN_S;
    }

protected:
    volatile Reg32 & gic_dist(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }

    // The versions bellow are dynamic and work on any A9, but are quite inefficient
    //    static volatile Reg32 & gic_dist(unsigned int offset) {
    //        register Reg32 base asm("r0");
    //        ASM("mrc p15, 4, %0, c15, c0, 0" : "=r"(base)); // get private peripheral space base into r0
    //        return reinterpret_cast<volatile Reg32 *>(base + GICDIST)[offset / sizeof(Reg32)];
    //    }
};


class GIC_CPU: public GIC
{
public:
    Interrupt_Id int_id() {
        Reg32 icciar = gic_cpu(ICCIAR) & INT_ID_MASK;

        // For every read of a valid interrupt id from the ICCIAR, the ISR must
        // perform a matching write to the ICCEOIR
        gic_cpu(ICCEOIR) = icciar;
        return icciar;
    }

    void init() {
        // Mask no interrupts
        gic_cpu(ICCPMR) = 0xf0;

        // Enable interrupts signaling by the CPU interfaces to the connected processors
        gic_cpu(ICCICR) = ACK_CTL | ITF_EN_NS | ITF_EN_S;
    }

protected:
    volatile Reg32 & gic_cpu(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }

    // The versions bellow are dynamic and work on any A9, but are quite inefficient
    //    static volatile Reg32 & gic_cpu(unsigned int offset) {
    //        register Reg32 base asm("r0");
    //        ASM("mrc p15, 4, %0, c15, c0, 0" : "=r"(base)); // get private peripheral space base into r0
    //        return reinterpret_cast<volatile Reg32 *>(base + GICCPU)[offset / sizeof(Reg32)];
    //    }
};

__END_SYS

#endif
