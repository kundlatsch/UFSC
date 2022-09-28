// EPOS AES Utility Declarations

#ifndef __util_aes_h
#define __util_aes_h

#include <utility/string.h>
#define __aes_common_only__
#include <machine/aes.h>
#undef __aes_common_only__

__BEGIN_UTIL

// EPOS 128-bit Advanced Encryption Standard (AES) Software Implementation
// Adapted from https://github.com/kokke/tiny-AES128-C
template<>
class SWAES<16>: public AES_Common
{
private:
    static const unsigned int Nb = 4; // number of columns comprising a _state
    static const unsigned int Nk = 4; // number of 32 bit words in a key
    static const int Nr = 10; // number of rounds in AES cipher

    typedef unsigned char State[4][4]; // array holding the intermediate results during decryption

public:
    static const unsigned int KEY_SIZE = 16;

public:
    SWAES(const Mode & m = ECB): _mode(m) {
        assert((m == ECB) || (m == CBC));
        for(unsigned int i = 0; i < 23; i++)
            iv[i] = 0;
    }

    Mode mode() { return _mode; }

    void encrypt(const unsigned char * data, const unsigned char * key, unsigned char * result) { crypt(data, key, result, true); }
    void decrypt(const unsigned char * data, const unsigned char * key, unsigned char * result) { crypt(data, key, result, false); }

private:
    void mode(const Mode & m) {
        assert((m == ECB) || (m == CBC));
        _mode = m;
    }

    void crypt(const unsigned char * data, const unsigned char * key, unsigned char * result, bool encrypt) {
        db<Ciphers>(TRC) << "AES::" << (encrypt ? "en" : "de") << "crypt(data=" << data << ",key=" << key << ",result=" << result << endl;
        db<Ciphers>(INF) << "AES::" << (encrypt ? "en" : "de") << "crypt:data = {" << int(data[0]);
        for(unsigned int i = 1; i < 16; i++)
            db<Ciphers>(INF) << "," << int(data[i]);
        db<Ciphers>(INF) << "}" << endl;
        db<Ciphers>(INF) << "AES::" << (encrypt ? "en" : "de") << "crypt:key = {" << int(key[0]);
        for(unsigned int i = 1; i < 16; i++)
            db<Ciphers>(INF) << "," << int(key[i]);
        db<Ciphers>(INF) << "}" << endl;

        switch(_mode) {
        case CBC:
            if(encrypt)
                cbc_encrypt_buffer(result, data, 16, key, iv);
            else
                cbc_decrypt_buffer(result, data, 16, key, iv);
            break;
        case ECB:
            if(encrypt)
                ebc_encrypt(data, key, result);
            else
                ebc_decrypt(data, key, result);
            break;
        }

        db<Ciphers>(INF) << "AES::" << (encrypt ? "en" : "de") << "crypt:result = {" << int(result[0]);
        for(unsigned int i = 1; i < 16; i++)
            db<Ciphers>(INF) << "," << int(result[i]);
        db<Ciphers>(INF) << "}" << endl;
    }

    void cbc_encrypt_buffer(unsigned char * output, const unsigned char * input, int length, const unsigned char * key, unsigned char * iv);
    void cbc_decrypt_buffer(unsigned char * output, const unsigned char * input, int length, const unsigned char * key, unsigned char * iv);
    void ebc_encrypt(const unsigned char * input, const unsigned char * key, unsigned char *output);
    void ebc_decrypt(const unsigned char * input, const unsigned char * key, unsigned char *output);

    void expand_key();
    void add_round_key(int round);
    void sub_bytes();
    void shift_rows();
    void mix_columns();
    void inv_mix_columns();
    void inv_sub_bytes();
    void inv_shift_rows();

    void cipher();
    void inv_cipher();

    void block_copy(unsigned char * output, const unsigned char * input) { memcpy(output, input, KEY_SIZE); }

    unsigned char xtime(unsigned char x) { return ((x<<1) ^ (((x>>7) & 1) * 0x1b)); }
    unsigned char multiply(unsigned char x, unsigned char y) {
        return (((y & 1) * x) ^
                ((y >> 1 & 1) * xtime(x)) ^
                ((y >> 2 & 1) * xtime(xtime(x))) ^
                ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
                ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))));
    }

    void xor_with_iv(unsigned char * buf) { for(unsigned int i = 0; i < KEY_SIZE; ++i) buf[i] ^= _iv[i]; }

private:
    Mode _mode;

    State * _state;
    unsigned char _round_key[176];
    const unsigned char * _key;
    unsigned char * _iv; // initial Vector used only for CBC mode
    unsigned char iv[23];

    static const unsigned char sbox[256];
    static const unsigned char rsbox[256];
    static const unsigned char rcon[255];
};

__END_UTIL

#endif
