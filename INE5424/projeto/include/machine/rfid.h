// EPOS RFID Reader Mediator Common Package

#ifndef __rfid_reader_h
#define __rfid_reader_h

#include <system/config.h>

__BEGIN_SYS

class RFID_Reader_Common
{
protected:
    RFID_Reader_Common() {}
};

__END_SYS

#endif

#if defined(__RFID_READER_H) && !defined(__rfid_reader_common_only__)
#include __RFID_READER_H
#endif
