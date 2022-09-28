// EPOS RISC-V IC Mediator Declarations

#ifndef __riscv_ic_h
#define __riscv_ic_h

#include <architecture/cpu.h>
#include <machine/ic.h>
#include <system/memory_map.h>

__BEGIN_SYS

// Core Local Interrupter (CLINT)
class CLINT
{
private:
    typedef CPU::Reg Reg;
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::Log_Addr Log_Addr;

public:
    static const unsigned int IRQS = 16;

    // Interrupts (mcause with interrupt = 1)
    enum : unsigned int {
        IRQ_USR_SOFT            = 0,
        IRQ_SUP_SOFT            = 1,
        IRQ_MAC_SOFT            = 3,
        IRQ_USR_TIMER           = 4,
        IRQ_SUP_TIMER           = 5,
        IRQ_MAC_TIMER           = 7,
        IRQ_USR_EXT             = 8,
        IRQ_SUP_EXT             = 9,
        IRQ_MAC_EXT             = 11,
        INTERRUPT               = 1UL << 31,
        INT_MASK                = ~INTERRUPT

    };

    // Registers offsets from CLINT_BASE
    enum {                                // Description
        MSIP                    = 0x0000, // Generate machine mode software interrupts (IPIs); each HART is offseted by 4 bytes from MSIP
        MTIMECMP                = 0x4000, // Compare (32-bit, per hart register)
        MTIME                   = 0xbff8, // Counter (lower 32 bits, shared by all harts)
        MTIMEH                  = 0xbffc, // Counter (upper 32 bits, shared by all harts)
        MSIP_CORE_OFFSET        = 4,      // Offset in bytes from MSIP for each hart's software interrupt trigger register
        MTIMECMP_CORE_OFFSET    = 8       // Offset in bytes from MTIMECMP for each hart's compare register
    };

    // MTVEC modes
    enum Mode {
        DIRECT  = 0,
        INDEXED = 1
    };

public:
    static void mtvec(Mode mode, Phy_Addr base) {
    	Reg tmp = (base & 0xfffffffc) | (Reg(mode) & 0x3);
        ASM("csrw mtvec, %0" : : "r"(tmp) : "cc");
    }

    static Reg mtvec() {
        Reg value;
        ASM("csrr %0, mtvec" : "=r"(value) : : );
        return value;
    }

    static void stvec(Mode mode, Log_Addr base) {
    	Reg tmp = (base & 0xfffffffc) | (Reg(mode) & 0x3);
        ASM("csrw stvec, %0" : : "r"(tmp) : "cc");
    }

    static Reg stvec() {
        Reg value;
        ASM("csrr %0, stvec" : "=r"(value) : : );
        return value;
    }
};

class IC: private IC_Common, private CLINT
{
    friend class Setup;
    friend class Machine;

private:
    typedef CPU::Reg Reg;

    static const unsigned int INTS = CPU::EXCEPTIONS + IRQS;
    static const bool multitask = Traits<System>::multitask;

public:
    using IC_Common::Interrupt_Id;
    using IC_Common::Interrupt_Handler;

    enum {
        HARD_INT        = CPU::EXCEPTIONS,
        INT_SYS_TIMER   = HARD_INT + (multitask ? IRQ_SUP_TIMER : IRQ_MAC_TIMER),
        INT_RESCHEDULER = HARD_INT + (multitask ? IRQ_SUP_SOFT : IRQ_MAC_SOFT),  // an IPI is mapped to the machine with mcause set to IRQ_MAC_SOFT
    };

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
        if(multitask)
            CPU::sie(CPU::SSI | CPU::STI | CPU::SEI);
        else
            CPU::mie(CPU::MSI | CPU::MTI | CPU::MEI);
    }

    static void enable(Interrupt_Id i) {
        db<IC>(TRC) << "IC::enable(int=" << i << ")" << endl;
        assert(i < INTS);
        enable();
        // TODO: this should handle individual INTs and also be done at PLIC
    }

    static void disable() {
        db<IC>(TRC) << "IC::disable()" << endl;
        if(multitask)
            CPU::siec(CPU::SSI | CPU::STI | CPU::SEI);
        else
            CPU::miec(CPU::MSI | CPU::MTI | CPU::MEI);
}

    static void disable(Interrupt_Id i) {
        db<IC>(TRC) << "IC::disable(int=" << i << ")" << endl;
        assert(i < INTS);
        disable();
        // TODO: this should handle individual INTs and also be done at PLIC
    }

    static Interrupt_Id int_id() {
        // Id is retrieved from [m|s]cause even if mip has the equivalent bit up, because only [m|s]cause can tell if it is an interrupt or an exception
        Reg id = (multitask) ? CPU::scause() : CPU::mcause();
        if(id & INTERRUPT)
            return (id & INT_MASK) + HARD_INT;
        else
            return (id & INT_MASK);
    }

    static int irq2int(int i) { return i + HARD_INT; }

    static int int2irq(int i) { return i - HARD_INT; }

    static void ipi(unsigned int cpu, Interrupt_Id i) {
        db<IC>(TRC) << "IC::ipi(cpu=" << cpu << ",int=" << i << ")" << endl;
        assert(i < INTS);
        reg(MSIP + cpu * MSIP_CORE_OFFSET) = 1;
    }

    static void ipi_eoi(Interrupt_Id i) { reg(MSIP + CPU::id() * MSIP_CORE_OFFSET) = 0; }

private:
    static void dispatch();

    // Logical handlers
    static void int_not(Interrupt_Id i);
    static void exception(Interrupt_Id i);

    // Physical handler
    static void entry() __attribute((naked, aligned(4)));

    static void init();

    static volatile CPU::Reg32 & reg(unsigned int o) { return reinterpret_cast<volatile CPU::Reg32 *>(Memory_Map::CLINT_BASE)[o / sizeof(CPU::Reg32)]; }

private:
    static Interrupt_Handler _int_vector[INTS];
};

__END_SYS

#endif
