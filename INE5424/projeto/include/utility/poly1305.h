// EPOS Poly1305-AES Message Authentication Code Component Declarations

#ifndef __poly1305_h
#define __poly1305_h

#include <utility/string.h>
#include <utility/bignum.h>
#include <utility/math.h>

__BEGIN_UTIL

template<typename Cipher>
class Poly1305
{
    typedef _UTIL::Bignum<17> Bignum;

public:
    Poly1305(const unsigned char k[16], const unsigned char r[16]) : _k(k, 16), _r(r, 16) {
        clamp();
    }
    Poly1305() {}

    void stamp(unsigned char out[16], const unsigned char nonce[16], const unsigned char * message, int message_len) {
        // cr = (c_1 * r^q + c_2 * r^(q-1) + ... + c_q * r^1) % (2^130 - 5)
        Bignum cr(0);
        for(; message_len > 0; message_len -= 16, message += 16) {
            int len = Math::min(16, message_len);
            Bignum c(message, len);
            reinterpret_cast<unsigned char *>(c._data)[len] = 1;

            cr += c;
            cr *= _r;
        }

        unsigned char ciphertext[16];
        Cipher cipher;
        cipher.encrypt(nonce, reinterpret_cast<const unsigned char *>(_k._data), ciphertext);

        // out = (cr + aes(k,n)) % 2^128
        Bignum::simple_add(reinterpret_cast<Bignum::Digit *>(out), reinterpret_cast<const Bignum::Digit *>(ciphertext), cr._data, 4);
    }

    bool verify(const unsigned char mac[16], const unsigned char nonce[16], const unsigned char * message, unsigned int message_len) {
        unsigned char my_mac[16];
        stamp(my_mac, nonce, message, message_len);
        for(int i = 0; i < 16; i++) {
            if(my_mac[i] != mac[i])
                return false;
        }
        return true;
    }

    void k(const unsigned char k1[16]) { new (&_k) Bignum(k1,16); }
    void r(const unsigned char r1[16]) { new (&_r) Bignum(r1,16); clamp(); }

private:
    void clamp() {
        reinterpret_cast<unsigned char *>(_r._data)[3] &= 15;
        reinterpret_cast<unsigned char *>(_r._data)[7] &= 15;
        reinterpret_cast<unsigned char *>(_r._data)[11] &= 15;
        reinterpret_cast<unsigned char *>(_r._data)[15] &= 15;
        reinterpret_cast<unsigned char *>(_r._data)[4] &= 252;
        reinterpret_cast<unsigned char *>(_r._data)[8] &= 252;
        reinterpret_cast<unsigned char *>(_r._data)[12] &= 252;
    }

    Bignum _k;
    Bignum _r;
};

__END_UTIL

#endif
