// EPOS EEPROM Mediator Common Package

#ifndef __eeprom_h
#define __eeprom_h

#include <system/config.h>

__BEGIN_SYS

class EEPROM_Common
{
public:
    typedef unsigned int Address;

protected:
    EEPROM_Common() {}

public:
    int size();

    unsigned char read(const Address & a);
    void write(const Address & a, unsigned char d);
};

__END_SYS

#endif

#if defined(__EEPROM_H) && !defined(__eeprom_common_only__)
#include __EEPROM_H
#endif
