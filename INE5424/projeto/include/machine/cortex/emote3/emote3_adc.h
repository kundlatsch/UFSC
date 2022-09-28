// EPOS EPOSMoteIII (ARM Cortex-M3) ADC Mediator Declarations

#ifndef __emote3_adc_h_
#define __emote3_adc_h_

#include <architecture/cpu.h>
#define __adc_common_only__
#include <machine/adc.h>
#undef __adc_common_only__
#include <machine/gpio.h>
#include <system/memory_map.h>
#include <machine/cortex/engine/pl061.h>
#include "emote3_ioctrl.h"

__BEGIN_SYS

class CC2538_ADC: public ADC_Common
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::ADC0_BASE) eMote3_ADC"

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers offsets from BASE (i.e. this)
    enum {
        // Register Name     Offset   Type  Width  Reset Value
        ADCCON1           =  0x00, // RW    32     0x00000033
        ADCCON2           =  0x04, // RW    32     0x00000010
        ADCCON3           =  0x08, // RW    32     0x00000000
        ADCL              =  0x0C, // RW    32     0x00000000
        ADCH              =  0x10, // RO    32     0x00000000
    };

    enum {                      // Description
        ADCCON1_EOC   = 1 << 7, // End of conversion. Cleared when ADCH has been read. If a new
                                // conversion is completed before the previous data has been read,
                                // the EOC bit remains high.
                                // 0: Conversion not complete
                                // 1: Conversion completed
        ADCCON1_ST    = 1 << 6, // Start conversion
                                // Read as 1 until conversion completes
                                // 0: No conversion in progress.
                                // 1: Start a conversion sequence if ADCCON1.STSEL = 11 and no
                                // sequence is running.
        ADCCON1_STSEL = 1 << 4, // Start select
                                // Selects the event that starts a new conversion sequence
                                // 00: Not implemented
                                // 01: Full speed. Do not wait for triggers
                                // 10: Timer 1 channel 0 compare event
                                // 11: ADCCON1.ST = 1
        ADCCON1_RCTRL = 1 << 2, //Controls the 16-bit random-number generator (see User Guide
                                // Chapter 16)
                                // When 01 is written, the setting automatically returns to 00 when the
                                // operation completes.
                                // 00: Normal operation (13x unrolling)
                                // 01: Clock the LFSR once (13x unrolling)
                                // 10: Reserved
                                // 11: Stopped. The random-number generator is turned off.
    };

    enum {                      // Description
        ADCCON2_SREF = 1 << 6,  // Selects reference voltage used for the sequence of conversions
                                // 00: Internal reference
                                // 01: External reference on AIN7 pin
                                // 10: AVDD5 pin
                                // 11: External reference on AIN6-AIN7 differential input
        ADCCON2_SDIV = 1 << 4,  // Sets the decimation rate for channels included in the sequence of
                                // conversions. The decimation
                                // rate also determines the resolution and time required to complete a
                                // conversion.
                                // 00: 64 decimation rate (7 bits ENOB setting)
                                // 01: 128 decimation rate (9 bits ENOB setting)
                                // 10: 256 decimation rate (10 bits ENOB setting)
                                // 11: 512 decimation rate (12 bits ENOB setting)
        ADCCON2_SCH  = 1 << 0,  // Sequence channel select
                                // Selects the end of the sequence
                                // A sequence can either be from AIN0 to AIN7 (SCH <= 7) or from
                                // differential input AIN0-AIN1 to AIN6-AIN7 (8 <= SCH <= 11). For
                                // other
                                // settings, only one conversions is performed.
                                // When read, these bits indicate the channel number on which a
                                // conversion is ongoing:
                                // 0000: AIN0
                                // 0001: AIN1
                                // 0010: AIN2
                                // 0011: AIN3
                                // 0100: AIN4
                                // 0101: AIN5
                                // 0110: AIN6
                                // 0111: AIN7
                                // 1000: AIN0-AIN1
                                // 1001: AIN2-AIN3
                                // 1010: AIN4-AIN5
                                // 1011: AIN6-AIN7
                                // 1100: GND
                                // 1101: Reserved
                                // 1110: Temperature sensor
                                // 1111: VDD/3
    };

    enum {                      // Description
        ADCCON3_EREF = 1 << 6,  // Selects reference voltage used for the extra conversion
                                // 00: Internal reference
                                // 01: External reference on AIN7 pin
                                // 10: AVDD5 pin
                                // 11: External reference on AIN6-AIN7 differential input
        ADCCON3_EDIV = 1 << 4,  // Sets the decimation rate used for the extra conversion
                                // The decimation rate also determines the resolution and the time
                                // required to complete the conversion.
                                // 00: 64 decimation rate (7 bits ENOB)
                                // 01: 128 decimation rate (9 bits ENOB)
                                // 10: 256 decimation rate (10 bits ENOB)
                                // 11: 512 decimation rate (12 bits ENOB)
        ADCCON3_ECH  = 1 << 0,  // Single channel select. Selects the channel number of the single
                                // conversion that is triggered by
                                // writing to ADCCON3.
                                // 0000: AIN0
                                // 0001: AIN1
                                // 0010: AIN2
                                // 0011: AIN3
                                // 0100: AIN4
                                // 0101: AIN5
                                // 0110: AIN6
                                // 0111: AIN7
                                // 1000: AIN0-AIN1
                                // 1001: AIN2-AIN3
                                // 1010: AIN4-AIN5
                                // 1011: AIN6-AIN7
                                // 1100: GND
                                // 1101: Reserved
                                // 1110: Temperature sensor
                                // 1111: VDD/3
    };

    enum {                      // Description
        ADCL_ADC = 1 << 2,      // Least-significant part of ADC conversion result
    };

    enum {                      // Description
        ADCH_ADC = 1 << 0,      // Most-significant part of ADC conversion result
    };

    enum Channel {
        SINGLE_ENDED_ADC0 = 0,
        SINGLE_ENDED_ADC1 = 1,
        SINGLE_ENDED_ADC2 = 2,
        SINGLE_ENDED_ADC3 = 3,
        SINGLE_ENDED_ADC4 = 4,
        SINGLE_ENDED_ADC5 = 5,
        SINGLE_ENDED_ADC6 = 6,
        SINGLE_ENDED_ADC7 = 7,
        DIFF8             = 8,
        DIFF9             = 9,
        DIFF10            = 10,
        DIFF11            = 11,
        GND               = 12,
        TEMPERATURE       = 14,
        AVDD5_3           = 15,
    };

    enum {
        INTERNAL_REF   = 0,
        EXTERNAL_REF   = 1, // External reference on AIN7 pin
        SYSTEM_REF     = 2,
        EXTERNAL_DIFF  = 3
    };

    enum Resolution {
        BITS_7  = 0, //  7 bits resolution, 64  decimation rate
        BITS_9  = 1, //  9 bits resolution, 128 decimation rate
        BITS_10 = 2, // 10 bits resolution, 256 decimation rate
        BITS_12 = 3  // 12 bits resolution, 512 decimation rate
    };

public:
    unsigned short read(const Channel & channel, const Reference & reference, const Resolution & resolution) {
        adc(ADCCON3) = (reference * ADCCON3_EREF) | (resolution * ADCCON3_EDIV) | (channel * ADCCON3_ECH);
        while(!(adc(ADCCON1) & ADCCON1_EOC));
        unsigned short ret = (adc(ADCH) << 8) | adc(ADCL);
        switch(resolution) {
            case BITS_7:  ret >>= 9; break;
            case BITS_9:  ret >>= 7; break;
            case BITS_10: ret >>= 6; break;
            case BITS_12: ret >>= 4; break;
        }
        return ret;
    }

    // returns the voltage corresponding to the reading, with three decimal places (e.g. 2534 means 2.534V)
    // reference uses the same voltage notation
    unsigned int convert(unsigned short reading, unsigned int reference, const Resolution & resolution) {
        unsigned int limit;
        switch(resolution) {
            case BITS_7:  limit =   63; break;
            case BITS_9:  limit =  255; break;
            case BITS_10: limit =  511; break;
            case BITS_12: limit = 2047; break;
        }
        return reading * reference / limit;
    }

private:
    volatile Reg32 & adc(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

class ADC_Engine: public CC2538_ADC
{
public:
    ADC_Engine(unsigned int unit, const Reference & reference, unsigned int bits)
    : _channel(static_cast<Channel>(unit)), _reference(reference), _resolution((bits <= 7) ? BITS_7 : (bits <= 9) ? BITS_9 : (bits <= 10) ? BITS_10 : BITS_12) {
        _adc = new(reinterpret_cast<CC2538_ADC *>(Memory_Map::ADC_BASE)) CC2538_ADC;
        // Configure GPIO port A for ADC
        PL061 * pl061 = new(reinterpret_cast<PL061 *>(Memory_Map::GPIOA_BASE)) PL061;
        pl061->direction(1 << unit, PL061::IN);
        pl061->pull(1 << unit, PL061::FLOATING);
        IOCtrl * ioc = new(reinterpret_cast<IOCtrl *>(Memory_Map::IOC_BASE)) IOCtrl;
        ioc->gpio_floating(0, unit);
    }

    unsigned short read() { return _adc->read(_channel, _reference, _resolution); }
    unsigned int convert(unsigned short reading, unsigned int reference) { return _adc->convert(reading, reference, _resolution); }

    static void init() {}

private:
    Channel _channel;
    Reference _reference;
    Resolution _resolution;

    CC2538_ADC * _adc;
};

__END_SYS

#endif
