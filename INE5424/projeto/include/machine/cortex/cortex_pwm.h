// EPOS ARM Cortex PWM Mediator Declarations

#ifndef __cortex_pwm_h
#define __cortex_pwm_h

#include <timer.h>
#include <gpio.h>
#include __MODEL_H

__BEGIN_SYS

class PWM: private PWM_Common, private Machine_Model
{
public:
    PWM(User_Timer * timer, GPIO * gpio, const Percent & duty_cycle)
    : _gpio(gpio), _timer(timer) {
        _timer->disable();
        enable_pwm(_timer->_channel, _gpio->_port, _gpio->_pin);
        _timer->pwm(duty_cycle);
    }
    ~PWM() { disable_pwm(_timer->_channel, _gpio->_port, _gpio->_pin); }

    void duty_cycle(const Percent & d) { _timer->pwm(d); }
    void enable() { _timer->enable(); }
    void disable() { _timer->disable(); }
    void power(const Power_Mode & mode) { _timer->power(mode); }

private:
    GPIO * _gpio;
    User_Timer * _timer;
};

__END_SYS

#endif
