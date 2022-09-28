// EPOS (Litte-endian) Big Numbers Utility Implementation

#include <utility/bignum.h>

__BEGIN_UTIL

// Class attributes
template<>
const Bignum<16>::_Word Bignum<16>::_mod = {{ 0xff, 0xff, 0xff, 0xff,
                                              0xff, 0xff, 0xff, 0xff,
                                              0xff, 0xff, 0xff, 0xff,
                                              0xfd, 0xff, 0xff, 0xff }};

template<>
const Bignum<16>::_Barrett Bignum<16>::_barrett_u = {{ 17, 0, 0, 0,
                                                        8, 0, 0, 0,
                                                        4, 0, 0, 0,
                                                        2, 0, 0, 0,
                                                        1, 0, 0, 0}};


// 2^(130) - 5: used by Poly1305
template<>
const Bignum<17>::_Word Bignum<17>::_mod = {{ 0xfb, 0xff, 0xff, 0xff,
                                              0xff, 0xff, 0xff, 0xff,
                                              0xff, 0xff, 0xff, 0xff,
                                              0xff, 0xff, 0xff, 0xff,
                                              0x03, 0x00, 0x00, 0x00 }};

// 0x400000000000000000000000000000005000000000000000
template<>
const Bignum<17>::_Barrett Bignum<17>::_barrett_u = {{ 0x00, 0x00, 0x00, 0x00,
                                                       0x00, 0x00, 0x00, 0x50,
                                                       0x00, 0x00, 0x00, 0x00,
                                                       0x00, 0x00, 0x00, 0x00,
                                                       0x00, 0x00, 0x00, 0x00,
                                                       0x00, 0x00, 0x00, 0x40 }};
__END_UTIL
