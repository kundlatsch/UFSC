// EPOS RISC-V Interrupt Controller Initialization

#include <machine/ic.h>
#include <machine/timer.h>

__BEGIN_SYS

// Class methods
void IC::init()
{
    db<Init, IC>(TRC) << "IC::init()" << endl;

    assert(CPU::int_disabled()); // will be reenabled at Thread::init() by Context::load()

    disable(); // will be enabled on demand as handlers are registered

    // Set all exception handlers to exception()
    for(Interrupt_Id i = 0; i < CPU::EXCEPTIONS; i++)
        _int_vector[i] = &exception;

    // Set all interrupt handlers to int_not()
    for(Interrupt_Id i = HARD_INT; i < INTS; i++)
        _int_vector[i] = int_not;
}

__END_SYS
