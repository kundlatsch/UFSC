// EPOS Cortex Timer Mediator Initialization

#include <machine/timer.h>
#include <machine/ic.h>

__BEGIN_SYS

void Timer::init()
{
    db<Init, Timer>(TRC) << "Timer::init()" << endl;

    IC::int_vector(IC::INT_SYS_TIMER, int_handler);
    IC::enable(IC::INT_SYS_TIMER);
    Engine::init();
}

__END_SYS
