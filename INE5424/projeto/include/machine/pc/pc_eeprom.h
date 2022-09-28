// EPOS PC EEPROM Mediator Common Declarations

#ifndef __pc_eeprom_h
#define __pc_eeprom_h

#include <machine/eeprom.h>
#include "rtc.h"

__BEGIN_SYS

class EEPROM: public EEPROM_Common, private MC146818
{
public:
    typedef EEPROM_Common::Address Address;

public:
    EEPROM() {};

    int size() const { return cmos_size(); }

    unsigned char read(const Address & a) { return cmos_read(a); }
    void write(const Address & a, unsigned char d) { cmos_write(a, d); }
};

__END_SYS

#endif
