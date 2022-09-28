// EPOS I2C Mediator Common package

#ifndef __i2c_h
#define __i2c_h

#include <system/config.h>

__BEGIN_SYS

class I2C_Common
{
public:
    enum Role {
        MASTER,
        SLAVE,
    };

protected:
    I2C_Common() {}

public:
    void config(const Role & role);

    bool get(char slave_address, char * data, bool stop = true);
    bool put(unsigned char slave_address, char data, bool stop = true);

    bool read(char slave_address, char * data, unsigned int size, bool stop = true);
    bool write(unsigned char slave_address, const char * data, unsigned int size, bool stop = true);

    void flush();
    bool ready_to_get();
    bool ready_to_put();

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true);
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true);

    void loopback(bool on = true);      // loopback(false) to switch off
    void reset();
};

__END_SYS

#endif

#if defined(__I2C_H) && !defined(__i2c_common_only__)
#include __I2C_H
#endif
