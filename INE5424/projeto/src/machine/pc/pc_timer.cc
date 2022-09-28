// EPOS PC Timer Mediator Implementation

#include <machine/ic.h>
#include <machine/timer.h>

__BEGIN_SYS

// Class attributes
Timer * Timer::_channels[CHANNELS];

// Class methods
void Timer::int_handler(Interrupt_Id i)
{
    if((!Traits<System>::multicore || (Traits<System>::multicore && (CPU::id() == 0))) && _channels[ALARM]) {
        _channels[ALARM]->_current[0] = _channels[ALARM]->_initial;
        _channels[ALARM]->_handler(i);
    }

    if((!Traits<System>::multicore || (Traits<System>::multicore && (CPU::id() == 0))) && _channels[USER]) {
        if(_channels[USER]->_retrigger)
            _channels[USER]->_current[0] = _channels[USER]->_initial;
        _channels[USER]->_handler(i);
    }

    if(_channels[SCHEDULER] && (--_channels[SCHEDULER]->_current[CPU::id()] <= 0)) {
        _channels[SCHEDULER]->_current[CPU::id()] = _channels[SCHEDULER]->_initial;
        _channels[SCHEDULER]->_handler(i);
    }
}

__END_SYS
