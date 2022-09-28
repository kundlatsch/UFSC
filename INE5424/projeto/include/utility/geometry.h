// EPOS Geometry Utility Declarations

#ifndef __geometry_h
#define __geometry_h

#include <utility/math.h>

__BEGIN_UTIL

template<typename T, unsigned int dimensions>
struct Point;

template<typename T>
struct Point<T, 2>
{
private:
    typedef typename IF<EQUAL<char, T>::Result, int, T>::Result Print_Type;
    typedef typename LARGER<T>::Result Larger_T;

public:
    using Number = T;
    using Distance = typename UNSIGNED<Larger_T>::Result;

    Point() {}
    Point(const T & xi, const T & yi): x(xi), y(yi) {}

    // Euclidean distance
    template<typename P>
    Distance operator-(const P & p) const {
        // Care for unsigned T
        Larger_T xx = p.x > x ? p.x - x : x - p.x;
        Larger_T yy = p.y > y ? p.y - y : y - p.y;
        return sqrt(xx*xx + yy*yy);
    }

    // Translation
    template<typename P>
    Point & operator-=(const P & p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }
    template<typename P>
    Point & operator+=(const P & p) {
        x += p.x;
        y += p.y;
        return *this;
    }
    template<typename P>
    Point operator+(const P & p) const {
        Point ret(x, y);
        return ret += p;
    }

    static Point trilaterate(const Point & p1, const Distance & d1,  const Point & p2, const Distance & d2, const Point & p3, const Distance & d3) {
        Larger_T a[2][3];

        a[0][0] = 2 * (-p1.x + p3.x);
        a[0][1] = 2 * (-p1.y + p3.y);
        a[0][2] = 1 * ((d1 * d1 - (p1.x * p1.x + p1.y * p1.y)) - (d3 * d3 - (p3.x * p3.x + p3.y * p3.y)));

        a[1][0] = 2 * (-p2.x + p3.x);
        a[1][1] = 2 * (-p2.y + p3.y);
        a[1][2] = 1 * ((d2 * d2 - (p2.x * p2.x + p2.y * p2.y))-(d3 * d3 - (p3.x * p3.x + p3.y * p3.y)));

        if (a[0][0] != 0) {
            a[1][1] -= a[0][1] * a[1][0] / a[0][0];
            a[1][2] -= a[0][2] * a[1][0] / a[0][0];
            a[1][0] = 0;
        }

        T x, y;

        if(a[1][1] == 0)
            y = 0;
        else
            y = a[1][2] / a[1][1];

        if(a[0][0] == 0)
            x = 0;
        else
            x = (a[0][2] - (y * a[0][1])) / a[0][0];

        return Point(x, y);
    }

    bool operator==(const Point & p) const { return (x == p.x) && (y == p.y); }
    bool operator!=(const Point & p) const { return !(*this == p); }

    friend OStream & operator<<(OStream & os, const Point<T, 2> & c) {
        os << "{" << static_cast<Print_Type>(c.x) << "," << static_cast<Print_Type>(c.y) << "}";
        return os;
    }

    T x, y;
}__attribute__((packed));

template<typename T>
struct Point<T, 3>
{
private:
    typedef typename IF<EQUAL<char, T>::Result, int, T>::Result Print_Type;
    typedef typename LARGER<T>::Result Larger_T;

public:
    using Number = T;
    using Distance = typename UNSIGNED<Larger_T>::Result;

    Point() {}
    Point(const T & xi, const T & yi, const T & zi): x(xi), y(yi), z(zi) {}

    // Euclidean distance
    template<typename P>
    Distance operator-(const P & p) const {
        // Care for unsigned T
        Distance xx = p.x > x ? p.x - x : x - p.x;
        Distance yy = p.y > y ? p.y - y : y - p.y;
        Distance zz = p.z > z ? p.z - z : z - p.z;
        return Math::sqrt(xx * xx + yy * yy + zz * zz);
    }

    // Translation
    template<typename P>
    Point & operator-=(const P & p) {
        x -= p.x;
        y -= p.y;
        z -= p.z;
        return *this;
    }
    template<typename P>
    Point & operator+=(const P & p) {
        x += p.x;
        y += p.y;
        z += p.z;
        return *this;
    }
    template<typename P>
    Point operator+(const P & p) const {
        Point ret(x, y, z);
        return ret += p;
    }

    // TODO: 3D trilateration
    static Point trilaterate(const Point & p1, const Distance & d1,  const Point & p2, const Distance & d2, const Point & p3, const Distance & d3) {
        Larger_T a[2][3];

        a[0][0] = 2 * (-p1.x + p3.x);
        a[0][1] = 2 * (-p1.y + p3.y);
        a[0][2] = 1 * ((d1 * d1 - (p1.x * p1.x + p1.y * p1.y)) - (d3 * d3 - (p3.x * p3.x + p3.y * p3.y)));

        a[1][0] = 2 * (-p2.x + p3.x);
        a[1][1] = 2 * (-p2.y + p3.y);
        a[1][2] = 1 * ((d2 * d2 - (p2.x * p2.x + p2.y * p2.y))-(d3 * d3 - (p3.x * p3.x + p3.y * p3.y)));

        if (a[0][0] != 0) {
            a[1][1] -= a[0][1] * a[1][0] / a[0][0];
            a[1][2] -= a[0][2] * a[1][0] / a[0][0];
            a[1][0] = 0;
        }

        T x, y;

        if(a[1][1] == 0)
            y = 0;
        else
            y = a[1][2] / a[1][1];

        if(a[0][0] == 0)
            x = 0;
        else
            x = (a[0][2] - (y * a[0][1])) / a[0][0];

        return Point(x, y, 0);
    }

    bool operator==(const Point & p) const { return (x == p.x) && (y == p.y) && (z == p.z); }
    bool operator!=(const Point & p) const { return !(*this == p); }

    friend OStream & operator<<(OStream & os, const Point & c) {
        os << "(" << static_cast<Print_Type>(c.x) << "," << static_cast<Print_Type>(c.y) << "," << static_cast<Print_Type>(c.z) << ")";
        return os;
    }

    T x, y, z;
}__attribute__((packed));

template<typename T1, typename T2 = void>
struct Sphere
{
private:
    typedef typename IF<EQUAL<T1, char>::Result, int, T1>::Result Print_Type;

public:
    using Number = T1;
    using Center = Point<Number, 3>;
    using Radius = typename IF<EQUAL<T2, void>::Result, typename Center::Distance, T2>::Result;

    Sphere() {}
    Sphere(const Center & c, const Radius & r = 0): center(c), radius(r) {}

    bool contains(const Center & c) const { return (center - c) <= radius; }

    friend OStream & operator<<(OStream & os, const Sphere & s) {
        os << "{" << "c=" << s.center << ",r=" << static_cast<Print_Type>(s.radius) << "}";
        return os;
    }

    Center center;
    Radius radius;
}__attribute__((packed));

__END_UTIL

#endif
