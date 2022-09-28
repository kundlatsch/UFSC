// EPOS Analog to Digital Converter (ADC) Mediator Common Package

#ifndef __adc_h
#define __adc_h

#include <system/config.h>

__BEGIN_SYS

class ADC_Common
{
public:
    static const unsigned int DEFAULT = -1UL;

    enum Reference {
        INTERNAL,
        EXTERNAL,
        SYSTEM,
        EXTERNAL_DIFF
    };

protected:
    ADC_Common() {}

public:
    unsigned int read();
    unsigned int convert(unsigned short reading, unsigned int reference);
};

__END_SYS

#endif

#if defined(__ADC_H) && !defined(__adc_common_only__)
#include __ADC_H
#endif
