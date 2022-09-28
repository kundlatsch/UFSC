// EPOS CM1101 CO2 Sensor Mediator Declarations

#ifndef __ccm1101_h
#define __ccm1101_h

#include <machine/uart.h>

__BEGIN_SYS

// Cubic Nondispersive Infrared (NDIR) Carbon Dioxide Single Beam Sensor CM1101
class CM1101
{
public:
    // Commands
    enum {
        MEASUREMENT_LOOKUP      = 01,
        VOLTAGE_LOOKUP          = 02,
        GET_FIRMWARE_VERSION    = 0x1e,
        GET_SERIAL_NUMBER       = 0x1f
    };

    // Return codes
    enum {
        ACK                     = 0x16,
        NACK                    = 0x06
    };

    // Status
    enum {
        OK                      = 0x00,
        BAD_LEN                 = 0x01,
        BAD_CMD                 = 0x02,
        BAD_STT                 = 0x03,
        BAD_CRC                 = 0x04
    };

    // Versions
    // TODO: define the correct integer value for each version as the checksum of the responses to GET_FIRMWARE_VERSION
    enum {
        UNKNOWN,
        V1_21,
        V3_03
    };
    static const unsigned int MAX_RESPONSE_LENGTH = 8;

public:
    CM1101(UART * uart): _uart(uart), _version(0), _status(OK), _co2(0), _temperature(0), _humidity(0) { get_firmware_version(); }

    int get() { return (_status == OK) ? _co2 : 0; }
    int sample() { update_data(); return get(); }

    int co2() { update_data(); return get(); } // in PPM
    int temperature() { update_data(); return (_status == OK) ? _temperature : 0; } // in °C
    int humidity() { update_data(); return (_status == OK) ? _humidity : 0;} // in %

    void enable() {} // not supported by CM1101
    void disable() {} // not supported by CM1101

private:
    void get_firmware_version() {
        char resp[MAX_RESPONSE_LENGTH];

        send_command(GET_FIRMWARE_VERSION);

        char ret = _uart->get();
        char len = _uart->get();
        char cmd = _uart->get();
        if(ret == ACK) {
            char cs = ret + len + cmd;
            for(int i = 0; i < (len-1); i++) {
                resp[i] = _uart->get();
                cs += resp[i];
            }
            char c = _uart->get();
            if(c == (0x100 - cs)) {
                _version = cs; // We use the command's response checksum to identify versions
                _status = OK;
            } else {
                _version = UNKNOWN;
                _status = BAD_CRC;
            }
        } else {
            _version = UNKNOWN;

            char err = _uart->get();
            char cs = _uart->get();
            if(cs == (0x100 - (ret + len + cmd + err)))
                _status = err;
            else
                _status = BAD_CRC;
        }
    }

    void update_data() {
        // CHECK: this code looks weird. If the manual says it's Celsius, it should be. The 0x100 are right? Shouldn't it be decimal 100 as in BCD? And what about continuing after the NACK? Shouldn't we return right after?
        char resp[MAX_RESPONSE_LENGTH];

        send_command(MEASUREMENT_LOOKUP);

        char ret = _uart->get();
        char len = _uart->get();
        char cmd = _uart->get();
        if(ret == ACK) {
            char cs = ret + len + cmd;
            for(int i = 0; i < (len - 1); i++) {
                resp[i] = _uart->get();
                cs += resp[i];
            }
            char c = _uart->get();
            if(c == (0x100 - cs)) {
                _co2 = resp[0] * 0x100 + resp[1];
                if(_version == V3_03) {
                    // Temperature seems to be in Fahrenheit despite the manual stating they're in Celsius!
                    _temperature = 10 * ((resp[2] * 0x100 + resp[3]) / 10 - 32) / 18;
                    _humidity = (resp[4] * 0x100 + resp[5]) / 10;
                }
                _status = OK;
            } else
                _status = BAD_CRC;
        } else {
            char err = _uart->get();
            char cs = _uart->get();
            if(cs == (0x100 - (ret + len + cmd + err)))
                _status = err;
            else
                _status = BAD_CRC;
        }
    }

    void send_command(unsigned char cmd, unsigned char * df = 0, unsigned int df_len = 0) {
        unsigned char df_sum = 0;

        _uart->put(0x11);
        _uart->put(df_len + 1);
        _uart->put(cmd);

        for(unsigned int i = 0; i < df_len; i++) {
            _uart->put(df[i]);
            df_sum += df[i];
        }

        _uart->put(0x100 - (0x11 + (df_len + 1) + cmd + df_sum));
    }

private:
    UART * _uart;
    unsigned int _version;
    char _status;

    int _co2;
    int _temperature;
    int _humidity;
};

__END_SYS

#endif
