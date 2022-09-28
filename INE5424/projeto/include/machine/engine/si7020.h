// EPOS SI7020 Temperature and Humidity Sensor Mediator Declarations

#ifndef __si7020_h
#define __si7020_h

#include <machine/machine.h>
#include <machine/i2c.h>

__BEGIN_SYS

// Silicon Labs SI7020 Temperature and Humidity sensor
class SI7020
{
private:
    // Addresses
    enum {
        I2C_ADDR        = 0x40, // The 7-bit base slave address is 0x40
        RH_HOLD         = 0xe5, // Measure Relative Humidity, Hold Master Mode
        RH_NOHOLD       = 0xf5, // Measure Relative Humidity, No Hold Master Mode
        TEMP_HOLD       = 0xe3, // Measure Temperature, Hold Master Mode
        TEMP_NOHOLD     = 0xf3, // Measure Temperature, No Hold Master Mode
        TEMP_PREV       = 0xe0, // Read Temperature Value from Previous RH Measurement
        RESET_SI        = 0xfe, // Reset
        WREG            = 0xe6, // Write RH/T User Register 1
        RREG            = 0xe7, // Read RH/T User Register 1
        WHCR            = 0x51, // Write Heater Control Register
        RHCR            = 0x11, // Read Heater Control Register
    };

    // Resolution
    enum {
        RH_12B_TEMP_14B = 0x0, /// < 12 bits for RH, 14 bits for Temp
        RH_8B_TEMP_12B  = 0x1, /// < 8  bits for RH, 12 bits for Temp
        RH_10B_TEMP_13B = 0x2, /// < 10 bits for RH, 13 bits for Temp
        RH_11B_TEMP_11B = 0x3, /// < 11 bits for RH, 11 bits for Temp
    };

public:
    SI7020(I2C * i2c): _i2c(i2c) {}

    bool reset(I2C * i2c) {
        bool ret = i2c->put(I2C_ADDR, RESET_SI, true);
        if(ret)
            Machine::delay(15000); // Si7020 may take up to 15ms to power up after a soft reset
        return ret;
    }

    int humidity() { // in %
        char data[2];
        _i2c->put(I2C_ADDR, RH_HOLD, false);

        _i2c->read(I2C_ADDR, data, 2, true);

        int ret = (data[0] << 8 ) | data[1];

        ret = (1250 * ret) / 65536 - 60;
        ret = (ret + 5) / 10;

        // the measured value of %RH may be slightly greater than 100
        // when the actual RH level is close to or equal to 100
        if(ret < 0) ret = 0;
        else if(ret > 100) ret = 100;
        return ret;
    }

    int temperature() { // in °C
        char data[2];
        _i2c->put(I2C_ADDR, TEMP_HOLD, false);

        _i2c->read(I2C_ADDR, data, 2, true);

        int ret = (data[0] << 8) | data[1];
        ret = ((17572 * ret)) / 65536 - 4685;
        ret = ret + 50 / 100;
        return ret;
    }

private:
    I2C * _i2c;
};

__END_SYS

#endif
