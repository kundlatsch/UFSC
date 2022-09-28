// EPOS ARM Cortex ADC Mediator Declarations

#ifndef __cortex_adc_h
#define __cortex_adc_h

#include <machine/adc.h>
#include __HEADER_MMOD(adc)

__BEGIN_SYS

class ADC: private ADC_Engine
{
    friend Machine;

private:
    typedef ADC_Engine Engine;

public:
    using ADC_Common::INTERNAL;
    using ADC_Common::EXTERNAL;
    using ADC_Common::SYSTEM;
    using ADC_Common::EXTERNAL_DIFF;


public:
    ADC(unsigned int unit = 0, const Reference & reference = INTERNAL, unsigned int bits = 12): Engine(unit, reference, bits) {}

    using Engine::read;
    using Engine::convert;

private:
    using Engine::init;
};

__END_SYS

#endif
