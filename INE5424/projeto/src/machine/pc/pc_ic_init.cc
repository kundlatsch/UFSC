// EPOS PC Interrupt Controller Initialization

#include <architecture/cpu.h>
#include <machine/ic.h>

__BEGIN_SYS

void IC::init()
{
    db<Init, IC>(TRC) << "IC::init()" << endl;

    CPU::int_disable();

    // Set all IDT entries to proper int_dispatch() offsets
    CPU::IDT_Entry * idt = reinterpret_cast<CPU::IDT_Entry *>(Memory_Map::IDT);
    for(unsigned int i = 0; i < CPU::IDT_ENTRIES; i++)
        if(i < INTS)
            idt[i] = CPU::IDT_Entry(CPU::SEL_SYS_CODE, Log_Addr(entry) + i * 16, CPU::SEG_IDT_ENTRY);
        else
            idt[i] = CPU::IDT_Entry(CPU::SEL_SYS_CODE, Log_Addr(entry) + CPU::EXC_LAST * 16, CPU::SEG_IDT_ENTRY);

    // Install some important exception handlers
    idt[CPU::EXC_PF]     = CPU::IDT_Entry(CPU::SEL_SYS_CODE, Log_Addr(&exc_pf),  CPU::SEG_IDT_ENTRY);
    idt[CPU::EXC_DOUBLE] = CPU::IDT_Entry(CPU::SEL_SYS_CODE, Log_Addr(&exc_pf),  CPU::SEG_IDT_ENTRY);
    idt[CPU::EXC_GPF]    = CPU::IDT_Entry(CPU::SEL_SYS_CODE, Log_Addr(&exc_gpf), CPU::SEG_IDT_ENTRY);
    idt[CPU::EXC_NODEV]  = CPU::IDT_Entry(CPU::SEL_SYS_CODE, Log_Addr(&exc_fpu), CPU::SEG_IDT_ENTRY);

    // Install the syscall trap handler
    if(Traits<Build>::MODE == Traits<Build>::KERNEL)
        idt[INT_SYSCALL] = CPU::IDT_Entry(CPU::SEL_SYS_CODE, Log_Addr(&CPU::syscalled), CPU::SEG_IDT_ENTRY);

    // Set all interrupt handlers to int_not()
    for(unsigned int i = 0; i < INTS; i++)
 	_int_vector[i] = int_not;

    remap();
    disable();

    CPU::int_enable();
}

__END_SYS
