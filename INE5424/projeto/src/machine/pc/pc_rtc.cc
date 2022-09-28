// EPOS PC RTC Mediator Implementation

#include <machine/rtc.h>

__BEGIN_SYS

RTC::Date RTC::date()
{
    unsigned int tmp = reg(SECONDS);
    Date date(reg(YEAR), reg(MONTH), reg(DAY),
              reg(HOURS), reg(MINUTES), tmp);

    if(tmp != reg(SECONDS)) // RTC update in between?
        date = Date(reg(YEAR), reg(MONTH), reg(DAY),
        	    reg(HOURS), reg(MINUTES), reg(SECONDS));

    date.adjust_year(1900);
    if(date.year() < EPOCH_YEAR)
        date.adjust_year(100);

    db<RTC>(TRC) << "RTC::date() => " << date << endl;

    return date;
}

void RTC::date(const Date & d)
{
    db<RTC>(TRC) << "RTC::date(date= " << d << ")" << endl;

    reg(YEAR, d.year());
    reg(MONTH, d.month());
    reg(DAY, d.day());
    reg(HOURS, d.hour());
    reg(MINUTES, d.minute());
    reg(SECONDS, d.second());
}

__END_SYS
