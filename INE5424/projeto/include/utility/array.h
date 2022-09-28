// EPOS Array Utility Declarations

#ifndef __array_h
#define __array_h

#include <utility/ostream.h>
#include <utility/string.h>

__BEGIN_UTIL

template<typename T, unsigned int N>
class Array
{
private:
    static const unsigned int SIZE = N * sizeof(T);

public:
    typedef T Object_Type;

public:
    Array() {}
    Array(const void * data, unsigned int size) { copy_and_pad(data, size); }
    template <typename U>
    Array(const U & o) { copy_and_pad(&o, sizeof(U)); }

    static constexpr unsigned int length() { return N; };

    Object_Type & operator[](const unsigned int i) { return _data[i]; }
    const Object_Type & operator[](const unsigned int i) const { return _data[i]; }
    operator const Object_Type *() const { return _data; }
    operator Object_Type *() { return _data; }

    template <typename U>
    bool operator==(const U & o) const { return (SIZE == sizeof(U)) && (!memcmp(&o, _data, SIZE)); }
    template <typename U>
    bool operator!=(const U & o) const { return !(*this == o); }

    unsigned int search(const Object_Type & obj) {
        unsigned int i = 0;
        for(; i < N; i++)
            if(_data[i] == obj)
                break;
        return i;
    }

    template<typename _T, unsigned int _N>
    friend OStream & operator<<(OStream & out, const Array<_T, _N> & array) {
        out << "[";
        for(unsigned int i = 0; i < N; i++) {
            out << array[i];
            if(i < N - 1)
                out << ",";
        }
        out << "]";
        return out;
    }

private:
    void copy_and_pad(const void * data, unsigned int size) {
        if(size > SIZE)
            memcpy(_data, data, SIZE);
        else {
            memset(_data, 0, SIZE);
            memcpy(_data, data, size);
        }
    }

private:
    T _data[N];
} __attribute__((packed));

template<typename T>
class Array<T, 0> {
public:
    static constexpr unsigned int length() { return 0; };
};

__END_UTIL

#endif
