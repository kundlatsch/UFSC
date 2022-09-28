// EPOS PWM Mediator Common package

#ifndef __pwm_h
#define __pwm_h

#include <system/config.h>

__BEGIN_SYS

class PWM_Common
{
protected:
    PWM_Common() {}

public:
    void duty_cycle(const Percent & d);
    void enable();
    void disable();
};

__END_SYS

#endif

#if defined(__PWM_H) && !defined(__pwm_common_only__)
#include __PWM_H
#endif
