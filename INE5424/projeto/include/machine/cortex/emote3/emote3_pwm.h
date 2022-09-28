// EPOS ARM Cortex-M3 PWM Mediator Declarations

#ifndef __emote3_pwm_h
#define __emote3_pwm_h

#include <machine/timer.h>
#include <machine/gpio.h>
#include __MODEL_H

__BEGIN_SYS

// PWM
    static void enable_pwm(unsigned int timer, char gpio_port, unsigned int gpio_pin) {
        unsigned int port = gpio_port - 'A';
        unsigned int sel = PA0_SEL + 0x20 * port + 0x4 * gpio_pin;

        switch(timer) {
            case 0: ioc(sel) = GPT0CP1; break;
            case 1: ioc(sel) = GPT1CP1; break;
            case 2: ioc(sel) = GPT2CP1; break;
            case 3: ioc(sel) = GPT3CP1; break;
        }
        ioc(sel + 0x80) = OE;

        unsigned int pin_bit = 1 << gpio_pin;
        switch(gpio_port)
        {
            case 'A': gpioa(AFSEL) |= pin_bit; break;
            case 'B': gpiob(AFSEL) |= pin_bit; break;
            case 'C': gpioc(AFSEL) |= pin_bit; break;
            case 'D': gpiod(AFSEL) |= pin_bit; break;
        }
    }

    static void disable_pwm(unsigned int timer, unsigned int gpio_port, unsigned int gpio_pin) {
        unsigned int pin_bit = 1 << gpio_pin;
        switch(gpio_port)
        {
            case 'A': gpioa(AFSEL) &= ~pin_bit; break;
            case 'B': gpiob(AFSEL) &= ~pin_bit; break;
            case 'C': gpioc(AFSEL) &= ~pin_bit; break;
            case 'D': gpiod(AFSEL) &= ~pin_bit; break;
        }
    }



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
