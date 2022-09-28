// EPOS (Litte-endian) Big Numbers Utility Declarations

#ifndef __bignum_h
#define __bignum_h

#include <utility/random.h>

__BEGIN_UTIL

// This class implements a prime finite field (Fp or GF(p))
// It basically consists of (possibly) big numbers between 0 and a prime modulo, with + - * / operators
// Primarily meant to be used primarily by asymmetric cryptography (e.g. Diffie-Hellman)
template<unsigned int SIZE>
class Bignum
{
    template<typename Cipher> friend class Poly1305;

public:
    typedef unsigned int Digit;
    typedef unsigned long long Double_Digit;

    static const unsigned int DIGITS = (SIZE + sizeof(Digit) - 1) / sizeof(Digit);
    static const unsigned int BITS_PER_DIGIT = sizeof(Digit) * 8;

    typedef Digit Word[DIGITS];
    typedef Double_Digit Double_Word[DIGITS];

private:
    union _Word {
        unsigned char bytes[sizeof(Word)];
        Digit data[sizeof(Word) / sizeof(Digit)];
    };
    union _Barrett {
        unsigned char bytes[sizeof(Word) + sizeof(Digit)];
        Digit data[sizeof(Word) / sizeof(Digit)];
    };

public:
    Bignum(unsigned int n = 0) __attribute__((noinline)) {
        *this = n;
    }
    Bignum(const void * bytes, unsigned int len) {
        for(unsigned int i = 0, j = 0; i < DIGITS; i++) {
            _data[i] = 0;
            for(unsigned int k = 0; k < sizeof(Digit) && j < len; k++, j++)
                _data[i] += (reinterpret_cast<const unsigned char *>(bytes)[j] << (8 * k));
        }
    }

    bool is_even() { return !(_data[0] % 2); }

    operator unsigned int() { return _data[0]; }

    void operator^=(const Bignum &b) {
        for(unsigned int i = 0; i < DIGITS; i++)
            _data[i] ^= b._data[i];
    }

    void operator=(unsigned int n) {
        static const bool shift = sizeof(unsigned int) > sizeof(Digit);
        if(!shift) {
            _data[0] = n;
            for(unsigned int i = 1; i < DIGITS; i++)
                _data[i] = 0;
        } else {
            unsigned int i;
            for(i = 0; (n != 0) && ( i < DIGITS); i++) {
                _data[i] = n;
                n >>= shift ? BITS_PER_DIGIT : 0;
            }
            for(; i < DIGITS; i++)
                _data[i] = 0;
        }
    }

    void operator=(const Bignum & b) {
        for(unsigned int i = 0; i < DIGITS; i++)
            _data[i] = b._data[i];
    }

    const Digit& operator[](unsigned int i) const { return _data[i]; }

    bool operator==(const Bignum & b) const { return (cmp(_data, b._data, DIGITS) == 0); }
    bool operator!=(const Bignum & b) const { return (cmp(_data, b._data, DIGITS) != 0); }
    bool operator>=(const Bignum & b) const { return (cmp(_data, b._data, DIGITS) >= 0); }
    bool operator<=(const Bignum & b) const { return (cmp(_data, b._data, DIGITS) <= 0); }
    bool  operator>(const Bignum & b) const { return (cmp(_data, b._data, DIGITS) > 0); }
    bool  operator<(const Bignum & b) const { return (cmp(_data, b._data, DIGITS) < 0); }

    void operator*=(const Bignum & b) __attribute__((noinline)) { // _data = (_data * b._data) % _mod
        if(b == 1) return;

        if(Traits<Bignum>::hysterically_debugged) {
            db<Bignum>(TRC) << "Bignum::operator*=(this=" << *this << ",other=" << b << ",mod=[";
            for(unsigned int i = 0; i < DIGITS - 1; i++)
                db<Bignum>(TRC) << _mod.data[i] << ",";
            db<Bignum>(TRC) << _mod.data[DIGITS - 1] << "]) => ";
        }

        Digit mult_result[2 * DIGITS];
        simple_mult(mult_result, _data, b._data, DIGITS);
        barrett_reduction(_data, mult_result, DIGITS);

        if(Traits<Bignum>::hysterically_debugged)
            db<Bignum>(TRC) << *this << endl;
    }

    void operator+=(const Bignum &b)__attribute__((noinline)) { // _data = (_data + b._data) % _mod
        if(Traits<Bignum>::hysterically_debugged) {
            db<Bignum>(TRC) << "Bignum::operator+=(this=" << *this << ",other=" << b << ",mod=[";
            for(unsigned int i = 0; i < DIGITS - 1; i++)
                db<Bignum>(TRC) << _mod.data[i] << ",";
            db<Bignum>(TRC) << _mod.data[DIGITS - 1] << "]) => ";
        }

        if(simple_add(_data, _data, b._data, DIGITS))
            simple_sub(_data, _data, _mod.data, DIGITS);
        if(cmp(_data, _mod.data, DIGITS) >= 0)
            simple_sub(_data, _data, _mod.data, DIGITS);

        if(Traits<Bignum>::hysterically_debugged)
            db<Bignum>(TRC) << *this << endl;
    }

    void operator-=(const Bignum &b)__attribute__((noinline)) { // _data = (_data - b._data) % _mod
        if(Traits<Bignum>::hysterically_debugged) {
            db<Bignum>(TRC) << "Bignum::operator-=(this=" << *this << ",other=" << b << ",mod=[";
            for(unsigned int i = 0; i < DIGITS - 1; i++)
                db<Bignum>(TRC) << _mod.data[i] << ",";
            db<Bignum>(TRC) << _mod.data[DIGITS - 1] << "]) => ";
        }

        if(simple_sub(_data, _data, b._data, DIGITS))
            simple_add(_data, _data, _mod.data, DIGITS);

        if(Traits<Bignum>::hysterically_debugged)
            db<Bignum>(TRC) << *this << endl;
    }

    // Shift left (actually shift right, because of little endianness)
    // - Does not apply modulo
    // - Returns carry bit
    bool multiply_by_two(bool carry = 0) __attribute__((noinline))
    {
        if(Traits<Bignum>::hysterically_debugged && !carry) {
            db<Bignum>(TRC) << "Bignum::multiply_by_two(this=" << *this << ",mod=[";
            for(unsigned int i = 0; i < DIGITS - 1; i++)
                db<Bignum>(TRC) << _mod.data[i] << ",";
            db<Bignum>(TRC) << _mod.data[DIGITS-1] << "]) => ";
        }

        bool next_carry;
        for(unsigned int i = 0; i < DIGITS; i++) {
            next_carry = _data[i] >> (BITS_PER_DIGIT-1);
            _data[i] <<= 1;
            _data[i] += Digit(carry);
            carry = next_carry;
        }

        if(Traits<Bignum>::hysterically_debugged && !carry)
            db<Bignum>(TRC) << *this << endl;

        return carry;
    }

    // Shift right (actually shift left, because of little endianness)
    // - Does not apply modulo
    // - Returns carry bit
    bool divide_by_two(bool carry = 0) __attribute__((noinline))
    {
        if(Traits<Bignum>::hysterically_debugged && !carry) {
            db<Bignum>(TRC) << "Bignum::divide_by_two(this=" << *this << ",mod=[";
            for(unsigned int i = 0; i < DIGITS - 1; i++)
                db<Bignum>(TRC) << _mod.data[i] << ",";
            db<Bignum>(TRC) << _mod.data[DIGITS-1] << "]) => ";
        }

        bool next_carry;
        for(int i = DIGITS - 1; i >= 0; i--) {
            next_carry = _data[i] % 2;
            _data[i] >>= 1;
            _data[i] += (Digit)carry << (BITS_PER_DIGIT-1);
            carry = next_carry;
        }

        if(Traits<Bignum>::hysterically_debugged && !carry)
            db<Bignum>(TRC) << *this << endl;

        return carry;
    }

    void randomize() __attribute__((noinline)) { // Sets _data to a random number smaller than _mod
        int i;
        for(i = DIGITS - 1; i >= 0 && (_mod.data[i] == 0); i--)
            _data[i]=0;
        _data[i] = Random::random() % _mod.data[i];
        for(--i; i >= 0; i--)
            _data[i] = Random::random();
    }

    void invert() __attribute__((noinline)) { // _data = i, such that (_data * i) % _mod = 1
        Bignum A(1), u, v, zero(0);
        for(unsigned int i = 0; i < DIGITS; i++) {
            u._data[i] = _data[i];
            v._data[i] = _mod.data[i];
        }
        *this = 0;
        while(u != zero) {
            while(u.is_even()) {
                u.divide_by_two();
                if(A.is_even())
                    A.divide_by_two();
                else {
                    bool carry = simple_add(A._data, A._data, _mod.data, DIGITS);
                    A.divide_by_two(carry);
                }
            }
            while(v.is_even()) {
                v.divide_by_two();
                if(is_even())
                    divide_by_two();
                else {
                    bool carry = simple_add(_data, _data, _mod.data, DIGITS);
                    divide_by_two(carry);
                }
            }
            if(u >= v) {
                u -= v;
                A -= *this;
            } else {
                v-=u;
                *this -= A;
            }
        }
    }

    friend OStream &operator<<(OStream & out, const Bignum & b){
        unsigned int i;
        out << '[';
        for(i=0;i<DIGITS;i++) {
            out << (unsigned int)b._data[i];
            if(i < DIGITS-1)
                out << ", ";
        }
        out << "]";
        return out;
    }

    friend Debug &operator<<(Debug & out, const Bignum & b) {
        unsigned int i;
        out << '[';
        for(i = 0; i < DIGITS; i++) {
            out << (unsigned int)b._data[i];
            if(i < DIGITS - 1)
                out << ", ";
        }
        out << "]";
        return out;
    }

private:
    static int cmp(const Digit * a, const Digit * b, int size) { // a == b -> 0, a > b -> 1, a < b -> -1
        for(int i = size - 1; i >= 0; i--) {
            if(a[i] > b[i]) return 1;
            else if(a[i] < b[i]) return -1;
        }
        return 0;
    }

    // res = a - b
    // returns: borrow bit
    // -No modulo applied
    // -a, b and res are assumed to have size 'size'
    // -a, b, res are allowed to point to the same place
    static bool simple_sub(Digit * res, const Digit * a, const Digit * b, unsigned int size) __attribute__((noinline)) {
        Double_Digit borrow = 0;
        Double_Digit aux = Double_Digit(1) << BITS_PER_DIGIT;
        for(unsigned int i = 0; i < size; i++) {
            Double_Digit anow = a[i];
            Double_Digit bnow = Double_Digit(b[i]) + borrow;
            borrow = (anow < bnow);
            res[i] = borrow ? (aux - bnow) + anow : anow - bnow;
        }
        return borrow;
    }

    // res = a + b
    // returns: carry bit
    // -No modulo applied
    // -a, b and res are assumed to have size 'size'
    // -a, b, res are allowed to point to the same place
    static bool simple_add(Digit * res, const Digit * a, const Digit * b, unsigned int size) __attribute__((noinline)) {
        bool carry = 0;
        for(unsigned int i = 0; i < size; i++) {
            Double_Digit tmp = Double_Digit(carry) + Double_Digit(a[i]) + Double_Digit(b[i]);
            res[i] = tmp;
            carry = tmp >> BITS_PER_DIGIT;
        }
        return carry;
    }

    // res = (a * b)
    // - Does not apply module
    // - a and b are assumed to be of size 'size'
    // - res is assumed to be of size '2*size'
    static void simple_mult(Digit * res, const Digit * a, const Digit * b, unsigned int size) {
        unsigned int i;
        Double_Digit r0 = 0, r1 = 0, r2 = 0;
        for(i = 0; i < size * 2 - 1; i++) {
            for(unsigned int j = 0; (j < size) && (j <= i); j++) {
                unsigned int k = i - j;
                if(k < size) {
                    Double_Digit aj = a[j];
                    Double_Digit bk = b[k];
                    Double_Digit prod = aj * bk;
                    r0 += Digit(prod);
                    r1 += (prod >> BITS_PER_DIGIT) + (r0 >> BITS_PER_DIGIT);
                    r0 = Digit(r0);
                    r2 += (r1 >> BITS_PER_DIGIT);
                    r1 = Digit(r1);
                }
            }
            res[i] = r0;
            r0 = r1;
            r1 = r2;
            r2 = 0;
        }
        res[i] = r0;
    }

    // res = a % _mod
    // - Intended to be used after a multiplication
    // - res is assumed to be of size 'size'
    // - a is assumed to be of size '2*size'
    void barrett_reduction(Digit * res, const Digit * a, unsigned int size) {
        Digit q[size + 1];

        // q = floor( ( floor( a/base^(size-1) ) * barrett_u ) / base^(size+1))
        Double_Digit r0 = 0, r1 = 0, r2 = 0;
        unsigned int i;
        for(i = 0; i < (2 * (size + 1)) - 1; i++) {
            for(unsigned int j = 0; (j < size + 1) && (j <= i); j++) {
                unsigned int k = i - j;
                if(k < size + 1) {
                    // shifting left (little endian) size-1 places
                    // a is assumed to have size size*2
                    Double_Digit aj = a[j + (size - 1)];
                    Double_Digit bk = _barrett_u.data[k];
                    Double_Digit prod = aj * bk;
                    r0 += Digit(prod);
                    r1 += (prod >> BITS_PER_DIGIT) + (r0 >> BITS_PER_DIGIT);
                    r0 = Digit(r0);
                    r2 += r1 >> BITS_PER_DIGIT;
                    r1 = Digit(r1);
                }
            }
            // shifting left (little endian) size+1 places
            if(i >= size + 1)
                q[i - (size + 1)] = r0;
            r0 = r1;
            r1 = r2;
            r2 = 0;
        }
        q[i - (size + 1)] = r0;

        Digit r[size + 1];
        // r = (q * _mod) % base^(size+1)
        r0 = 0, r1 = 0, r2 = 0;
        for(i = 0; i < size + 1; i++) {
            for(unsigned int j = 0; j <= i; j++) {
                unsigned int k = i - j;
                Double_Digit aj = q[j];
                Double_Digit bk = (k == size ? 0 : _mod.data[k]);
                Double_Digit prod = aj * bk;
                r0 += Digit(prod);
                r1 += (prod >> BITS_PER_DIGIT) + (r0 >> BITS_PER_DIGIT);
                r0 = Digit(r0);
                r2 += r1 >> BITS_PER_DIGIT;
                r1 = Digit(r1);
            }
            r[i] = r0;
            r0 = r1;
            r1 = r2;
            r2 = 0;
        }

        // r = ((a % base^(size+1)) - r) % base^(size+1)
        simple_sub(r, a, r, size+1);

        // _data = r % _mod
        while((r[size] > 0) || (cmp(r, _mod.data, size) >= 0)) {
            if(simple_sub(r, r, _mod.data, size))
                r[size]--;
        }

        for(unsigned int i = 0; i < size; i++)
            res[i] = r[i];
    }

private:
    Word _data;

    static const _Word _mod;
    static const _Barrett _barrett_u;
};

__END_UTIL

#endif
