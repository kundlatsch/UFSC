// EPOS Cortex-M Interrupt Controller Initialization

#include <architecture/cpu.h>
#include <architecture/tsc.h>
#include <machine/ic.h>

__BEGIN_SYS

void IC::init()
{
    db<Init, IC>(TRC) << "IC::init()" << endl;

    CPU::int_disable(); // will be reenabled at Thread::init()
    Engine::init();

    disable(); // will be enabled on demand as handlers are registered

    // Set all interrupt handlers to int_not()
    for(Interrupt_Id i = 0; i < INTS; i++)
        _int_vector[i] = int_not;

    _int_vector[IC::INT_HARD_FAULT] = hard_fault;

    // TSC is initialized before IC, so we register its interrupt now
    if(Traits<TSC>::enabled) {
        static const Interrupt_Id int_id =
                Traits<Timer>::UNITS == 1 ? INT_USER_TIMER0
              : Traits<Timer>::UNITS == 2 ? INT_USER_TIMER1
              : Traits<Timer>::UNITS == 3 ? INT_USER_TIMER2
                                          : INT_USER_TIMER3;

        int_vector(int_id, TSC::int_handler);
        enable(int_id);
    }
}

__END_SYS
