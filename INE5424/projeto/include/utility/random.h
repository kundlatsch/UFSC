// EPOS Pseudo Random Number Generator Utility Declarations

// From http://en.wikipedia.org/wiki/Linear_congruential_generator
// with A = 2 and variable C on a simplification of:
// X1 = aX0 + c
// as X1 = (X0 << 1) xor C

#ifndef __random_h
#define __random_h

#include <system/config.h>

__BEGIN_UTIL

class Random
{
private:
    static const unsigned long A = 1103515245;
    static const unsigned long C = 12345;
    static const unsigned long M = 4294967295U;

public:
    static int random() {
        _seed = ((_seed * A) + C) % M; //_seed = (_seed << 1) ^ n;
        return _seed;
    }

    static void seed(int value) {
        _seed = value;
    }

private:
    static int _seed;
};

__END_UTIL

#endif
