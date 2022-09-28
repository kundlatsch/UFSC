// EPOS PC Interrupt Controller Mediator Declarations

#ifndef __pc_ic_h
#define __pc_ic_h

#include <architecture/cpu.h>
#include <machine/ic.h>
#include <system/memory_map.h>

__BEGIN_SYS

// Intel 8259A Interrupt Controller (master and slave are seen as a unit)
class i8259A
{
    friend class APIC;

private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;

    static const unsigned int IRQS = 16;
    static const unsigned int HARD_INT = 32;
    static const unsigned int SOFT_INT = HARD_INT + IRQS;

public:
    // I/O Ports
    enum {
        MASTER      = 0x20,
        MASTER_CMD  = MASTER,
        MASTER_DAT  = MASTER + 1,
        SLAVE       = 0xa0,
        SLAVE_CMD   = SLAVE,
        SLAVE_DAT   = SLAVE + 1
    };

    // Commands
    enum {
        SELECT_IRR  = 0x0a,
        SELECT_ISR  = 0x0b,
        ICW1        = 0x11, // flank, cascaded, more ICWs
        ICW4        = 0x01,
        EOI         = 0x20
    };

    // IRQs
    typedef unsigned int IRQ;
    enum {
        IRQ_TIMER       = 0,
        IRQ_KEYBOARD    = 1,
        IRQ_CASCADE     = 2,
        IRQ_SERIAL24    = 3,
        IRQ_SERIAL13    = 4,
        IRQ_PARALLEL23  = 5,
        IRQ_FLOPPY      = 6,
        IRQ_PARALLEL1   = 7,
        IRQ_RTC         = 8,
        IRQ_MOUSE       = 12,
        IRQ_MATH        = 13,
        IRQ_DISK1       = 14,
        IRQ_DISK2       = 15,
        IRQ_LAST        = IRQ_DISK2
    };

    // Interrupts
    enum {
        INT_FIRST_HARD  = HARD_INT,
        INT_TIMER       = HARD_INT + IRQ_TIMER,
        INT_KEYBOARD    = HARD_INT + IRQ_KEYBOARD,
        INT_IPI         = HARD_INT + IRQ_LAST + 1, // in multicores, IPIs must be acknowledged just like other hardware interrupts
        INT_LAST_HARD   = INT_IPI,
        INT_FIRST_SOFT
    };

public:
    i8259A() {}

    static int irq2int(int i) { return i + HARD_INT; }
    static int int2irq(int i) { return i - HARD_INT; }

    static void enable() { imr(1 << IRQ_CASCADE); }
    static void enable(int i) { imr(imr() & ~(1 << int2irq(i))); }
    static void disable() { imr(~(1 << IRQ_CASCADE)); }
    static void disable(int i) { imr(imr() | (1 << int2irq(i))); }

    static void remap(Reg8 base = HARD_INT) {
        Reg8 m_imr = CPU::in8(MASTER_DAT);      // save IMRs
        Reg8 s_imr = CPU::in8(SLAVE_DAT);

        // Configure Master PIC
        CPU::out8(MASTER_CMD, ICW1);
        CPU::out8(MASTER_DAT, base);            // ICW2 is the base
        CPU::out8(MASTER_DAT, 1 << IRQ_CASCADE);// ICW3 = IRQ2 cascaded
        CPU::out8(MASTER_DAT, ICW4);

        // Configure Slave PIC
        CPU::out8(SLAVE_CMD, ICW1);
        CPU::out8(SLAVE_DAT, base + 8);         // ICW2 is the base
        CPU::out8(SLAVE_DAT, 0x02);             // ICW3 = cascaded from IRQ1
        CPU::out8(SLAVE_DAT, ICW4);

        CPU::out8(MASTER_DAT, m_imr);           // restore saved IMRs
        CPU::out8(SLAVE_DAT, s_imr);
    }

    static void reset() { remap(); disable(); }

    static Reg16 irr() { // Pending interrupts
        CPU::out8(MASTER_CMD, SELECT_IRR);
        CPU::out8(SLAVE_CMD, SELECT_IRR);
        return CPU::in8(MASTER_CMD) | (CPU::in8(SLAVE_CMD) << 8);
    }

    static Reg16 isr() { // In-service interrupts
        CPU::out8(MASTER_CMD, SELECT_ISR);
        CPU::out8(SLAVE_CMD, SELECT_ISR);
        return CPU::in8(MASTER_CMD) | (CPU::in8(SLAVE_CMD) << 8);
    }

    static Reg16 imr() { // Interrupt mask
        return CPU::in8(MASTER_DAT) | (CPU::in8(SLAVE_DAT) << 8);
    }

    static void imr(Reg16 mask) {
        CPU::out8(MASTER_DAT, mask & 0xff);
        CPU::out8(SLAVE_DAT, mask >> 8);
    }

    static bool eoi(unsigned int i) {
        int irq = int2irq(i);

        if(!(isr() & (1 << irq))) {           // spurious interrupt?
            if(isr() & (1 << IRQ_CASCADE))    // cascade?
                CPU::out8(MASTER_CMD, EOI);   // send EOI to master
            return false;
        }

        if(irq >= 8)                    // slave interrupt?
            CPU::out8(SLAVE_CMD, EOI);  // send EOI to slave
        CPU::out8(MASTER_CMD, EOI);     // always send EOI to master

        return true;
    }

    static void ipi(int dest, int interrupt) {}
};

// Intel IA-32 APIC (internal, not tested with 82489DX)
class APIC
{
private:
    typedef CPU::Reg8 Reg8;
    typedef CPU::Reg16 Reg16;
    typedef CPU::Reg Reg;
    typedef CPU::Reg64 Reg64;
    typedef CPU::Log_Addr Log_Addr;

    static const unsigned int HARD_INT = i8259A::HARD_INT;
    static const unsigned int SOFT_INT = i8259A::HARD_INT;

public:
    // Interrupts
    enum {
        INT_FIRST_HARD  = i8259A::INT_FIRST_HARD,
        INT_TIMER       = i8259A::INT_TIMER,
        INT_KEYBOARD    = i8259A::INT_KEYBOARD,
        INT_IPI         = i8259A::INT_IPI,
        INT_LAST_HARD   = i8259A::INT_LAST_HARD,
        INT_FIRST_SOFT  = i8259A::INT_FIRST_SOFT
    };

    // Default mapping addresses
    enum {
        LOCAL_APIC_PHY_ADDR = 0xfee00000,
        LOCAL_APIC_LOG_ADDR = Memory_Map::APIC,
        LOCAL_APIC_SIZE     = Memory_Map::IO_APIC - Memory_Map::APIC,
        IO_APIC_PHY_ADDR    = 0xfec00000,
        IO_APIC_LOG_ADDR    = Memory_Map::IO_APIC,
        IO_APIC_SIZE        = Memory_Map::VGA - Memory_Map::IO_APIC
    };

    // Memory-mapped registers
    // Values added with _base (originaly 0xfee00000). See Figure 10-1 of Intel Arch Dev Manual Vol 3A, 2014. Pg 354 PDF.
    enum {
        ID            = 0x020,  // Task priority
        VERSION       = 0x030,  // Task priority
        TPR           = 0x080,  // Task priority
        APR           = 0x090,  // Arbitration priority
        PPR           = 0x0a0,  // Processor priority
        EOI           = 0x0b0,  // End of interrupt
        RRR           = 0x0c0,  // Remote read
        LDR           = 0x0d0,  // Logical destination
        DFR           = 0x0e0,  // Destination format
        SVR           = 0x0f0,  // Spurious interrupt vector
        ISR0_31       = 0x100,  // In-service
        ISR32_63      = 0x110,  // In-service
        ISR64_95      = 0x120,  // In-service
        ISR96_127     = 0x130,  // In-service
        ISR128_159    = 0x140,  // In-service
        ISR160_191    = 0x150,  // In-service
        ISR192_223    = 0x160,  // In-service
        ISR224_255    = 0x170,  // In-service
        TMR0_31       = 0x180,  // Trigger mode
        TMR32_63      = 0x190,  // Trigger mode
        TMR64_95      = 0x1a0,  // Trigger mode
        TMR96_127     = 0x1b0,  // Trigger mode
        TMR128_159    = 0x1c0,  // Trigger mode
        TMR160_191    = 0x1d0,  // Trigger mode
        TMR192_223    = 0x1e0,  // Trigger mode
        TMR224_255    = 0x1f0,  // Trigger mode
        IRR0_31       = 0x200,  // Interrupt request
        IRR32_63      = 0x210,  // Interrupt request
        IRR64_95      = 0x220,  // Interrupt request
        IRR96_127     = 0x230,  // Interrupt request
        IRR128_159    = 0x240,  // Interrupt request
        IRR160_191    = 0x250,  // Interrupt request
        IRR192_223    = 0x260,  // Interrupt request
        IRR224_255    = 0x270,  // Interrupt request
        ESR           = 0x280,  // Error status
        LVT_CMCI      = 0x2f0,  // LVT CMCI
        ICR0_31       = 0x300,  // Interrupt command
        ICR32_63      = 0x310,  // Interrupt command
        LVT_TIMER     = 0x320,  // LVT timer
        LVT_THERMAL   = 0x330,  // LVT thermal sensor
        LVT_PERF      = 0x340,  // LVT performance monitor
        LVT_LINT0     = 0x350,  // LVT interrupt line 0
        LVT_LINT1     = 0x360,  // LVT interrupt line 1
        LVT_ERROR     = 0x370,  // LVT error
        TIMER_INITIAL = 0x380,  // Timer's initial count
        TIMER_CURRENT = 0x390,  // Timer's current count
        TIMER_PRESCALE = 0x3e0   // Timer's BUS CLOCK prescaler
    };

    // MSR registers
    enum {
        IA32_APIC_BASE_MSR = 0x1b
    };

    // MSR and masks
    enum {
        BSP_MASK = 0x100
    };

    // Flags
    enum {
        // Local APIC ID Register
        ID_SHIFT                = 24,
        ID_MASK                 = 0xff000000,

        // Spurious Interrupt Vector Register
        SVR_VECTOR              = 0x000000ff,
        SVR_APIC_ENABLED        = (1 << 8),
        SVR_FOCUS_DISABLED      = (1 << 9),

        // Error Status Register
        ESR_SEND_CHECK          = (1 << 0),
        ESR_RECV_CHECK          = (1 << 1),
        ESR_SEND_ACCEPT         = (1 << 2),
        ESR_RECV_ACCEPT         = (1 << 3),
        ESR_SEND_ILLEGAL_VECTOR = (1 << 5),
        ESR_RECV_ILLEGAL_VECTOR = (1 << 6),
        ESR_ILLEGAL_REGISTER    = (1 << 7),

        // Interrupt Command Register (64 bits)
        ICR_DEST                = (3 << 18),
        ICR_SELF                = (1 << 18),
        ICR_ALL                 = (1 << 19),
        ICR_OTHERS              = (3 << 18),
        ICR_TRIGMOD             = (1 << 15),
        ICR_EDGE                = (0 << 15),
        ICR_LEVEL               = (1 << 15),
        ICR_ASSERTED            = (1 << 14),
        ICR_DEASSERT            = (0 << 14),
        ICR_ASSERT              = (1 << 14),
        ICR_STATUS              = (1 << 12),
        ICR_IDLE                = (0 << 12),
        ICR_PENDING             = (1 << 12),
        ICR_DESTMODE            = (1 << 11),
        ICR_PHY                 = (0 << 11),
        ICR_LOG                 = (1 << 11),
        ICR_DELMODE             = (7 <<  8),
        ICR_FIXED               = (0 <<  8),
        ICR_LOWPRI              = (1 <<  8),
        ICR_SMI                 = (2 <<  8),
        ICR_NMI                 = (4 <<  8),
        ICR_INIT                = (5 <<  8),
        ICR_STARTUP             = (6 <<  8),

        // Local Vector Table
        LVT_MASKED              = (1 << 16),
        LVT_EDGE                = (0 << 15),
        LVT_LEVEL               = (1 << 15),
        LVT_FIXED               = (0 << 8),
        LVT_SMI                 = (2 << 8),
        LVT_NMI                 = (4 << 8),
        LVT_EXTINT              = (7 << 8),
        LVT_INIT                = (5 << 8),

        // Local Timer (32 bits)
        TIMER_PERIODIC          = (1 << 17),
        TIMER_ONE_SHOT          = (0 << 17),
        TIMER_MASKED            = LVT_MASKED,
        TIMER_PRESCALE_BY_1     = 0xb,
        TIMER_PRESCALE_BY_2     = 0x0,
        TIMER_PRESCALE_BY_4     = 0x8,
        TIMER_PRESCALE_BY_8     = 0x2,
        TIMER_PRESCALE_BY_16    = 0xa,
        TIMER_PRESCALE_BY_32    = 0x1,
        TIMER_PRESCALE_BY_64    = 0x9,
        TIMER_PRESCALE_BY_128   = 0x3,
    };

public:
    APIC() {}

    static int irq2int(int i) { return i + HARD_INT; }
    static int int2irq(int i) { return i - HARD_INT; }

    static void remap(Log_Addr addr = LOCAL_APIC_LOG_ADDR) {
        _base = addr;
    }

    static void enable() {
        Reg v = read(SVR);
        v |= SVR_APIC_ENABLED;
        write(SVR, v);
    }
    static void enable(int i) { enable(); }

    static void disable() {
        Reg v  = read(SVR);
        v &= ~SVR_APIC_ENABLED;
        write(SVR, v);
    }
    static void disable(int i) { disable(); }

    static Reg read(unsigned int reg) {
        return *static_cast<volatile Reg *>(_base + reg);
    }
    static void write(unsigned int reg, Reg v) {
        *static_cast<volatile Reg *>(_base + reg) = v;
    }

    static volatile unsigned int id() {
        return (read(ID) & ID_MASK) >> ID_SHIFT;
    }
    static int version() {
        return read(VERSION);
    }

    static void ipi(unsigned int cpu, unsigned int interrupt);
    static void ipi_init(volatile int * status);
    static void ipi_start(Log_Addr entry, volatile int * status);

    static void reset(Log_Addr addr = LOCAL_APIC_LOG_ADDR) {
        // APIC must be on very early in the boot process, so it is
        // subject to memory remappings. We also cannot be sure about
        // global constructors here
        remap(addr);
        if(Traits<System>::multicore) {
            clear();
            enable();
            connect();
        } else
            disable();
    }

    static int eoi(unsigned int i) { // End of interrupt
        write(APIC::EOI, 0);
        return true;
    }

    static void config_timer(Reg count, bool interrupt, bool periodic) {
        Reg v = INT_TIMER;
        v |= (interrupt) ? 0 : TIMER_MASKED;
        v |= (periodic) ? TIMER_PERIODIC : 0;
        write(TIMER_INITIAL, count / 16);
        write(TIMER_PRESCALE, TIMER_PRESCALE_BY_16);
        reset_timer();
        write(LVT_TIMER, v);
    }

    static void enable_timer() {
        write(LVT_TIMER, read(LVT_TIMER) & ~TIMER_MASKED);
    }
    static void disable_timer() {
        write(LVT_TIMER, read(LVT_TIMER) | TIMER_MASKED);
    }

    static Reg read_timer() {
        return read(TIMER_CURRENT);
    }

    static void reset_timer() {
        disable();
        write(TIMER_CURRENT, read(TIMER_INITIAL));
        enable();
    }

    static void config_pmu(const Reg & i) {
        Reg v = i;
        write(LVT_PERF, v);
    }

    static void enable_pmu() {
    	write(LVT_PERF, read(LVT_PERF) & ~LVT_MASKED);
    }
    static void disable_pmu() {
    	write(LVT_PERF, read(LVT_PERF) | LVT_MASKED);
    }

private:
    static int maxlvt() {
        Reg v = read(VERSION);
        // 82489DXs do not report # of LVT entries
        return (v & 0xf) ? (v >> 16) & 0xff : 2;
    }

    static void clear() {
        int lvts = maxlvt();

        // Masking an LVT entry on a P6 can trigger a local APIC error
        // if the vector is zero. Mask LVTERR first to prevent this
        if(lvts >= 3)
            write(LVT_ERROR, 1 | LVT_MASKED); // any non-zero vector

        // Careful: we have to set masks only first to deassert
        // any level-triggered sources
        write(LVT_TIMER, read(LVT_TIMER) | LVT_MASKED);
        write(LVT_LINT0, read(LVT_LINT0) | LVT_MASKED);
        write(LVT_LINT1, read(LVT_LINT1) | LVT_MASKED);
        if(lvts >= 4) {
            write(LVT_PERF, read(LVT_PERF) | LVT_MASKED);
            write(LVT_THERMAL, read(LVT_THERMAL) | LVT_MASKED);
        }

        // Clean APIC state
        write(LVT_TIMER, LVT_MASKED);
        write(LVT_LINT0, LVT_MASKED);
        write(LVT_LINT1, LVT_MASKED);
        if(lvts >= 3)
            write(LVT_ERROR, LVT_MASKED);
        if(lvts >= 4) {
            write(LVT_PERF, LVT_MASKED);
            write(LVT_THERMAL, LVT_MASKED);
        }
        if(read(VERSION) & 0xf) { // !82489DX
            if(lvts > 3)
                write(ESR, 0);
            read(ESR);
        }
    }

    static void connect() {
        CPU::out8(0x22, 0x70);
        CPU::out8(0x23, 0x01);
    }

    static void disconnect() {
        CPU::out8(0x22, 0x70);
        CPU::out8(0x23, 0x00);
    }

private:
    static Log_Addr _base;
};

// IC uses i8259A on single-processor machines and the APIC timer on MPs
class IC: private IC_Common, private IF<Traits<System>::multicore, APIC, i8259A>::Result
{
    friend class Machine;
    friend class Thread;

private:
    typedef IF<Traits<System>::multicore, APIC, i8259A>::Result Engine;

    typedef CPU::Reg Reg;
    typedef CPU::Log_Addr Log_Addr;

public:
    using IC_Common::Interrupt_Id;
    using IC_Common::Interrupt_Handler;

    enum {
        INT_FIRST_HARD  = Engine::INT_FIRST_HARD,
        INT_SYS_TIMER   = Engine::INT_TIMER,
        INT_KEYBOARD    = Engine::INT_KEYBOARD,
        INT_LAST_HARD   = Engine::INT_LAST_HARD,
        INT_RESCHEDULER = Engine::INT_IPI,
        INT_SYSCALL     = Engine::INT_FIRST_SOFT,
        INT_PMU,
        LAST_INT
    };

    static const unsigned int INTS = LAST_INT;

public:
    IC() {}

    static Interrupt_Handler int_vector(Interrupt_Id i) {
        assert(i < INTS);
        return _int_vector[i];
    }

    static void int_vector(Interrupt_Id i, const Interrupt_Handler & h) {
        db<IC>(TRC) << "IC::int_vector(int=" << i << ",h=" << reinterpret_cast<void *>(h) <<")" << endl;
        assert(i < INTS);
        _int_vector[i] = h;
    }

    static void enable() {
        db<IC>(TRC) << "IC::enable()" << endl;
        Engine::enable();
    }

    static void enable(Interrupt_Id i) {
        db<IC>(TRC) << "IC::enable(int=" << i << ")" << endl;
        assert(i < INTS);
        Engine::enable(i);
    }

    static void disable() {
        db<IC>(TRC) << "IC::disable()" << endl;
        Engine::disable();
    }

    static void disable(Interrupt_Id i) {
        db<IC>(TRC) << "IC::disable(int=" << i << ")" << endl;
        assert(i < INTS);
        Engine::disable(i);
    }

    using Engine::ipi;
    using Engine::irq2int;

private:
    static void dispatch(unsigned int i);

    // Logical handlers
    static void int_not(Interrupt_Id i);

    // Physical handlers
    static void entry();
    static void exc_not(Reg eip, Reg cs, Reg eflags, Reg error);
    static void exc_pf (Reg eip, Reg cs, Reg eflags, Reg error);
    static void exc_gpf(Reg eip, Reg cs, Reg eflags, Reg error);
    static void exc_fpu(Reg eip, Reg cs, Reg eflags, Reg error);

    static void init();

private:
    static Interrupt_Handler _int_vector[INTS];
};

// Core id in IA32 is handled by the APIC
inline volatile unsigned int CPU::id()
{
    return smp ? APIC::id() : 0;
}

__END_SYS

#endif
