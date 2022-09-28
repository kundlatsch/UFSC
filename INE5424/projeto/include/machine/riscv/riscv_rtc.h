// EPOS RISC-V RTC Mediator Declarations

#ifndef __riscv_rtc_h
#define __riscv_rtc_h

#include <rtc.h>

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
