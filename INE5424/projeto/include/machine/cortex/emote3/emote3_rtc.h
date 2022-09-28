// EPOS ARM Cortex-M3 RTC Mediator Declarations

#ifndef __emote3_rtc_h
#define __emote3_rtc_h

#include <machine/rtc.h>

__BEGIN_SYS

class RTC: public RTC_Common
{
public:
    RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { return 0; }
};

__END_SYS

#endif
