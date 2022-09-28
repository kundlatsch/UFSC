// EPOS EPOSMote III (ARM Cortex-M3) MCU RS-485 Mediator Declarations

#ifndef __emote3_rs485_h
#define __emote3_rs485_h

#include <machine/gpio.h>

__BEGIN_SYS

class RS485_Engine: public RS485_Common
{
private:
    static const char RE_PORT = 'C';
    static const unsigned int RE_PIN  = 5;
    static const char DE_PORT = 'C';
    static const unsigned int DE_PIN  = 6;

public:
    RS485_Engine(): _re(GPIO(RE_PORT, RE_PIN, GPIO::OUT)), _de(GPIO(DE_PORT, DE_PIN, GPIO::OUT)) {}

    void out() { _re.set(true); _de.set(true); }
    void in() { _re.set(false); _de.set(false); }

private:
    GPIO _re;
    GPIO _de;
};

__END_SYS

#endif
