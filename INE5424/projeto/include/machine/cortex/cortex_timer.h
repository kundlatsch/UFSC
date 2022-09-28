// EPOS ARM Cortex Timer Mediator Declarations

#ifndef __cortex_timer_h
#define __cortex_timer_h

#include <architecture/cpu.h>
#include <machine/ic.h>
#include <machine/timer.h>
#include __HEADER_MMOD(timer)
#include <utility/convert.h>

__BEGIN_SYS

// Tick timer used by the system
class Timer: public System_Timer_Engine
{
    friend Machine;
    friend IC;
    friend class Init_System;

protected:
    static const unsigned int CHANNELS = 2;
    static const unsigned int FREQUENCY = Traits<Timer>::FREQUENCY;

    typedef System_Timer_Engine Engine;
    typedef IC_Common::Interrupt_Id Interrupt_Id;

protected:
    Timer(Channel channel, const Hertz & frequency, const Handler & handler, bool retrigger = true)
    : _channel(channel), _initial(FREQUENCY / frequency), _retrigger(retrigger), _handler(handler) {
        db<Timer>(TRC) << "Timer(f=" << frequency << ",h=" << reinterpret_cast<void*>(handler) << ",ch=" << channel << ") => {count=" << _initial << "}" << endl;

        if(_initial && (channel < CHANNELS) && !_channels[channel])
            _channels[channel] = this;
        else
            db<Timer>(WRN) << "Timer not installed!"<< endl;

        for(unsigned int i = 0; i < Traits<Machine>::CPUS; i++)
            _current[i] = _initial;
    }

public:
    ~Timer() {
        db<Timer>(TRC) << "~Timer(f=" << frequency() << ",h=" << reinterpret_cast<void*>(_handler) << ",ch=" << _channel << ") => {count=" << _initial << "}" << endl;

        _channels[_channel] = 0;
    }

    Tick read() { return _current[CPU::id()]; }

    static void reset() { db<Timer>(TRC) << "Timer::reset()" << endl; Engine::reset(); }
    static void enable() { db<Timer>(TRC) << "Timer::enable()" << endl; Engine::enable(); }
    static void disable() { db<Timer>(TRC) << "Timer::disable()" << endl; Engine::disable(); }

    PPB accuracy();
    Hertz frequency() const { return (FREQUENCY / _initial); }
    void frequency(Hertz f) { _initial = FREQUENCY / f; reset(); }

    void handler(const Handler & handler) { _handler = handler; }

private:
    static void int_handler(Interrupt_Id i);
    static void eoi(Interrupt_Id int_id) { Engine::eoi(int_id); }

    static void init();

protected:
    unsigned int _channel;
    Tick _initial;
    bool _retrigger;
    volatile Tick _current[Traits<Build>::CPUS];
    Handler _handler;

    static Timer * _channels[CHANNELS];
};


// Timer used by Thread::Scheduler
class Scheduler_Timer: public Timer
{
public:
    Scheduler_Timer(Microsecond quantum, const Handler & handler): Timer(SCHEDULER, 1000000 / quantum, handler) {}

    int restart() {
        db<Timer>(TRC) << "Timer::restart() => {f=" << frequency() << ",h=" << reinterpret_cast<void *>(_handler) << ",count=" << _current[CPU::id()] << "}" << endl;

        int percentage = _current[CPU::id()] * 100 / _initial;
        _current[CPU::id()] = _initial;

        return percentage;
    }
};

// Timer used by Alarm
class Alarm_Timer: public Timer
{
public:
    static const unsigned int FREQUENCY = Timer::FREQUENCY;

public:
    Alarm_Timer(const Handler & handler): Timer(ALARM, FREQUENCY, handler) {}
};


// Timer available for users
class User_Timer: private User_Timer_Engine
{
    friend class PWM;

private:
    typedef User_Timer_Engine Engine;
    typedef Engine::Count Count;
    typedef IC_Common::Interrupt_Id Interrupt_Id;

    static const unsigned int UNITS = Traits<Timer>::UNITS - Traits<TSC>::enabled; // TSC uses the last timer. To use the it, you must disable the TSC.

public:
    using Timer_Common::Handler;

public:
    User_Timer(Channel channel, Microsecond time, const Handler & handler, bool periodic = false)
    : Engine(channel, time, handler ? true : false, periodic), _channel(channel), _handler(handler) {
        assert(channel < UNITS);
        if(_handler) {
            IC::Interrupt_Id id = _channel == 0 ? IC::INT_USER_TIMER0 : _channel == 1 ? IC::INT_USER_TIMER1 :
                                  _channel == 2 ? IC::INT_USER_TIMER2 : IC::INT_USER_TIMER3;
            IC::int_vector(id, _handler);
            IC::enable(id);
        }
    }
    
    ~User_Timer() {
        if(_handler) {
            IC::Interrupt_Id id = _channel == 0 ? IC::INT_USER_TIMER0 : _channel == 1 ? IC::INT_USER_TIMER1 :
                                  _channel == 2 ? IC::INT_USER_TIMER2 : IC::INT_USER_TIMER3;
            IC::disable(id);
        }
    }

    using Engine::read;

    using Engine::enable;
    using Engine::disable;

    void power(const Power_Mode & mode);

    static void eoi(Interrupt_Id int_id) { Engine::eoi(int_id); }

private:
    unsigned int _channel;
    Handler _handler;
};

__END_SYS

#endif
