// EPOS CPU Mediator Common Package

#ifndef __cpu_h
#define __cpu_h

#include <system/config.h>

__BEGIN_SYS

class CPU_Common
{
protected:
    static const bool BIG_ENDIAN = (Traits<CPU>::ENDIANESS == Traits<CPU>::BIG);

protected:
    CPU_Common() {}

public:
    typedef unsigned char  Reg8;
    typedef unsigned short Reg16;
    typedef IF<Traits<CPU>::WORD_SIZE == 32, unsigned long /* IPL32 */, unsigned int /* LP64 */>::Result Reg32;
    typedef IF<Traits<CPU>::WORD_SIZE == 32, unsigned long long /* IPL32 */, unsigned long /* LP64 */>::Result Reg64;
    typedef SWITCH<Traits<CPU>::WORD_SIZE, CASE<16, Reg16, CASE<32, Reg32, CASE<64, Reg64>>>>::Result Reg;

    template <typename Reg>
    class Address
    {
    public:
        Address() {}
        Address(const Address & a) : _addr(a._addr) {}
        Address(const Reg & a) : _addr(a) {}
        template<typename T>
        Address(T * a) : _addr(Reg(a)) {}

        operator const Reg &() const { return _addr; }

        template<typename T>
        operator T *() const { return reinterpret_cast<T *>(_addr); }

        template<typename T>
        bool operator==(T a) const { return (_addr == Reg(a)); }
        template<typename T>
        bool operator< (T a) const { return (_addr < Reg(a)); }
        template<typename T>
        bool operator> (T a) const { return (_addr > Reg(a)); }
        template<typename T>
        bool operator>=(T a) const { return (_addr >= Reg(a)); }
        template<typename T>
        bool operator<=(T a) const { return (_addr <= Reg(a)); }

        template<typename T>
        Address operator-(T a) const { return _addr - Reg(a); }
        template<typename T>
        Address operator+(T a) const { return _addr + Reg(a); }
        template<typename T>
        Address & operator+=(T a) { _addr += Reg(a); return *this; }
        template<typename T>
        Address & operator-=(T a) { _addr -= Reg(a); return *this; }
        template<typename T>
        Address & operator&=(T a) { _addr &= Reg(a); return *this; }
        template<typename T>
        Address & operator|=(T a) { _addr |= Reg(a); return *this; }

        Address & operator[](int i) { return *(this + i); }

        friend OStream & operator<<(OStream & os, const Address & a) { os << reinterpret_cast<void *>(a._addr); return os; }

    private:
        Reg _addr;
    };

    typedef Address<Reg> Log_Addr;
    typedef Address<Reg> Phy_Addr;

    class Context;

public:
    static Log_Addr pc();       // program counter / instruction pointer

    static Log_Addr sp();       // ABI stack pointer
    static void sp(Log_Addr sp);

    static Log_Addr fp();       // ABI frame pointer
    static void fp(Log_Addr sp);

    static Log_Addr ra();       // ABI return address (either link register or from the stack)

    static Reg fr();            // ABI function return (either a register or from the stack)
    static void fr(Reg fr);

    static Hertz clock()  { return Traits<CPU>::CLOCK; }
    static void clock(const Hertz & frequency) {}
    static Hertz max_clock() { return Traits<CPU>::CLOCK; }
    static Hertz min_clock() { return Traits<CPU>::CLOCK; }

    static Hertz bus_clock() { return Traits<CPU>::CLOCK; }

    static void int_enable();
    static void int_disable();
    static bool int_enabled();
    static bool int_disabled();

    static void halt() { for(;;); }

    static void switch_context(Context * volatile * o, Context * volatile n);

    static void syscall(void * message);

    static unsigned int id();
    static unsigned int cores();

    template <typename T>
    static T tsl(volatile T & lock) {
        T old = lock;
        lock = 1;
        return old;
    }

    template <typename T>
    static T finc(volatile T & value) {
        T old = value;
        value++;
        return old;
    }

    template <typename T>
    static T fdec(volatile T & value) {
        T old = value;
        value--;
        return old;
    }

    template <typename T>
    static T cas(volatile T & value, T compare, T replacement) {
        T old = value;
        if(value == compare) {
            value = replacement;
        }
        return old;
    }

    template <int (* finc)(volatile int &)>
    static void smp_barrier(unsigned int cores, unsigned int id) {
        if(cores > 1) {
            static volatile int ready[2];
            static volatile int i;

            int j = i;

            finc(ready[j]);
            if(id == 0) {
                while(ready[j] < int(cores));       // wait for all CPUs to be ready
                i = !i;                             // toggle ready
                ready[j] = 0;                       // signalizes waiting CPUs
            } else {
                while(ready[j]);                    // wait for CPU[0] signal
            }
        }
    }

    static void fpu_save();
    static void fpu_restore();

    static void flush_tlb();
    static void flush_tlb(Log_Addr addr);

    static Reg64 htole64(Reg64 v) { return (BIG_ENDIAN) ? swap64(v) : v; }
    static Reg32 htole32(Reg32 v) { return (BIG_ENDIAN) ? swap32(v) : v; }
    static Reg16 htole16(Reg16 v) { return (BIG_ENDIAN) ? swap16(v) : v; }
    static Reg64 letoh64(Reg64 v) { return htole64(v); }
    static Reg32 letoh32(Reg32 v) { return htole32(v); }
    static Reg16 letoh16(Reg16 v) { return htole16(v); }

    static Reg64 htobe64(Reg64 v) { return (!BIG_ENDIAN) ? swap64(v) : v; }
    static Reg32 htobe32(Reg32 v) { return (!BIG_ENDIAN) ? swap32(v) : v; }
    static Reg16 htobe16(Reg16 v) { return (!BIG_ENDIAN) ? swap16(v) : v; }
    static Reg64 betoh64(Reg64 v) { return htobe64(v); }
    static Reg32 betoh32(Reg32 v) { return htobe32(v); }
    static Reg16 betoh16(Reg16 v) { return htobe16(v); }

    static Reg32 htonl(Reg32 v) { return (BIG_ENDIAN) ? v : swap32(v); }
    static Reg16 htons(Reg16 v) { return (BIG_ENDIAN) ? v : swap16(v); }
    static Reg32 ntohl(Reg32 v) { return htonl(v); }
    static Reg16 ntohs(Reg16 v) { return htons(v); }

protected:
    static Reg64 swap64(Reg64 v) { return
        ((v & 0xff00000000000000ULL) >> 56) |
        ((v & 0x00ff000000000000ULL) >> 40) |
        ((v & 0x0000ff0000000000ULL) >> 24) |
        ((v & 0x000000ff00000000uLL) >> 8)  |
        ((v & 0x00000000ff000000ULL) << 8)  |
        ((v & 0x0000000000ff0000ULL) << 24) |
        ((v & 0x000000000000ff00ULL) << 40) |
        ((v & 0x00000000000000ffULL) << 56); }
    static Reg32 swap32(Reg32 v) { return
        ((v & 0xff000000) >> 24) |
        ((v & 0x00ff0000) >> 8) |
        ((v & 0x0000ff00) << 8) |
        ((v & 0x000000ff) << 24); }
    static Reg16 swap16(Reg16 v) { return
        ((v & 0xff00) >> 8) |
        ((v & 0x00ff) << 8); }
};

template<typename T>
inline T align32(const T & addr) { return (addr + 3) & ~3U; }
template<typename T>
inline T align64(const T & addr) { return (addr + 7) & ~7U; }
template<typename T>
inline T align128(const T & addr) { return (addr + 15) & ~15U; }

__END_SYS

#endif

#if defined(__CPU_H) && !defined(__cpu_common_only__)
#include __CPU_H
#endif
