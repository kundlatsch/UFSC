// EPOS Raspberry Pi3 (ARM Cortex-A53) Timer Mediator Declarations

#ifndef __raspberry_pi3_timer_h
#define __raspberry_pi3_timer_h

#define __ic_common_only__
#include <machine/ic.h>
#undef __ic_common_only__
#include <machine/timer.h>
#include <machine/cortex/engine/cortex_a53/bcm_timer.h>
#include <machine/cortex/engine/cortex_a53/bcm_arm_timer.h>
#include <system/memory_map.h>
#include <utility/convert.h>

__BEGIN_SYS

// System Timer peripheral (not available on qemu). source: https://github.com/bztsrc/raspi3-tutorial/tree/master/07_delays
class System_Timer_Engine: public Timer_Common
{
    friend Timer; // for init()

private:
    static const unsigned int UNIT = 0;
    static const unsigned int FREQUENCY = Traits<Timer>::FREQUENCY;

    typedef IF<Traits<Machine>::emulated == 0, BCM_Timer, BCM_Mailbox_Timer>::Result Timer_Base;

    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef Timer_Base::Count Count;

public:
    Count read() { return timer()->read(); }

    static void reset() {
        disable();
        _count = timer()->clock() / FREQUENCY;
        timer()->config(UNIT, _count);
        enable();
    }

    static void enable() { timer()->enable(); }
    static void disable() { timer()->disable(); }

    static Hertz clock() { return timer()->clock(); }

protected:
    static void eoi(Interrupt_Id id) { Traits<Machine>::emulated ? timer()->eoi() : timer()->config(UNIT, _count); }

private:
    static void init() {
        reset();
    }

private:
    static Timer_Base * timer() { return reinterpret_cast<Timer_Base *>( Memory_Map::TIMER0_BASE ); }
    
private:
    static Count _count;
};

// This timer clock is related to the Platform clock (i.e., the frequency of the cores)
class ARM_Timer_Engine: public Timer_Common
{
    friend Timer; // for init()

private:
    static const unsigned int UNIT = 0;
    static const unsigned int FREQUENCY = Traits<Timer>::FREQUENCY;

    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef ARM_Timer::Count Count;

public:
    Count read() { return timer()->read(); }

    static void enable() { timer()->enable(); }
    static void disable() { timer()->disable(); }

    static Hertz clock() { return timer()->clock(); }

protected:
    static void eoi(Interrupt_Id id) { timer()->eoi(); }
    static void reset() {
        timer()->disable();
        _count = timer()->clock() / FREQUENCY;
        timer()->config(UNIT, _count);
        timer()->enable();
    }

private:
    static void init() {
        reset();
    }

private:
    static ARM_Timer * timer() { return reinterpret_cast<ARM_Timer *>(Memory_Map::TIMER1_BASE); }

private:
    static Count _count;
};

class User_Timer_Engine: public Timer_Common
{
    friend Timer; // for init()

private:
    static const unsigned int UNITS = Traits<Timer>::UNITS;
    static const unsigned int UNIT = 1;

    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef BCM_Timer::Count Count;

public:
    User_Timer_Engine(unsigned int unit, const Microsecond & time, bool interrupt, bool periodic) {
        assert(unit < UNITS);
        _count = Convert::us2count(time, clock());
        _periodic = periodic;
        power(FULL);
        timer()->config(UNIT, _count);
        if(interrupt)
            IC::enable(IC::INT_USER_TIMER0);
        else
            IC::disable(IC::INT_USER_TIMER0);
    }

    ~User_Timer_Engine() {
        power(OFF);
    }

    Count read() { return timer()->read(); }

    void enable() { timer()->enable(); }
    void disable() { timer()->disable(); }

    Hertz clock() { return timer()->clock(); }

    void power(const Power_Mode & mode) {
        switch(mode) {
        case ENROLL:
            break;
        case DISMISS:
            break;
        case SAME:
            break;
        case FULL:
        case LIGHT:
        case SLEEP:
            // TODO: clock_timer()
            break;
        case OFF:
            break;
        }
    }

protected:
    static void eoi(Interrupt_Id id) { if(_periodic) timer()->config(UNIT, _count); }

private:
    static BCM_Timer * timer() { return reinterpret_cast<BCM_Timer *>(Memory_Map::TIMER0_BASE); }

private:
    static bool _periodic;
    static Count _count;
};

__END_SYS

#endif
