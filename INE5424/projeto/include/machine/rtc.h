// EPOS RTC Mediator Common Package

#ifndef __rtc_h
#define __rtc_h

#include <system/config.h>
#include <utility/debug.h>

__BEGIN_SYS

class RTC_Common
{
protected:
    RTC_Common() {}

public:
    // Calendar date and time
    class Date {
    public:
        Date() {}
        Date(unsigned int Y, unsigned int M, unsigned int D, unsigned int h, unsigned int m, unsigned int s)
        : _Y(Y), _M(M), _D(D), _h(h), _m(m), _s(s) {}
        Date(const Second & seconds, unsigned long epoch_days = 0);

        operator Second() const { return to_offset(); }
        Second to_offset(unsigned long epoch_days = 0) const;

        unsigned int year() const { return _Y; };
        unsigned int month() const { return _M; };
        unsigned int day() const { return _D; };
        unsigned int hour() const { return _h; };
        unsigned int minute() const { return _m; };
        unsigned int second() const { return _s; };

        void adjust_year(int y) { _Y += y; };

        friend Debug & operator<<(Debug & db, const Date & d) {
            db << "{" << d._Y << "/" << d._M << "/" << d._D << " " << d._h << ":" << d._m << ":" << d._s << "}";
            return db;
        }

    private:
        unsigned int _Y;
        unsigned int _M;
        unsigned int _D;
        unsigned int _h;
        unsigned int _m;
        unsigned int _s;
    };
};

// If the machine does not feature a RTC, define seconds_since_epoch to be 0
#if !defined(__RTC_H) && !defined(__rtc_common_only__)
class RTC: public RTC_Common
{
public:
    RTC() {}

    static Date date();
    static void date(const Date & d);

    static Second seconds_since_epoch() { return 0; }
};
#endif

__END_SYS

#endif

#if defined(__RTC_H) && !defined(__rtc_common_only__)
#include __RTC_H
#endif
