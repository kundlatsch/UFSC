// EPOS RTC Mediator Common Package Implementation

#include <machine/rtc.h>

__BEGIN_SYS

RTC_Common::Date::Date(const Second & seconds, unsigned long epoch_days)
{
    static int days_per_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    int tmp = seconds;

    _s = tmp % 60;
    tmp /= 60;
    _m = tmp % 60;
    tmp /= 60;
    _h = tmp % 24;
    tmp /= 24;
    tmp += epoch_days;
    for(_Y = 1; tmp - 365 > 0; _Y++, tmp -= 365)
        if((!(_Y % 4) && (_Y % 100)) || !(_Y % 400))
            tmp--;
    days_per_month[1] = 28;
    if((!(_Y % 4) && (_Y % 100)) || !(_Y % 400))
        days_per_month[1] = 29;
    for(_M = 1; tmp - days_per_month[_M] > 0; _M++, tmp -= days_per_month[_M]);
    _D = tmp;
}

Second RTC_Common::Date::to_offset(unsigned long epoch_days) const
{
    Date tmp(*this);

    tmp._M -= 2;
    if(tmp._M < 0) {   // 1..12 -> 11,12,1..10
        tmp._M += 12;  // puts Feb last since it may have leap day
        tmp._Y -= 1;
    }
    return ((((tmp._Y/4 - tmp._Y/100 + tmp._Y/400 + 367 * tmp._M/12 + tmp._D) + tmp._Y * 365 - epoch_days) * 24 + tmp._h) * 60 + tmp._m) * 60 + tmp._s;
}


__END_SYS
