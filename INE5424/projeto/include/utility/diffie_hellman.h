// EPOS Elliptic Curve Diffie-Hellman (ECDH) Component Declarations

#ifndef __diffie_hellman_h
#define __diffie_hellman_h

#include <system/config.h>
#include <utility/bignum.h>

__BEGIN_UTIL

template<typename Cipher>
class Diffie_Hellman
{
public:
    static const unsigned int SECRET_SIZE = Cipher::KEY_SIZE;
    static const unsigned int PUBLIC_KEY_SIZE = 2 * SECRET_SIZE;

private:
    typedef _UTIL::Bignum<SECRET_SIZE> Bignum;

    class Elliptic_Curve_Point
    {
    public:
        typedef typename Diffie_Hellman::Bignum Coordinate;

        Elliptic_Curve_Point() __attribute__((noinline)) {}
        Elliptic_Curve_Point(const Coordinate & _x, const Coordinate & _y, const Coordinate & _z) { x = _x; y = _y; z = _z; }

        void operator*=(const Coordinate & b);

        friend Debug &operator<<(Debug &out, const Elliptic_Curve_Point &a) {
            out << "{x=" << a.x << ",y=" << a.y << ",z=" << a.z << "}";
            return out;
        }
        friend OStream &operator<<(OStream &out, const Elliptic_Curve_Point &a) {
            out << "{x=" << a.x << ",y=" << a.y << ",z=" << a.z << "}";
            return out;
        }

    private:
        void jacobian_double();
        void add_jacobian_affine(const Elliptic_Curve_Point &b);

    public:
        Coordinate x, y, z;
    };

public:
    typedef Elliptic_Curve_Point Public_Key;
    typedef Bignum Shared_Key;
    typedef Bignum Private_Key;

    Diffie_Hellman() {
        new (&_base_point.x) Bignum(_default_base_point_x, SECRET_SIZE);
        new (&_base_point.y) Bignum(_default_base_point_y, SECRET_SIZE);
        _base_point.z = 1;
        generate_keypair();
    }

    Diffie_Hellman(const Elliptic_Curve_Point & base_point): _base_point(base_point) {
        generate_keypair();
    }

    Elliptic_Curve_Point public_key() { return _public; }

    Shared_Key shared_key(Elliptic_Curve_Point public_key) {
        db<Diffie_Hellman>(TRC) << "Diffie_Hellman::shared_key(pub=" << public_key << ",priv=" << _private << ")" << endl;

        public_key *= _private;
        public_key.x ^= public_key.y;

        db<Diffie_Hellman>(INF) << "Diffie_Hellman: shared key = " << public_key.x << endl;
        return public_key.x;
    }

private:
    void generate_keypair() {
        db<Diffie_Hellman>(TRC) << "Diffie_Hellman::generate_keypair()" << endl;

        _private.randomize();

        db<Diffie_Hellman>(INF) << "Diffie_Hellman Private: " << _private << endl;
        db<Diffie_Hellman>(INF) << "Diffie_Hellman Base Point: " << _base_point << endl;

        _public = _base_point;
        _public *= _private;

        db<Diffie_Hellman>(INF) << "Diffie_Hellman Public: " << _public << endl;
    }

private:
    Private_Key _private;
    Elliptic_Curve_Point _base_point;
    Elliptic_Curve_Point _public;
    static const char _default_base_point_x[SECRET_SIZE];
    static const char _default_base_point_y[SECRET_SIZE];
};

//TODO: base point is dependent of SECRET_SIZE
template<typename Cipher>
const char Diffie_Hellman<Cipher>::_default_base_point_x[SECRET_SIZE] =
{
 '\x86', '\x5B', '\x2C', '\xA5',
 '\x7C', '\x60', '\x28', '\x0C',
 '\x2D', '\x9B', '\x89', '\x8B',
 '\x52', '\xF7', '\x1F', '\x16'
};

template<typename Cipher>
const char Diffie_Hellman<Cipher>::_default_base_point_y[SECRET_SIZE] =
{
 '\x83', '\x7A', '\xED', '\xDD',
 '\x92', '\xA2', '\x2D', '\xC0',
 '\x13', '\xEB', '\xAF', '\x5B',
 '\x39', '\xC8', '\x5A', '\xCF'
};

template<typename Cipher>
void Diffie_Hellman<Cipher>::Elliptic_Curve_Point::operator*=(const Coordinate & b)
{
    // Finding last '1' bit of b
    static const unsigned int bits_in_digit = sizeof(typename Coordinate::Digit) * 8;

    typename Coordinate::Digit now;
    unsigned int b_len = sizeof(Coordinate) / sizeof(typename Coordinate::Digit);
    for(; (b_len > 1) && (b[b_len - 1] == 0); b_len--);
    if((b_len == 0) || (b[b_len - 1] == 0)) {
        x = 0;
        y = 0;
        z = 0;
        return;
    }

    now = b[b_len - 1];

    bool bin[bits_in_digit]; // Binary representation of 'now'
    unsigned int current_bit = bits_in_digit;

    Elliptic_Curve_Point pp(*this);

    for(int i = bits_in_digit - 1; i >= 0; i--) {
        if(now % 2)
            current_bit = i + 1;
        bin[i] = now % 2;
        now /= 2;
    }

    for(int i = b_len - 1; i >= 0; i--) {
        for(; current_bit < bits_in_digit; current_bit++) {
            jacobian_double();
            if(bin[current_bit])
                add_jacobian_affine(pp);
        }
        if(i > 0) {
            now = b[i-1];
            for(int j = bits_in_digit-1; j >= 0; j--) {
                bin[j] = now % 2;
                now /= 2;
            }
            current_bit = 0;
        }
    }

    Coordinate Z;
    z.invert();
    Z = z;
    Z *= z;

    x *= Z;
    Z *= z;

    y *= Z;
    z = 1;
}

template<typename Cipher>
void Diffie_Hellman<Cipher>::Elliptic_Curve_Point::jacobian_double()
{
    Coordinate B, C(x), aux(z);

    aux *= z; C -= aux;
    aux += x; C *= aux;
    C *= 3;

    z *= y; z *= 2;

    y *= y; B = y;

    y *= x; y *= 4;

    B *= B; B *= 8;

    x = C; x *= x;
    aux = y; aux *= 2;
    x -= aux;

    y -= x; y *= C;
    y -= B;
}

template<typename Cipher>
void Diffie_Hellman<Cipher>::Elliptic_Curve_Point::add_jacobian_affine(const Elliptic_Curve_Point &b)
{
    Coordinate A(z), B, C, X, Y, aux, aux2;

    A *= z;

    B = A;

    A *= b.x;

    B *= z; B *= b.y;

    C = A; C -= x;

    B -= y;

    X = B; X *= B;
    aux = C; aux *= C;

    Y = aux;

    aux2 = aux; aux *= C;
    aux2 *= 2; aux2 *= x;
    aux += aux2; X -= aux;

    aux = Y; Y *= x;
    Y -= X; Y *= B;
    aux *= y; aux *= C;
    Y -= aux;

    z *= C;

    x = X; y = Y;
}



__END_UTIL

#endif
