// EPOS Bitmap Utility Declarations

#ifndef __bitmap_h
#define __bitmap_h

#include "string.h"

__BEGIN_UTIL

template<unsigned int BITS>
class Bitmap
{
private:
    static const unsigned int BPI = sizeof(int) * 8;
    static const unsigned int SIZE = (BITS + BPI - 1) / BPI;
    static const unsigned int mask = BPI - 1;

public:
    Bitmap() { memset(&_map, 0, SIZE * sizeof(int)); }

    bool set(unsigned int index) {
        if((index < BITS) && !(_map[index / BPI] & (1 << (index & mask)))) {
            _map[index / BPI] |= (1 << (index & mask));
            return true;
        }
        return false;
    }

    bool reset(unsigned int index) {
        if((index < BITS) && (_map[index / BPI] & (1 << (index & mask)))) {
            _map[index / BPI] &= ~(1 << (index & mask));
            return true;
        }
        return false;
    }

    bool full(unsigned int upto) const {
        unsigned int i;
        for(i = 0; i < upto / BPI; i++)
            if(_map[i] != ~0U)
                return false;
        if((upto & mask) && ((_map[i] | ~((1 << (upto & mask)) - 1)) != ~0U))
            return false;
        return true;
    }

    bool empty(unsigned int upto = BITS) const {
        unsigned int i;
        for(i = 0; i < upto / BPI; i++)
            if(_map[i])
                return false;
        if((upto & mask) && (_map[i] & ((1 << (upto & mask)) - 1)))
            return false;
        return true;
    }

private:
     unsigned int _map[SIZE];
};

__END_UTIL

#endif
