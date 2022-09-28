// EPOS Math Utility Declarations

#ifndef __math_h
#define __math_h

#include <system/config.h>

__BEGIN_UTIL

namespace Math
{

constexpr double pi() { return 3.14159265358979323846264338327950288; }
constexpr float e() { return 2.7182818284590452354; }

template <typename T>
inline T logf(T num, float base = e(), float epsilon = 1e-12)
{
    if(num == 0) return 1;

    if(num < 1  && base < 1) return 0;

    T integer = 0;
    while(num < 1) {
        integer--;
        num *= base;
    }

    while(num >= base) {
        integer++;
        num /= base;
    }

    T partial = 0.5;
    num *= num;
    T decimal = 0.0;
    while(partial > epsilon) {
        if(num >= base) {
            decimal += partial;
            num /= base;
        }
        partial *= 0.5;
        num *= num;
    }

    return integer + decimal;
}

template <typename T>
inline T sqrt(T x)
{
    T res = 0;

    // "one" starts at the highest power of four <= than the argument.
    T one = static_cast<T>(1) << (sizeof(T) * 8 - 2);
    while(one > x)
        one >>= 2;

    while(one != 0) {
        if(x >= res + one) {
            x -= (res + one);
            res += 2 * one;
        }
        res >>= 1;
        one >>= 2;
    }
    return res;
}

template <typename T>
inline T pow(T x, unsigned int y)
{
    if(y == 0)
        return 1;
    else if(y == 1)
        return x;
    else {
        T tmp = pow(x, y / 2);
        if((y % 2) == 0)
            return tmp * tmp;
        else
            return x * tmp * tmp;
    }
}

inline float fast_log2(float val)
{
    int * const exp_ptr = reinterpret_cast <int *> (&val);
    int x = *exp_ptr;
    const int log_2 = ((x >> 23) & 255) - 128;
    x &= ~(255 << 23);
    x += 127 << 23;
    (*exp_ptr) = x;

    val = ((-1.0f/3) * val + 2) * val - 2.0f/3;

    return (val + log_2);
}

inline float fast_log(float val)
{
    static const float ln_2 = 0.69314718;
    return (fast_log2(val) * ln_2);
}

template <typename T>
const T & min(const T & x, const T & y)
{
    return (x <= y) ? x : y;
}

template <typename T>
const T & max(const T & x, const T & y)
{
    return (x > y) ? x : y;
}

template <typename T>
T abs(const T & x)
{
    return (x > 0) ? x : -x;
}

template <typename T>
T sin(T x) {
    T cur = x;
    float acc = 1;
    T fact= 1;
    T pow = x;
    for (int i = 1; abs<T>(acc) > .000001 && i < 100; i++){
        fact *= ((2 * i) * (2 * i + 1));
        pow *= -1 * x * x;
        acc = pow / fact;
        cur += acc;
    }
    return cur;
}

// src: https://stackoverflow.com/a/2284969
template <typename T>
T cos(T x) {
    T t, s;
    int p;
    p = 0;
    s = 1;
    t = 1;
    while(abs<T>(static_cast<T>(t)/s) > .000001) {
        p++;
        t = (-t * x * x) / ((2 * p - 1) * (2 * p));
        s += t;
    }
    return s;
}

template <typename T>
T largest(const T array[], int size)
{
    T result = array[0];
    for(int i = 1; i < size; i++)
        if(array[i] > result)
            result = array[i];
    return result;
}

template <typename T>
T smallest(const T array[], int size)
{
    T result = array[0];
    for(int i = 1; i < size; i++)
        if(array[i] < result)
            result = array[i];
    return result;
}

template <typename T>
T mean(const T array[], int size)
{
    T sum = 0;
    for(int i = 0; i < size; i++)
        sum += array[i];
    return sum / size;
}

template <typename T>
T variance(const T array[], int size, const T & mean)
{
    T var = 0;
    for(int i = 0; i < size; i++) {
        T tmp = mean - array[i];
        var = var + (tmp * tmp);
    }
    return var / (size - 1);
}

// Babylonian power of ten helper
template<typename T>
T power_of_ten(int num)
{
    T rst = 1.0;
    if(num >= 0)
        for(int i = 0; i < num ; i++)
            rst *= 10.0;
    else
        for(int i = 0; i < (0 - num); i++)
            rst *= 0.1;
    return rst;
}

// Babylonian Square Root
template<typename T>
T babylonian_sqrt(const T & a)
{
    T z = a;
    T rst = 0.0;
    int max = 8;     // to define maximum digit
    int i;
    T j = 1.0;

    for(i = max ; i > 0 ; i--) {
        // value must be bigger then 0
        if(z - ((2 * rst) + (j * power_of_ten<T>(i))) * (j * power_of_ten<T>(i)) >= 0) {
            while(z - ((2 * rst) + (j * power_of_ten<T>(i))) * (j * power_of_ten<T>(i)) >= 0) {
                j++;
                if(j >= 10)
                    break;
            }
            j--; //correct the extra value by minus one to j
            z -= ((2 * rst) + (j * power_of_ten<T>(i))) * (j * power_of_ten<T>(i)); //find value of z

            rst += j * power_of_ten<T>(i);     // find sum of a

            j = 1.0;
        }
    }

    for(i = 0 ; i >= 0 - max ; i--){
        if(z - ((2 * rst) + (j * power_of_ten<T>(i))) * (j * power_of_ten<T>(i)) >= 0) {
            while(z - ((2 * rst) + (j * power_of_ten<T>(i))) * (j * power_of_ten<T>(i)) >= 0)
                j++;
            j--;
            z -= ((2 * rst) + (j * power_of_ten<T>(i))) * (j * power_of_ten<T>(i)); //find value of z

            rst += j * power_of_ten<T>(i);     // find sum of a
            j = 1.0;
        }
    }
    // find the number on each digit
    return rst;
}

// Greatest Common Divisor (Euclid's algorithm)
template <typename T1, typename T2>
inline T1 gcd(T1 a, T2 b)
{
    if(b == 0)
        return a;
    else
        return gcd(b, a % b);
}

// Least Common Multiple
template <typename T1, typename T2>
inline T1 lcm(T1 a, T2 b)
{
    T1 temp = gcd(a,b);
    return temp ? a * (b / temp) : 0;
}

}

__END_UTIL

#endif
