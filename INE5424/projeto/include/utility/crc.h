// EPOS CRC Utility Declarations

#ifndef __crc_h
#define __crc_h

#include <system/config.h>

__BEGIN_UTIL

class CRC
{
public:
    static unsigned short crc16(char * ptr, int size) {
        unsigned short crc = 0;

        while(--size >= 0) {
            crc ^= (unsigned short)(*ptr++) << 8;
            int i = 8;
            do{
        	if(crc & 0x8000)
        	    crc = (crc << 1) ^ 0x1021;
        	else
        	    crc = crc << 1;
            } while(--i);
        }

        return crc;
    }

};

__END_UTIL

#endif
