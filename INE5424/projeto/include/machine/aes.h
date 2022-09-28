// EPOS AES Mediator Common Package

#ifndef __aes_h
#define __aes_h

#include <system/config.h>

__BEGIN_SYS

class AES_Common
{
public:
    enum Mode {
        ECB,
        CBC,
    };

protected:
    AES_Common() {}

public:
    const Mode & mode();

    void encrypt(const unsigned char * data, const unsigned char * key, unsigned char * result);
    void decrypt(const unsigned char * data, const unsigned char * key, unsigned char * result);
};

__END_SYS

#endif

#if !defined(__AES_H) && !defined(__aes_common_only__)
// If the machine does not feature an AES accelerator, then use a software lib
#include <utility/aes.h>
__BEGIN_SYS
template<unsigned int KEY_SIZE>
using AES = SWAES<KEY_SIZE>;
__END_SYS
#endif

#if defined(__AES_H) && !defined(__aes_common_only__)
#include __AES_H
__BEGIN_SYS
template<unsigned int KEY_SIZE>
using AES = HWAES<KEY_SIZE>;
__END_SYS
#endif
