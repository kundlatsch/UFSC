// EPOS Conversion Utility Declarations

#ifndef __convert_h
#define __convert_h

#include <utility/math.h>

__BEGIN_UTIL

namespace Convert
{
template<typename T>
inline T deg2rad(T deg) { return deg * Math::pi() / 180.0; }

template<typename T>
inline T g2ms2(T g) { return g * 9.80665; }

template<typename T>
inline T c2k(T c) { return c - 273.15; }

template<typename Count, typename Time, typename Temporary = typename LARGER<Time>::Result>
inline Count s2count(const Count & frequency, const Time & time) { return static_cast<Temporary>(time) * frequency / 1; }
template<typename Count, typename Time, typename Temporary = typename LARGER<Time>::Result>
inline Count ms2count(const Count & frequency, const Time & time) { return static_cast<Temporary>(time) * frequency / 1000; }
template<typename Count, typename Time, typename Temporary = typename LARGER<Time>::Result>
inline Count us2count(const Count & frequency, const Time & time) { return static_cast<Temporary>(time) * frequency / 1000000; }
template<typename Count, typename Temporary = typename LARGER<Count>::Result>
inline Count percent2count(const Percent & duty_cycle, const Count & period) { return period - (static_cast<Temporary>(period) * int(duty_cycle) / 100); }

template<typename Hertz, typename Count, typename Time, typename Temporary = typename LARGER<Time>::Result>
inline Time count2s(const Hertz & frequency, const Count & count) { return static_cast<Temporary>(count) / frequency; }
template<typename Hertz, typename Count, typename Time, typename Temporary = typename LARGER<Time>::Result>
inline Time count2ms(const Hertz & frequency, const Count & count) { return (static_cast<Temporary>(count) / (frequency / 1000 )) ; }
template<typename Hertz, typename Count, typename Time, typename Temporary = typename LARGER<Time>::Result>
inline Time count2us(const Hertz & frequency, const Count & count) { return (static_cast<Temporary>(count) / (frequency / 1000000)); }
};

__END_UTIL

#endif
