// EPOS ARM Cortex-M3 Nested Vectored Interrupt Controller Mediator Declarations

#ifndef __cortex_m3_nvic_h
#define __cortex_m3_nvic_h

#include <architecture/cpu.h>
#define __ic_common_only__
#include <machine/ic.h>
#undef __ic_common_only__

__BEGIN_SYS

class NVIC: public IC_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::SCB_BASE) NVIC".

private:
    typedef CPU::Reg32 Reg32;

public:
    // IRQs
    static const unsigned int IRQS = Traits<IC>::IRQS;
    static const unsigned int EXC_INT = 0;
    static const unsigned int HARD_INT = 16;
    typedef Interrupt_Id IRQ;
    enum {
        IRQ_GPIOA       = 0,
        IRQ_GPIOB       = 1,
        IRQ_GPIOC       = 2,
        IRQ_GPIOD       = 3,
        IRQ_GPIOE       = 4,
        IRQ_UART0       = 5,
        IRQ_UART1       = 6,
        IRQ_SSI0        = 7,
        IRQ_I2C         = 8,
        IRQ_ADC         = 14,
        IRQ_WATCHDOG    = 18,
        IRQ_GPT0A       = 19,
        IRQ_GPT0B       = 20,
        IRQ_GPT1A       = 21,
        IRQ_GPT1B       = 22,
        IRQ_GPT2A       = 23,
        IRQ_GPT2B       = 24,
        IRQ_AC          = 25,
        IRQ_RFTXRX      = 26,
        IRQ_RFERR       = 27,
        IRQ_SC          = 28,
        IRQ_FC          = 29,
        IRQ_AES         = 30,
        IRQ_PKA         = 31,
        IRQ_SMT         = 32,
        IRQ_MACTIMER    = 33,
        IRQ_SSI1        = 34,
        IRQ_GPT3A       = 35,
        IRQ_GPT3B       = 36,
        IRQ_USB         = 44, // Using alternate interrupt mapping
        IRQ_UDMASW      = 46,
        IRQ_UDMAERR     = 47,
        IRQ_LAST        = IRQ_UDMAERR
    };

    // Registers' offsets in System Control Space
    enum {                              // Description                                          Type    Value after reset
        IRQ_ENABLE0     = 0x100,        // Interrupt  0-31 Set Enable                           R/W     0x00000000
        IRQ_ENABLE1     = 0x104,        // Interrupt 32-63 Set Enable                           R/W     0x00000000
        IRQ_ENABLE2     = 0x108,        // Interrupt 64-95 Set Enable                           R/W     0x00000000
        IRQ_DISABLE0    = 0x180,        // Interrupt  0-31 Clear Enable                         R/W     0x00000000
        IRQ_DISABLE1    = 0x184,        // Interrupt 32-63 Clear Enable                         R/W     0x00000000
        IRQ_DISABLE2    = 0x188,        // Interrupt 64-95 Clear Enable                         R/W     0x00000000
        IRQ_PEND0       = 0x200,        // Interrupt  0-31 Set Pending                          R/W     0x00000000
        IRQ_PEND1       = 0x204,        // Interrupt 32-63 Set Pending                          R/W     0x00000000
        IRQ_PEND2       = 0x208,        // Interrupt 64-95 Set Pending                          R/W     0x00000000
        IRQ_UNPEND0     = 0x280,        // Interrupt  0-31 Clear Pending                        R/W     0x00000000
        IRQ_UNPEND1     = 0x284,        // Interrupt 32-63 Clear Pending                        R/W     0x00000000
        IRQ_UNPEND2     = 0x288,        // Interrupt 64-95 Clear Pending                        R/W     0x00000000
        IRQ_ACTIVE0     = 0x300,        // Interrupt  0-31 Active Bit                           R/W     0x00000000
        IRQ_ACTIVE1     = 0x304,        // Interrupt 32-63 Active Bit                           R/W     0x00000000
        IRQ_ACTIVE2     = 0x308,        // Interrupt 64-95 Active Bit                           R/W     0x00000000
        SWTRIG          = 0xf00         // Software Trigger Interrupt Register                  WO      0x00000000
    };

public:
    void enable() {
        nvic(IRQ_ENABLE0) = ~0;
        if(IRQS > 32) nvic(IRQ_ENABLE1) = ~0;
        if(IRQS > 64) nvic(IRQ_ENABLE2) = ~0;
    }

    void enable(Interrupt_Id id) {
        if(id >= HARD_INT) {
            IRQ i = int2irq(id);
            assert(i < IRQS);
            if(i < 32) nvic(IRQ_ENABLE0) = 1 << i;
            else if((IRQS > 32) && (i < 64)) nvic(IRQ_ENABLE1) = 1 << (i - 32);
            else if(IRQS > 64) nvic(IRQ_ENABLE2) = 1 << (i - 64);
        }
    }

    void disable() {
        nvic(IRQ_DISABLE0) = ~0;
        if(IRQS > 32) nvic(IRQ_DISABLE1) = ~0;
        if(IRQS > 64) nvic(IRQ_DISABLE2) = ~0;
    }

    void disable(Interrupt_Id id) {
        if(id >= HARD_INT) {
            IRQ i = int2irq(id);
            assert(i < IRQS);
            if(i < 32) nvic(IRQ_DISABLE0) = 1 << i;
            else if((IRQS > 32) && (i < 64)) nvic(IRQ_DISABLE1) = 1 << (i - 32);
            else if(IRQS > 64) nvic(IRQ_DISABLE2) = 1 << (i - 64);
            unpend(i);
        }
    }

    int irq2int(int i) const { return i + HARD_INT; }
    int int2irq(int i) const { return i - HARD_INT; }

    void init() {};

    // Only works in handler mode (inside IC::entry())
    Interrupt_Id int_id() { return CPU::psr() & 0x3f; }

protected:
    void unpend() {
        nvic(IRQ_UNPEND0) = ~0;
        nvic(IRQ_UNPEND1) = ~0;
        nvic(IRQ_UNPEND2) = ~0;
    }

    void unpend(const IRQ & i) {
        assert(i < IRQS);
        if(i < 32) nvic(IRQ_UNPEND0) = 1 << i;
        else if((IRQS > 32) && (i < 64)) nvic(IRQ_UNPEND1) = 1 << (i - 32);
        else if(IRQS > 64) nvic(IRQ_UNPEND2) = 1 << (i - 64);
    }

private:
    volatile Reg32 & nvic (unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
