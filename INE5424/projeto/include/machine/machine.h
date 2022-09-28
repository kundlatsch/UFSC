// EPOS Machine Mediator Common Package

#ifndef __machine_machine_h
#define __machine_machine_h

#include <system/config.h>
#include <architecture/tsc.h>
#include <utility/convert.h>

__BEGIN_SYS

class Machine_Common
{
public:
    template<typename Family, int unit = 0>
    struct Initializer
    {
        typedef typename Traits<Family>::DEVICES::template Get<unit>::Result DEV;

        static void init() {
            if(Traits<DEV>::enabled)
                DEV::init(unit);

            Initializer<Family, unit + 1>::init();
        };
    };

    template<typename Family>
    struct Initializer<Family, Traits<Family>::DEVICES::Length>
    {
        static void init() {};
    };

protected:
    Machine_Common() {}

public:
    static void delay(const Microsecond & time) {
        assert(Traits<TSC>::enabled);
        TSC::Time_Stamp end = TSC::time_stamp() + Convert::us2count<TSC::Time_Stamp, Microsecond>(TSC::frequency(), time);
        while(end > TSC::time_stamp());
    }

    static void panic();
    static void reboot();
    static void poweroff();

    static const UUID & uuid();

    static void clear_bss();
};

__END_SYS

#endif

#if defined(__MACH_H) && !defined(__mach_common_only__)
#include __MACH_H
#endif
