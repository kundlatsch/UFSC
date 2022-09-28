// EPOS GPIO Mediator Common package

#ifndef __gpio_h
#define __gpio_h

#include <system/config.h>

__BEGIN_SYS

class GPIO_Common
{
protected:
    GPIO_Common() {}

public:
    enum Level {
        HIGH,
        LOW
    };

    enum Edge {
        RISING,
        FALLING,
        BOTH,
        NONE
    };

    enum Direction {
        IN,
        OUT,
        INOUT
    };

    enum Pull {
        UP,
        DOWN,
        FLOATING
    };

    typedef unsigned int Port;
    enum {
        A,
        B,
        C,
        D,
        E,
        F
    };

    typedef unsigned int Pin;

public:
    unsigned char get();
    bool get(Pin mask);
    void set(unsigned char value);
    void set(Pin mask, bool value);
    void clear();
    void clear(Pin mask);

    void direction(Pin mask, Direction dir);

    void int_enable(Pin mask);
    void int_disable(Pin mask);
    void int_enable(Level level, bool power_up = false, Level power_up_level = HIGH);
    void int_enable(Edge edge, bool power_up = false, Edge power_up_edge = RISING);

    void pull(Pin mask, Pull p);
    void clear_interrupts(Pin mask);
};

__END_SYS

#endif

#if defined(__GPIO_H) && !defined(__gpio_common_only__)
#include __GPIO_H
#endif

