// EPOS EPOSMote III (ARM Cortex-M3) Timer Mediator Declarations

#ifndef __emote3_timer_h
#define __emote3_timer_h

#define __ic_common_only__
#include <machine/ic.h>
#undef __ic_common_only__
#include <machine/cortex/engine/cortex_m3/systick.h>
#include <machine/cortex/engine/cortex_m3/gptm.h>
#include "emote3_sysctrl.h"
#include <system/memory_map.h>
#include <utility/convert.h>

__BEGIN_SYS

class System_Timer_Engine: public Timer_Common
{
private:
    static const unsigned int FREQUENCY = Traits<Timer>::FREQUENCY;

    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef SysTick::Count Count;

public:
    System_Timer_Engine() { new(systick()) SysTick; }

    static Count count() { return systick()->count(); }
    static Count read() { return systick()->read(); }

    static void reset() { systick()->config(systick()->clock() / FREQUENCY, true, true); }
    static void enable() { systick()->enable(); }
    static void disable() { systick()->disable(); }

    static Hertz clock() { return systick()->clock(); }

protected:
    static void eoi(Interrupt_Id id) { systick()->eoi(id); };

    static void init() {
        disable();
        reset();
        enable();
    }

private:
    static SysTick * systick() { return reinterpret_cast<SysTick *>(Memory_Map::SCB_BASE); }
};


class User_Timer_Engine: public Timer_Common
{
private:
    static const unsigned int UNITS = Traits<Timer>::UNITS;

    typedef IC_Common::Interrupt_Id Interrupt_Id;

public:
    typedef GPTM::Count Count;

public:
    User_Timer_Engine(unsigned int unit, const Microsecond & time, bool interrupt, bool periodic): _unit(unit), _gptm(new (gptm(unit)) GPTM) {
        assert(unit < UNITS);
        power(FULL);
        _gptm->config(time, interrupt, periodic);
    }
    ~User_Timer_Engine() {
        power(OFF);
    }

    Count count() const { return _gptm->count(); }
    Count read() const { return _gptm->read(); }

    void enable() const { _gptm->enable(); }
    void disable() const { _gptm->disable(); }

    Hertz clock() const { return _gptm->clock(); }

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
            scr()->clock_timer(_unit);
            break;
        case SLEEP:
        case OFF:
            scr()->unclock_timer(_unit);
            break;
        }
    }

protected:
    static void eoi(Interrupt_Id id) {
        int i;
        switch(id) {
        case IC::INT_USER_TIMER0: i = 0; break;
        case IC::INT_USER_TIMER1: i = 1; break;
        case IC::INT_USER_TIMER2: i = 2; break;
        default: i = 3;
        }
        gptm(i)->eoi(id);
    };

private:
    static GPTM * gptm(unsigned int unit) { return reinterpret_cast<GPTM *>(Memory_Map::TIMER0_BASE + 0x1000 * unit); }
    static SysCtrl * scr() { return reinterpret_cast<SysCtrl *>(Memory_Map::SCR_BASE); }

private:
    unsigned int _unit;
    GPTM * _gptm;
};

__END_SYS

#endif
