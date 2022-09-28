// EPOS LM3S811 (ARM Cortex-M3) GPIO Mediator Declarations

#ifndef __lm3s811_gpio_h
#define __lm3s811_gpio_h

#include <machine/cortex/engine/cortex_m3/scb.h>
#include <machine/cortex/engine/pl061.h>
#include "lm3s811_sysctrl.h"
#include <system/memory_map.h>

__BEGIN_SYS

class GPIO_Engine: public GPIO_Common
{
protected:
    static const unsigned int PORTS = Traits<GPIO>::UNITS;

public:
    GPIO_Engine(const Port & port, const Pin & pin, const Direction & dir, const Pull & p, const Edge & int_edge)
    : _pin_mask(1 << pin) {
        assert(port < PORTS);
        _port = port;
        power(FULL);
        _gpio = new(reinterpret_cast<void *>(Memory_Map::GPIOA_BASE + port * 0x1000)) PL061;
        _gpio->select_pin_function(_pin_mask, PL061::FUN_GPIO);
        pull(p);
        direction(dir);
        if(int_edge != NONE)
            _gpio->clear_interrupts(_port, _pin_mask);
    }

    bool get() const {
        assert(_direction == IN || _direction == INOUT);
        return _gpio->get(_pin_mask);
    }

    void set(bool value = true) {
        assert(_direction == INOUT || _direction == OUT);
        if(_direction == INOUT)
            _gpio->direction(_pin_mask, OUT); // temporarily, so don't set _direction
        _gpio->set(_pin_mask, value);
        if(_direction == INOUT)
            _gpio->direction(_pin_mask, IN);
    }

    void clear() { _gpio->clear(_pin_mask); }

    void direction(const Direction & dir) {
        _direction = dir;
        _gpio->direction(_pin_mask, dir);
    }

    void pull(const Pull & p) { _gpio->pull(_pin_mask, p); }

    void int_enable() { _gpio->int_enable(_pin_mask); }
    void int_enable(const Level & level, bool power_up = false, const Level & power_up_level = HIGH);
    void int_enable(const Edge & edge, bool power_up = false, const Edge & power_up_edge = RISING);
    void int_disable() { _gpio->int_disable(_pin_mask); }
    void clear_interrupts() { _gpio->clear_interrupts(_port, 0xff); }

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
            break;
        case SLEEP:
        case OFF:
            break;
        }
    }

    static void init();

private:
    Port _port;
    Pin _pin_mask;
    Direction _direction;
    PL061 * _gpio;
};

__END_SYS

#endif
