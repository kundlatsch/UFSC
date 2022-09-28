// EPOS AES Utility Implementation

#include <utility/aes.h>

__BEGIN_UTIL

// The lookup-tables are marked const so they can be placed in read-only storage instead of RAM
// The numbers below can be computed dynamically trading ROM for RAM -
// This can be useful in (embedded) bootloader applications, where ROM is often limited.
const unsigned char SWAES<16>::sbox[256] = {
	// 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
	   0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	   0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	   0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	   0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	   0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	   0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	   0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	   0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	   0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	   0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	   0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	   0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	   0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	   0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	   0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	   0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

const unsigned char SWAES<16>::rsbox[256] = {
	// 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
           0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	   0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	   0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	   0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	   0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	   0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	   0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	   0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	   0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	   0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	   0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	   0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	   0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	   0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	   0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	   0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };


// The round constant word array, rcon[i], contains the values given by
// x to th e power (i-1) being powers of x (x is denoted as {02}) in the field GF(2^8)
// Note that i starts at 1, not 0).
const unsigned char SWAES<16>::rcon[255] = {
	// 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
           0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
	   0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
	   0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
	   0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
	   0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
	   0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
	   0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
	   0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
	   0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
	   0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
	   0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
	   0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
	   0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
	   0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
	   0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
	   0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };

void SWAES<16>::ebc_encrypt(const unsigned char * input, const unsigned char * key, unsigned char * output)
{
    // Copy input to output, and work in-memory on output
    block_copy(output, input);
    _state = reinterpret_cast<State *>(output);

    _key = key;
    expand_key();

    // The next function call encrypts the PlainText with the _key using AES algorithm.
    cipher();
}

void SWAES<16>::ebc_decrypt(const unsigned char * input, const unsigned char * key, unsigned char *output)
{
    // Copy input to output, and work in-memory on output
    block_copy(output, input);
    _state = reinterpret_cast<State *>(output);

    // The expand_key routine must be called before encryption.
    _key = key;
    expand_key();

    inv_cipher();
}

void SWAES<16>::cbc_encrypt_buffer(unsigned char * output, const unsigned char * _input, int length, const unsigned char * key, unsigned char * iv)
{
    unsigned char remainders = length % KEY_SIZE; /* Remaining bytes in the last non-full block */

    block_copy(output, _input);
    _state = reinterpret_cast<State *>(output);

    // Skip the key expansion if key is passed as 0
    if(0 != key) {
        _key = key;
        expand_key();
    }

    if(iv != 0)
        _iv = iv;

    int i;
    for(i = 0; i < length; i += KEY_SIZE) {
        unsigned char input[KEY_SIZE];
        for(int j=0; (j<(int)KEY_SIZE) && (j+i < length); j++)
            input[j] = _input[j+i];
        xor_with_iv(input);
        block_copy(output, input);
        _state = reinterpret_cast<State *>(output);
        cipher();
        _iv = output;
        output += KEY_SIZE;
    }

    if(remainders) {
        block_copy(output, _input+i);
        memset(output + remainders, 0, KEY_SIZE - remainders); /* add 0-padding */
        _state = reinterpret_cast<State *>(output);
        cipher();
    }
}

void SWAES<16>::cbc_decrypt_buffer(unsigned char * output, const unsigned char * input, int length, const unsigned char * key, unsigned char * iv)
{
    int i;
    unsigned char remainders = length % KEY_SIZE; /* Remaining bytes in the last non-full block */

    block_copy(output, input);
    _state = reinterpret_cast<State *>(output);

    // Skip the key expansion if key is passed as 0
    if(0 != key) {
        _key = key;
        expand_key();
    }

    // If iv is passed as 0, we continue to encrypt without re-setting the _iv
    if(iv != 0)
        _iv = iv;

    for(i = 0; i < length; i += KEY_SIZE) {
        block_copy(output, input);
        _state = reinterpret_cast<State *>(output);
        inv_cipher();
        xor_with_iv(output);
        _iv = const_cast<unsigned char *>(input);
        input += KEY_SIZE;
        output += KEY_SIZE;
    }

    if(remainders) {
        block_copy(output, input);
        memset(output+remainders, 0, KEY_SIZE - remainders); /* add 0-padding */
        _state = reinterpret_cast<State *>(output);
        inv_cipher();
    }
}

// This function produces Nb(Nr+1) round keys. The round keys are used in each round to decrypt the states.
void SWAES<16>::expand_key(void)
{
    unsigned int i, j, k;
    unsigned char tempa[4]; // Used for the column/row operations

    // The first round key is the key itself.
    for(i = 0; i < Nk; ++i) {
        _round_key[(i * 4) + 0] = _key[(i * 4) + 0];
        _round_key[(i * 4) + 1] = _key[(i * 4) + 1];
        _round_key[(i * 4) + 2] = _key[(i * 4) + 2];
        _round_key[(i * 4) + 3] = _key[(i * 4) + 3];
    }

    // All other round keys are found from the previous round keys.
    for(; (i < (Nb * (Nr + 1))); ++i) {
        for(j = 0; j < 4; ++j)
            tempa[j]=_round_key[(i-1) * 4 + j];
        if (i % Nk == 0) {
            // This function rotates the 4 bytes in a word to the left once.
            // [a0,a1,a2,a3] becomes [a1,a2,a3,a0]

            // Function RotWord()
            {
                k = tempa[0];
                tempa[0] = tempa[1];
                tempa[1] = tempa[2];
                tempa[2] = tempa[3];
                tempa[3] = k;
            }

            // SubWord() is a function that takes a four-byte input word and
            // applies the S-box to each of the four bytes to produce an output word.

            // Function Subword()
            {
                tempa[0] = sbox[static_cast<int>(tempa[0])];
                tempa[1] = sbox[static_cast<int>(tempa[1])];
                tempa[2] = sbox[static_cast<int>(tempa[2])];
                tempa[3] = sbox[static_cast<int>(tempa[3])];
            }

            tempa[0] =  tempa[0] ^ rcon[i/Nk];
        }
        else if (Nk > 6 && i % Nk == 4) {
            // Function Subword()
            {
                tempa[0] = sbox[static_cast<int>(tempa[0])];
                tempa[1] = sbox[static_cast<int>(tempa[1])];
                tempa[2] = sbox[static_cast<int>(tempa[2])];
                tempa[3] = sbox[static_cast<int>(tempa[3])];
            }
        }
        _round_key[i * 4 + 0] = _round_key[(i - Nk) * 4 + 0] ^ tempa[0];
        _round_key[i * 4 + 1] = _round_key[(i - Nk) * 4 + 1] ^ tempa[1];
        _round_key[i * 4 + 2] = _round_key[(i - Nk) * 4 + 2] ^ tempa[2];
        _round_key[i * 4 + 3] = _round_key[(i - Nk) * 4 + 3] ^ tempa[3];
    }
}

// This function adds the round key to _state.
// The round key is added to the _state by an XOR function.
void SWAES<16>::add_round_key(int round)
{
    int i,j;
    for(i=0;i<4;++i) {
        for(j = 0; j < 4; ++j) {
            (*_state)[i][j] ^= _round_key[round * Nb * 4 + i * Nb + j];
        }
    }
}

// The sub_bytes Function Substitutes the values in the
// _state matrix with values in an S-box.
void SWAES<16>::sub_bytes(void)
{
    int i, j;
    for(i = 0; i < 4; ++i) {
        for(j = 0; j < 4; ++j) {
            (*_state)[j][i] = sbox[static_cast<int>((*_state)[j][i])];
        }
    }
}

// The shift_rows() function shifts the rows in the _state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
void SWAES<16>::shift_rows(void)
{
    unsigned char temp;

    // Rotate first row 1 columns to left
    temp            = (*_state)[0][1];
    (*_state)[0][1] = (*_state)[1][1];
    (*_state)[1][1] = (*_state)[2][1];
    (*_state)[2][1] = (*_state)[3][1];
    (*_state)[3][1] = temp;

    // Rotate second row 2 columns to left
    temp            = (*_state)[0][2];
    (*_state)[0][2] = (*_state)[2][2];
    (*_state)[2][2] = temp;

    temp            = (*_state)[1][2];
    (*_state)[1][2] = (*_state)[3][2];
    (*_state)[3][2] = temp;

    // Rotate third row 3 columns to left
    temp            = (*_state)[0][3];
    (*_state)[0][3] = (*_state)[3][3];
    (*_state)[3][3] = (*_state)[2][3];
    (*_state)[2][3] = (*_state)[1][3];
    (*_state)[1][3] = temp;
}

// mix_columns function mixes the columns of the _state matrix
void SWAES<16>::mix_columns(void)
{
    int i;
    unsigned char Tmp,Tm,t;
    for(i = 0; i < 4; ++i) {
        t   = (*_state)[i][0];
        Tmp = (*_state)[i][0] ^ (*_state)[i][1] ^ (*_state)[i][2] ^ (*_state)[i][3] ;
        Tm  = (*_state)[i][0] ^ (*_state)[i][1] ; Tm = xtime(Tm);  (*_state)[i][0] ^= Tm ^ Tmp ;
        Tm  = (*_state)[i][1] ^ (*_state)[i][2] ; Tm = xtime(Tm);  (*_state)[i][1] ^= Tm ^ Tmp ;
        Tm  = (*_state)[i][2] ^ (*_state)[i][3] ; Tm = xtime(Tm);  (*_state)[i][2] ^= Tm ^ Tmp ;
        Tm  = (*_state)[i][3] ^ t ;        Tm = xtime(Tm);  (*_state)[i][3] ^= Tm ^ Tmp ;
    }
}

// mix_columns function mixes the columns of the _state matrix.
// The method used to multiply may be difficult to understand for the inexperienced.
// Please use the references to gain more information.
void SWAES<16>::inv_mix_columns(void)
{
    int i;
    unsigned char a,b,c,d;
    for(i = 0; i < 4; ++i) {
        a = (*_state)[i][0];
        b = (*_state)[i][1];
        c = (*_state)[i][2];
        d = (*_state)[i][3];

        (*_state)[i][0] = multiply(a, 0x0e) ^ multiply(b, 0x0b) ^ multiply(c, 0x0d) ^ multiply(d, 0x09);
        (*_state)[i][1] = multiply(a, 0x09) ^ multiply(b, 0x0e) ^ multiply(c, 0x0b) ^ multiply(d, 0x0d);
        (*_state)[i][2] = multiply(a, 0x0d) ^ multiply(b, 0x09) ^ multiply(c, 0x0e) ^ multiply(d, 0x0b);
        (*_state)[i][3] = multiply(a, 0x0b) ^ multiply(b, 0x0d) ^ multiply(c, 0x09) ^ multiply(d, 0x0e);
    }
}


// The sub_bytes Function Substitutes the values in the
// _state matrix with values in an S-box.
void SWAES<16>::inv_sub_bytes(void)
{
    int i,j;
    for(i=0;i<4;++i) {
        for(j=0;j<4;++j) {
            (*_state)[j][i] = rsbox[static_cast<int>((*_state)[j][i])];
        }
    }
}

void SWAES<16>::inv_shift_rows(void)
{
    unsigned char temp;

    // Rotate first row 1 columns to right
    temp            = (*_state)[3][1];
    (*_state)[3][1] = (*_state)[2][1];
    (*_state)[2][1] = (*_state)[1][1];
    (*_state)[1][1] = (*_state)[0][1];
    (*_state)[0][1] = temp;

    // Rotate second row 2 columns to right
    temp            = (*_state)[0][2];
    (*_state)[0][2] = (*_state)[2][2];
    (*_state)[2][2] = temp;

    temp            = (*_state)[1][2];
    (*_state)[1][2] = (*_state)[3][2];
    (*_state)[3][2] = temp;

    // Rotate third row 3 columns to right
    temp            = (*_state)[0][3];
    (*_state)[0][3] = (*_state)[1][3];
    (*_state)[1][3] = (*_state)[2][3];
    (*_state)[2][3] = (*_state)[3][3];
    (*_state)[3][3] = temp;
}


// cipher is the main function that encrypts the PlainText.
void SWAES<16>::cipher(void)
{
    unsigned char round = 0;

    // Add the First round key to the _state before starting the rounds.
    add_round_key(0);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr-1 rounds are executed in the loop below.
    for(round = 1; round < Nr; ++round) {
        sub_bytes();
        shift_rows();
        mix_columns();
        add_round_key(round);
    }

    // The last round is given below.
    // The mix_columns function is not here in the last round.
    sub_bytes();
    shift_rows();
    add_round_key(Nr);
}

void SWAES<16>::inv_cipher(void)
{
    unsigned char round=0;

    // Add the First round key to the _state before starting the rounds.
    add_round_key(Nr);

    // There will be Nr rounds.
    // The first Nr-1 rounds are identical.
    // These Nr-1 rounds are executed in the loop below.
    for(round = Nr - 1; round > 0; round--) {
        inv_shift_rows();
        inv_sub_bytes();
        add_round_key(round);
        inv_mix_columns();
    }

    // The last round is given below.
    // The mix_columns function is not here in the last round.
    inv_shift_rows();
    inv_sub_bytes();
    add_round_key(0);
}

__END_UTIL
