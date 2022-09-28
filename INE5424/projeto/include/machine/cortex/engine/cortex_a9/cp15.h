// EPOS Cortex-A9 Control Coprocessor 15 Mediator Declarations

#ifndef __cortex_a9_cp15_h
#define __cortex_a9_cp15_h

#include <architecture/cpu.h>

__BEGIN_SYS

class CP15
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::PRIVATE_TIMER_BASE) A9_Private_Timer".

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers' offsets from BASE

public:
};

__END_SYS

#endif
