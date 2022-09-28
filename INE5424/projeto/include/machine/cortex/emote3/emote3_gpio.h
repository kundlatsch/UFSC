// EPOS EPOSMote III (ARM Cortex-M3) GPIO Mediator Declarations

#ifndef __emote3_gpio_h
#define __emote3_gpio_h

#include <machine/cortex/engine/cortex_m3/scb.h>
#include <machine/cortex/engine/pl061.h>
#include "emote3_sysctrl.h"
#include <system/memory_map.h>
#include "emote3_ioctrl.h"

__BEGIN_SYS

class GPIO_Engine: public GPIO_Common
{
protected:
    static const unsigned int PORTS = Traits<GPIO>::UNITS;

public:
    GPIO_Engine(Port port, Pin pin, Direction dir, Pull p, Edge int_edge)
    : _port(port), _pin(pin), _pin_mask(1 << pin) {
        assert(port < PORTS);
        power(FULL);
        _gpio = new(reinterpret_cast<void *>(Memory_Map::GPIOA_BASE + port * 0x1000)) PL061;
        _gpio->select_pin_function(_pin_mask, PL061::FUN_GPIO);
        direction(dir);
        pull(p);
        if(int_edge != NONE)
            _gpio->clear_interrupts(port, _pin_mask);
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

    void direction(Direction dir) {
        _direction = dir;
        _gpio->direction(_pin_mask, dir);
    }

    void pull(Pull p) {
        IOCtrl * ioc = new(reinterpret_cast<IOCtrl *> (Memory_Map::IOC_BASE)) IOCtrl;
        switch(p) {
        case UP:
            ioc->gpio_pull_up(_port, _pin);
            break;
        case DOWN:
            ioc->gpio_pull_down(_port, _pin);
            break;
        case FLOATING:
            ioc->gpio_floating(_port, _pin);
            break;
        }
    }

    void int_enable() { _gpio->int_enable(_pin_mask); }
    void int_enable(Level level, bool power_up = false, Level power_up_level = HIGH);
    void int_enable(Edge edge, bool power_up = false, Edge power_up_edge = RISING);
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
    Pin _pin;
    Pin _pin_mask;
    Direction _direction;
    PL061 * _gpio;
};

__END_SYS

#endif
