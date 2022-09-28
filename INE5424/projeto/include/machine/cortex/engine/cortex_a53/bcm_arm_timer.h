// EPOS ARM Cortex-A53 ARM Timer Mediator Declarations

#ifndef __arm_timer_h
#define __arm_timer_h

#include <architecture/cpu.h>
#define __common_only__
#include <machine/rtc.h>
#include <machine/timer.h>
#undef __common_only__

__BEGIN_SYS

class ARM_Timer : public Timer_Common
{
    // This is a hardware object
    // Use with something like "new (Memory_Map::TIMER1_BASE) ARM_Timer"

private:
    typedef CPU::Reg32 Reg32;
    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef CPU::Reg64 Count;

    static const unsigned int CLOCK = 250000000 / 250; // 250 MHz / PRE_DIV (0xFA == 250)

    // Usefull Off-set Register from TIMER1_BASE
    enum {
        LOAD    = 0x000, // Load the content to VALUE when VALUE goes to 0 or when this register is written
        VALUE   = 0X004, // Read-Only, Count Down Value
        CONTROL = 0X008, // Timer control operations (enable, enable interruptions...)
        IRQ_CLR = 0x00c, // EOI
        RAW_IRQ = 0X010, // 1 if pendding INT
        MSK_IRQ = 0X014, // 1 if pendding INT and INT enable
        RELOAD  = 0X018, // Copy of LOAD, writing in this register does not reset VALUE Countdown
        PRE_DIV = 0X01c, // timer_clock = apb_clock/(pre_divider+1) - The reset value of this register is 0x7D so gives a divide by 126.
        CNTR    = 0x020  // Free Counter | Read Only
    };

    // CONTROL USEFULL BITS
    enum { // Reset = 3E0020
        CNTR_SIZE   = 1    << 1, // 1 == 23 bit counter, 0 == 16 bit counter
        PRE_SCALE   = 0x00 << 2, // 00 == Clock/1 | 01 == Clock/16 | 10 == Clock/256 | 11 == Clock/1 (?)
        INT_EN      = 1    << 5,
        TIMER_EN    = 1    << 7,
        FREE_CNTR   = 1    << 9,
        FREE_CLOCK  = 0x3e << 16
    };

public:
    //b2f8
    void config(unsigned int unit, const Count & count) {
        timer(CONTROL) = FREE_CLOCK;
        timer(RELOAD) = count;
        timer(LOAD) = count;
        timer(PRE_DIV) = 0xFA;
        timer(IRQ_CLR) = 0;
        timer(CONTROL) = FREE_CLOCK | TIMER_EN | INT_EN | PRE_SCALE | CNTR_SIZE; //0x3e00a2
    }

    Count count() {
        return static_cast<Count>(timer(CNTR));
    }

    void eoi() {
        timer(IRQ_CLR) = 0;
        /* ASM("dsb \t\n isb"); */
        while (timer(RAW_IRQ));
    }

    void enable() {
    }

    void disable() {
    }

    void set(const Count & count) {
        timer(RELOAD) = count;
    }

    Hertz clock() { return CLOCK; }

private:
    volatile Reg32 & timer(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
