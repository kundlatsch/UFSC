// EPOS PMU Mediator Common Package

#ifndef __pmu_h
#define __pmu_h

#include <system/config.h>

__BEGIN_SYS

class PMU_Common
{
public:
    typedef unsigned int Channel;
    typedef unsigned int Event;
    typedef unsigned long long Count;

    enum Flags {
        NONE,
        INT
    };

protected:
    PMU_Common() {}

public:
    static Count read(Channel channel) { return 0; }
    static void config(Channel channel, Event event, Flags flags = NONE) {}
    static void write(Channel channel, Count count) {}
    static void start(Channel channel) {}
    static void stop(Channel channel) {}
    static void reset(Channel channel) {}

protected:
    static const unsigned int CHANNELS = 0;
    static const unsigned int FIXED = 0;
    static const unsigned int EVENTS = 0;
};

#ifndef __PMU_H
class PMU: public PMU_Common {};
#endif

__END_SYS

#endif

#if defined(__PMU_H) && !defined(__pmu_common_only__)
#include __PMU_H
#endif
