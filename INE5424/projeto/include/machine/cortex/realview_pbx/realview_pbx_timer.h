// EPOS Realview PBX (ARM Cortex-A9) Timer Mediator Declarations

#ifndef __realview_pbx_timer_h
#define __realview_pbx_timer_h

#define __ic_common_only__
#include <machine/ic.h>
#undef __ic_common_only__
#include <machine/timer.h>
#include <machine/cortex/engine/cortex_a9/global_timer.h>
#include <machine/cortex/engine/cortex_a9/private_timer.h>
#include <system/memory_map.h>
#include <utility/convert.h>

__BEGIN_SYS

class System_Timer_Engine: public Timer_Common
{
    friend Timer; // for init()

private:
    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    static const unsigned int FREQUENCY = Traits<Timer>::FREQUENCY;

    typedef A9_Private_Timer::Count Count;

public:
    Count read() { return pt()->read(); }

    static void reset() { disable(); pt()->config(pt()->clock() / FREQUENCY); enable();}
    static void enable() { pt()->enable(); }
    static void disable() { pt()->disable(); }

    static Hertz clock() { return pt()->clock(); }

protected:
    static void eoi(Interrupt_Id id) { pt()->eoi(id); };

private:
    static void init() {
        reset();
    }

private:
    static A9_Private_Timer * pt() { return reinterpret_cast<A9_Private_Timer *>(Memory_Map::PRIVATE_TIMER_BASE); }
};


class User_Timer_Engine: public Timer_Common
{
private:
    static const unsigned int UNITS = Traits<Timer>::UNITS;

    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef A9_Global_Timer::Count Count;

public:
    User_Timer_Engine(unsigned int unit, const Microsecond & time, bool interrupt, bool periodic) {
        assert(unit < UNITS);
        power(FULL);
        new (gt()) A9_Global_Timer;
        gt()->config(time, interrupt, periodic);
    }
    ~User_Timer_Engine() {
        power(OFF);
    }

    Count count() const { return gt()->count(); }

    void enable() const  { gt()->enable(); }
    void disable() const { gt()->disable(); }

    Hertz clock() const { return gt()->clock(); }

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
            // TODO: clock_gt(mode)
            break;
        case OFF:
            break;
        }
    }

protected:
    static void eoi(Interrupt_Id id) { gt()->eoi(id); };

private:
    static A9_Global_Timer * gt() { return reinterpret_cast<A9_Global_Timer *>(Memory_Map::GLOBAL_TIMER_BASE); }
};

__END_SYS

#endif
