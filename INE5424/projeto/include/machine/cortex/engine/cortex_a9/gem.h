// EPOS Zynq Gigabit Ethernet Controller NIC Mediator Declarations

#ifndef __gem_h
#define __gem_h

#include <architecture/cpu.h>

__BEGIN_SYS

class GEM
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::UARTx_BASE) PL011".

private:
    typedef CPU::Reg32 Reg32;

public:

private:
    volatile Reg32 & gem(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
