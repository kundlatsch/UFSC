// EPOS Debug Utility Declarations

#include <utility/ostream.h>

#ifndef __debug_h
#define __debug_h

__BEGIN_UTIL

class Debug
{
public:
    template<typename T>
    Debug & operator<<(T p) { kerr << p; return *this; }
};

class Null_Debug
{
public:
    template<typename T>
    Null_Debug & operator<<(const T & o) { return *this; }

    template<typename T>
    Null_Debug & operator<<(const T * o) { return *this; }
};

template<bool debugged>
class Select_Debug: public Debug {};
template<>
class Select_Debug<false>: public Null_Debug {};

// Error
enum Debug_Error {ERR = 1};

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>
db(Debug_Error l)
{
    extern OStream::Err error;

    Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>() << begl;
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>() << error;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::error)>();
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>
db(Debug_Error l)
{
    extern OStream::Err error;

    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>() << begl;
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>() << error;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::error)>();
}

// Warning
enum Debug_Warning {WRN = 2};

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>
db(Debug_Warning l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>() << begl;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::warning)>();
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>
db(Debug_Warning l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>() << begl;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::warning)>();
}

// Info
enum Debug_Info {INF = 3};

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>
db(Debug_Info l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>() << begl;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::info)>();
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>
db(Debug_Info l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>() << begl;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::info)>();
}

// Trace
enum Debug_Trace {TRC = 4};

template<typename T>
inline Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>
db(Debug_Trace l)
{
    Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>() << begl;
    return Select_Debug<(Traits<T>::debugged && Traits<Debug>::trace)>();
}

template<typename T1, typename T2>
inline Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>
db(Debug_Trace l)
{
    Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>() << begl;
    return Select_Debug<((Traits<T1>::debugged || Traits<T2>::debugged) && Traits<Debug>::trace)>();
}


class Assert
{
public:
    static void fail(const char * __assertion, const char * __file, unsigned int __line, const char * __function) {
        db<Build>(ERR) << "Assertion fail: " << __assertion << ", function=" << __function << ", file=" << __file << ", line=" << __line << endl;
    }
};

__END_UTIL

#endif

