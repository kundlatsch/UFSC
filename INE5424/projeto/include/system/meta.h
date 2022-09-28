// EPOS Basic Metaprograms

#ifndef __meta_h
#define __meta_h

__BEGIN_SYS

// Native type wrapper (POD)
template <typename T1>
class Native_Wrapper
{
public:
	typedef T1 Type;

public:
    Native_Wrapper() {}
    Native_Wrapper(const Native_Wrapper & value): _value(value._value) {}
    Native_Wrapper(const T1 & value): _value(value) {}
    template<typename T2>
    Native_Wrapper(const T2 & value): _value(T1(value)) {}

    operator T1() { return _value; }
    operator const T1 &() const { return _value; }
    operator T1() volatile { return _value; }

    template<typename T2>
    void operator =(T2 value) { _value = T1(value); }

    template<typename T2>
    bool operator==(T2 value) const { return (_value == T1(value)); }
    template<typename T2>
    bool operator!=(T2 value) const { return (_value != T1(value)); }
    template<typename T2>
    bool operator< (T2 value) const { return (_value < T1(value)); }
    template<typename T2>
    bool operator> (T2 value) const { return (_value > T1(value)); }
    template<typename T2>
    bool operator>=(T2 value) const { return (_value >= T1(value)); }
    template<typename T2>
    bool operator<=(T2 value) const { return (_value <= T1(value)); }

    template<typename T2>
    bool operator==(T2 value) volatile { return (_value == T1(value)); }
    template<typename T2>
    bool operator!=(T2 value) volatile { return (_value != T1(value)); }
    template<typename T2>
    bool operator< (T2 value) volatile { return (_value < T1(value)); }
    template<typename T2>
    bool operator> (T2 value) volatile { return (_value > T1(value)); }
    template<typename T2>
    bool operator>=(T2 value) volatile { return (_value >= T1(value)); }
    template<typename T2>
    bool operator<=(T2 value) volatile { return (_value <= T1(value)); }

    template<typename T2>
    Native_Wrapper operator+(T2 value) const { return _value + T1(value); }
    template<typename T2>
    Native_Wrapper operator-(T2 value) const { return _value - T1(value); }
    template<typename T2>
    Native_Wrapper operator*(T2 value) const { return _value * T1(value); }
    template<typename T2>
    Native_Wrapper operator/(T2 value) const { return _value / T1(value); }
    template<typename T2>
    Native_Wrapper operator%(T2 value) const { return _value % T1(value); }

    template<typename T2>
    Native_Wrapper operator+(T2 value) volatile { return _value + T1(value); }
    template<typename T2>
    Native_Wrapper operator-(T2 value) volatile { return _value - T1(value); }
    template<typename T2>
    Native_Wrapper operator*(T2 value) volatile { return _value * T1(value); }
    template<typename T2>
    Native_Wrapper operator/(T2 value) volatile { return _value / T1(value); }
    template<typename T2>
    Native_Wrapper operator%(T2 value) volatile { return _value % T1(value); }

    T1 operator++() { return ++_value; }
    T1 operator--() { return --_value; }
    T1 operator++(int value) { return _value++; }
    T1 operator--(int value) { return _value--; }

    template<typename T2>
    Native_Wrapper & operator+=(T2 value) { _value += T1(value); return *this; }
    template<typename T2>
    Native_Wrapper & operator-=(T2 value) { _value -= T1(value); return *this; }
    template<typename T2>
    Native_Wrapper & operator*=(T2 value) { _value *= T1(value); return *this; }
    template<typename T2>
    Native_Wrapper & operator/=(T2 value) { _value /= T1(value); return *this; }
    template<typename T2>
    Native_Wrapper & operator%=(T2 value) { _value %= T1(value); return *this; }

    template<typename T2>
    Native_Wrapper & operator&=(T2 value) { _value &= T1(value); return *this; }
    template<typename T2>
    Native_Wrapper & operator|=(T2 value) { _value |= T1(value); return *this; }

    Native_Wrapper & operator[](int i) { return *(this + i); }

    template<typename T3>
    friend T3 & operator<<(T3 & os, const Native_Wrapper & value) { os << value._value; return os; }

private:
    T1 _value;
};

// Conditional Type
template<bool condition, typename Then, typename Else>
struct IF
{ typedef Then Result; };

template<typename Then, typename Else>
struct IF<false, Then, Else>
{ typedef Else Result; };


// Conditional Integer
template<bool condition, int Then, int Else>
struct IF_INT
{ enum { Result = Then }; };

template<int Then, int Else>
struct IF_INT<false, Then, Else>
{ enum { Result = Else }; };


// SWITCH-CASE of Types
const int DEFAULT = ~(~0u >> 1); // Initialize with the smallest int

struct Nil_Case {};

template<int tag_, typename Type_, typename Next_ = Nil_Case>
struct CASE
{
    enum { tag = tag_ };
    typedef Type_ Type;
    typedef Next_ Next;
};

template<int tag, typename Case>
class SWITCH
{
    typedef typename Case::Next Next_Case;
    enum {
        case_tag = Case::tag,
        found = ( case_tag == tag || case_tag == DEFAULT  )
    };
public:
    typedef typename IF<found, typename Case::Type, typename SWITCH<tag, Next_Case>::Result>::Result Result;
};

template<int tag>
class SWITCH<tag, Nil_Case>
{
public:
    typedef Nil_Case Result;
};


// EQUALty of Types
template<typename T1, typename T2>
struct EQUAL
{ enum { Result = false }; };

template<typename T>
struct EQUAL<T, T>
{ enum { Result = true }; };

template<typename T1, typename T2>
struct MAX
{ typedef typename IF<sizeof(T1) >= sizeof(T2), T1, T2>::Result Result; };

template<typename T1, typename T2>
struct MIN
{ typedef typename IF<sizeof(T1) < sizeof(T2), T1, T2>::Result Result; };


// Constant Arrays
template<unsigned int N, typename T>
constexpr unsigned int COUNTOF(const T (&)[N]) { return N; }

template<unsigned int N, typename T>
constexpr bool INARRAY(const T (& array)[N], const T & value) {
    for(const T & v : array)
        if(value == v) return true;
    return false;
}


// SIZEOF Type Package
template<typename ... Tn>
struct SIZEOF
{ static const unsigned int  Result = 0; };
template<typename T1, typename ... Tn>
struct SIZEOF<T1, Tn ...>
{ static const unsigned int Result = sizeof(T1) + SIZEOF<Tn ...>::Result ; };


// LIST of Types
template<typename ... Tn> class LIST;
template<typename T1, typename ... Tn>
class LIST<T1, Tn ...>
{
protected:
    typedef T1 Head;
    typedef LIST<Tn ...> Tail;

public:
    enum { Length = Tail::Length + 1 };

    template<int Index, int Current = 0, bool Stop = (Index == Current)>
    struct Get
    { typedef typename Tail::template Get<Index, Current + 1>::Result Result; };

    template<int Index, int Current>
    struct Get<Index, Current, true>
    { typedef Head Result; };

    template<typename Type, int Start = 0, int Current = 0, bool Stop = ((Current >= Start) && EQUAL<Head, Type>::Result)>
    struct Find
    { enum { Result = Tail::template Find<Type, Start, Current + 1>::Result }; };

    template<typename Type, int Start, int Current>
    struct Find<Type, Start, Current, true>
    { enum { Result = Current }; };

    template<typename Type>
    struct Count
    { enum { Result = EQUAL<Head, Type>::Result + Tail::template Count<Type>::Result }; };

    struct Recur: public Tail::Recur::Result
    { typedef Head Result; };

    enum { Polymorphic = (int(Length) != int(Count<Head>::Result)) };
};

template<>
class LIST<>
{
public:
    enum { Length = 0 };

    template<int Index, int Current = 0>
    struct Get
    { typedef void Result; };

    template<typename Type, int Start = 0, int Current = 0>
    struct Find
    { enum { Result = -1 }; };

    template<typename Type>
    struct Count
    { enum { Result = 0 }; };

    struct Recur
    { class Result {}; };

    enum { Polymorphic = false };
};


// LIST of Templates
template<template<typename T> class ... Tn> class ALIST;
template<template<typename T> class T1, template<typename T> class T2, template<typename T> class ... Tn>
class ALIST<T1, T2, Tn ...>
{
public:
    enum { Length = ALIST<Tn ...>::Length + 1 };

    template<typename T>
    struct Recur: public T1<T>, public ALIST<T2, Tn ...>::template Recur<T>
    {
        void enter() { T1<T>::enter(); T2<T>::enter(); }
        void leave() { T1<T>::leave(); T2<T>::leave(); }
        static void static_enter() { T1<T>::static_enter(); T2<T>::static_enter(); }
        static void static_leave() { T1<T>::static_leave(); T2<T>::static_leave(); }
    };
};
template<template<typename T> class T1, template<typename T> class ... Tn>
class ALIST<T1, Tn ...>
{
public:
    enum { Length = ALIST<Tn ...>::Length + 1 };

    template<typename T>
    struct Recur: public T1<T>, public ALIST<Tn ...>::template Recur<T>
    {
        void enter() { T1<T>::enter(); }
        void leave() { T1<T>::leave(); }
        static void static_enter() { T1<T>::static_enter(); }
        static void static_leave() { T1<T>::static_leave(); }
    };
};

template<>
class ALIST<>
{
public:
    enum { Length = 0 };

    template<typename T>
    struct Recur
    {
        void enter() {}
        void leave() {}
        static void static_enter() {}
        static void static_leave() {}
    };
};


// Serializer
inline void SERIALIZE(char * buf, int index) {}

template<typename T>
void SERIALIZE(char * buf, int index, const T & a) {
    __builtin_memcpy(&buf[index], &a, sizeof(T));
}

template<typename T, typename ... Tn>
void SERIALIZE(char * buf, int index, const T & a, const Tn & ... an) {
    __builtin_memcpy(&buf[index], &a, sizeof(T));
    SERIALIZE(buf, index + sizeof(T), an ...);
}

// Deserializer
inline void DESERIALIZE(char * buf, int index) {}

template<typename T>
void DESERIALIZE(char * buf, int index, T && a) {
    __builtin_memcpy(&a, &buf[index], sizeof(T));
}

template<typename T, typename ... Tn>
void DESERIALIZE(char * buf, int index, T && a, Tn && ... an) {
    __builtin_memcpy(&a, &buf[index], sizeof(T));
    DESERIALIZE(buf, index + sizeof(T), an ...);
}


// Returns the UNSIGNED counterpart of primitive type T
template<typename T>
struct UNSIGNED {
    typedef void Result; // Type T not supported
};
template<> struct UNSIGNED<char> { typedef unsigned char Result; };
template<> struct UNSIGNED<short> { typedef unsigned short Result; };
template<> struct UNSIGNED<int> { typedef unsigned int Result; };
template<> struct UNSIGNED<long> { typedef unsigned long Result; };
template<> struct UNSIGNED<long long> { typedef unsigned long long Result; };
template<> struct UNSIGNED<unsigned char> { typedef unsigned char Result; };
template<> struct UNSIGNED<unsigned short> { typedef unsigned short Result; };
template<> struct UNSIGNED<unsigned int> { typedef unsigned int Result; };
template<> struct UNSIGNED<unsigned long> { typedef unsigned long Result; };
template<> struct UNSIGNED<unsigned long long> { typedef unsigned long long Result; };

// Returns a type one rank LARGER than primitive type T
template<typename T>
struct LARGER {
    typedef T Result; // if there is no larger type, return T
};
template<> struct LARGER<bool> { typedef short Result; };
template<> struct LARGER<char> { typedef short Result; };
template<> struct LARGER<short> { typedef int Result; };
template<> struct LARGER<int> { typedef long long Result; };
template<> struct LARGER<long> { typedef long long Result; };
template<> struct LARGER<unsigned char> { typedef unsigned short Result; };
template<> struct LARGER<unsigned short> { typedef unsigned int Result; };
template<> struct LARGER<unsigned int> { typedef unsigned long long Result; };
template<> struct LARGER<unsigned long> { typedef unsigned long long Result; };

__END_SYS

#endif
