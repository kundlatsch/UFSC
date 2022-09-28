// EPOS Keyboard Mediator Common Package

#ifndef __keyboard_h
#define __keyboard_h

#include <utility/observer.h>
#include <machine/display.h>

__BEGIN_SYS

class Keyboard_Common
{
protected:
    Keyboard_Common() {}
};

class Serial_Keyboard: public Keyboard_Common
{
    friend class Setup;
    friend class Machine;

public:
    typedef _UTIL::Observer Observer;
    typedef _UTIL::Observed Observed;

public:
    Serial_Keyboard() {}

    static char get() { return Serial_Display::_engine.get(); }
    static bool ready_to_get() { return Serial_Display::_engine.ready_to_get(); }

    static void attach(Observer * obs) { _observed.attach(obs); }
    static void detach(Observer * obs) { _observed.detach(obs); }

private:
    static void init() {}

private:
    static Observed _observed;
};

__END_SYS

// If the machine does not feature a Keyboard, then use the serial console
#ifndef __KEYBOARD_H
__BEGIN_SYS
class Keyboard: public IF<Traits<Serial_Keyboard>::enabled, Serial_Keyboard, Dummy<>>::Result {};
__END_SYS
#endif

#endif

#if defined(__KEYBOARD_H) && !defined(__keyboard_common_only__)
#include __KEYBOARD_H
#endif
