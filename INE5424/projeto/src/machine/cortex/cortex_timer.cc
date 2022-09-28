// EPOS Cortex Timer Mediator Implementation

#include <machine/machine.h>
#include <machine/timer.h>

__BEGIN_SYS

Timer * Timer::_channels[CHANNELS];

#ifdef __mmod_raspberry_pi3__
System_Timer_Engine::Count System_Timer_Engine::_count;
#endif

void Timer::int_handler(Interrupt_Id i)
{
    if(_channels[SCHEDULER] && (--_channels[SCHEDULER]->_current[CPU::id()] <= 0)) {
        _channels[SCHEDULER]->_current[CPU::id()] = _channels[SCHEDULER]->_initial;

        if(!Traits<Machine>::cpus_use_local_timer && (CPU::id() == 0))
            for(unsigned int cpu = 1; cpu < CPU::cores(); cpu++)
                IC::ipi(cpu, IC::INT_RESCHEDULER);

        _channels[SCHEDULER]->_handler(i);
    }

    if((!Traits<System>::multicore || (Traits<System>::multicore && (CPU::id() == 0))) && _channels[ALARM]) {
        _channels[ALARM]->_current[0] = _channels[ALARM]->_initial;
        _channels[ALARM]->_handler(i);
    }
}

__END_SYS
