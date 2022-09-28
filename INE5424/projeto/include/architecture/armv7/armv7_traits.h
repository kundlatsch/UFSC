// EPOS ARMv7 Architecture Metainfo
#ifndef __armv7_traits_h
#define __armv7_traits_h

#include <system/config.h>

__BEGIN_SYS

template<> struct Traits<CPU>: public Traits<Build>
{
    static const unsigned int ENDIANESS         = LITTLE;
    static const unsigned int WORD_SIZE         = 32;
    static const unsigned int CLOCK             = (MODEL == LM3S811) ? 50000000 : (MODEL == Zynq) || (MODEL == Realview_PBX) ? 666666687 : 32000000;
    static const bool unaligned_memory_access   = false;
};

template<> struct Traits<MMU>: public Traits<Build>
{
    static const bool colorful = false;
    static const unsigned int COLORS = 1;
};

template<> struct Traits<FPU>: public Traits<Build>
{
    static const bool enabled = (Traits<Build>::MODEL == Traits<Build>::Raspberry_Pi3);;
    static const bool user_save = true;
};

template<> struct Traits<TSC>: public Traits<Build>
{
    // In order to use Machine::delay, TSC must be enabled
    // On eMote3, TSC uses User_Timer on channel 3. To use channel 3, you must disable the TSC
    // On LM3S811:
    // * TSC uses User_Timer on channel 1. To use channel 1, you must disable the TSC
    // * LM3S811 does not support up-count mode on general purpose timers,
    //   and QEMU (v2.7.50) does not support reading the value of general purpose timers,
    //   thus TSC::time_stamp() does not work, but Machine::delay does.
    static const bool enabled = (Traits<Build>::MODEL != Traits<Build>::LM3S811);

    // TODO: http://stackoverflow.com/questions/16236460/arm-cortex-a9-event-counters-return-0
};

template<> struct Traits<PMU>: public Traits<Build>
{
    static const bool enabled = (Traits<Build>::MODEL == Traits<Build>::Raspberry_Pi3);
};

__END_SYS

#endif
