// EPOS ARM Cortex RS-485 Mediator Declarations

#ifndef __cortex_rs485_h
#define __cortex_rs485_h

#include <architecture/cpu.h>
#include <machine/uart.h>
#include <machine/rs485.h>
#include __HEADER_MMOD(rs485)

__BEGIN_SYS

class RS485: private UART, private RS485_Engine
{
private:
    typedef RS485_Engine Transceiver;
    typedef UART Engine;

    static const unsigned int UNIT = Traits<UART>::DEF_UNIT;
    static const unsigned int BAUD_RATE = Traits<UART>::DEF_BAUD_RATE;
    static const unsigned int DATA_BITS = Traits<UART>::DEF_DATA_BITS;
    static const unsigned int PARITY = Traits<UART>::DEF_PARITY;
    static const unsigned int STOP_BITS = Traits<UART>::DEF_STOP_BITS;

public:
    RS485(unsigned int unit = UNIT, unsigned int baud_rate = BAUD_RATE, unsigned int data_bits = DATA_BITS, unsigned int parity = PARITY, unsigned int stop_bits = STOP_BITS)
    : Engine(unit, baud_rate, data_bits, parity, stop_bits) {}

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        Engine::config(baud_rate, data_bits, parity, stop_bits);
    }

    int read(char * data, unsigned int size) {
        char * d = data;
        unsigned int s = 0;

        for(; s < size; s++) {
            if(ready_to_get())
                d[s] = get();
            else
                break;
        }
        return s;
    }

    int write(const char * data, unsigned int size) {
        const char * d = data;
        unsigned int s = 0;

        Transceiver::out();
        for(; s < size; s++)
            put(d[s]);
        Engine::flush();
        Transceiver::in();

        return s;
    }

    using Engine::flush;
    using Engine::ready_to_get;
    using Engine::ready_to_put;

    using Engine::int_enable;
    using Engine::int_disable;

    using Engine::power;
};

__END_SYS

#endif
