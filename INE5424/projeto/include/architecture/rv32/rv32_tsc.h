// EPOS RISC-V 32 Time-Stamp Counter Mediator Declarations

#ifndef __rv32_tsc_h
#define __rv32_tsc_h

#include <architecture/cpu.h>
#include <architecture/tsc.h>
#include <system/memory_map.h>

__BEGIN_SYS

class TSC: private TSC_Common
{
    friend class CPU;
    friend class IC;

private:
    static const unsigned int CLOCK = Traits<Timer>::CLOCK;
    static const unsigned int ACCURACY = 40000; // this is actually unknown at the moment

    // Registers offsets from CLINT_BASE
    enum {               // Description
        MTIME  = 0xbff8, // Counter (lower 32 bits)
        MTIMEH = 0xbffc  // Counter (upper 32 bits)
    };

public:
    using TSC_Common::Time_Stamp;

    static const unsigned int FREQUENCY = CLOCK;

public:
    TSC() {}

    static Hertz frequency() { return CLOCK; }
    static PPB accuracy() { return ACCURACY; }

    static Time_Stamp time_stamp() { return (CPU::Reg64(reg(MTIMEH)) << 32) | reg(MTIME); }

private:
    static void init() {}

    static volatile CPU::Reg32 & reg(unsigned int o) { return reinterpret_cast<volatile CPU::Reg32 *>(Memory_Map::CLINT_BASE)[o / sizeof(CPU::Reg32)]; }
};

__END_SYS

#endif
