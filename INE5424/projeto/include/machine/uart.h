// EPOS UART Mediator Common Package

#ifndef __uart_h
#define __uart_h

#include <system/config.h>

__BEGIN_SYS

class UART_Common
{
public:
    // Parity
    enum {
        NONE = 0,
        ODD = 1,
        EVEN = 2
    };

protected:
    UART_Common() {}

public:
    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits);
    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits);

    char get();
    void put(char c);

    int read(char * data, unsigned int max_size);
    int write(const char * data, unsigned int size);

    void flush();
    bool ready_to_get();
    bool ready_to_put();

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true);
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true);
};

__END_SYS

#endif

#if defined(__UART_H) && !defined(__uart_common_only__)
#include __UART_H
#endif
