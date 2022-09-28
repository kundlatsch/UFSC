// EPOS RS485 Mediator Common Package

#ifndef __RS485_h
#define __RS485_h

#include <system/config.h>

__BEGIN_SYS

class RS485_Common
{
protected:
    RS485_Common() {}

public:
    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits);

    int read(char * data, unsigned int max_size);
    int write(const char * data, unsigned int size);

    void flush();
    bool ready_to_get();
    bool ready_to_put();

    void int_enable(bool receive = true, bool send = true, bool line = true, bool modem = true);
    void int_disable(bool receive = true, bool send = true, bool line = true, bool modem = true);
};

__END_SYS

#endif

#if defined(__RS485_H) && !defined(__rs485_common_only__)
#include __RS485_H
#endif
