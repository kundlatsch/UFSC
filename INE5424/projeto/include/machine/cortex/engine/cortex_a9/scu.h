// EPOS Cortex-A9 Snoop Control Unit Mediator Declarations

#ifndef __cortex_a9_scu_h
#define __cortex_a9_scu_h

#include <architecture/cpu.h>

__BEGIN_SYS

class SCU
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::PRIVATE_TIMER_BASE) A9_Private_Timer".

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers' offsets from BASE
    enum {                                      // Description              Type    Value after reset
        SCU_CTRL                    = 0x00,     // Control                  r/w     0x00000128
        SCU_CONFIG                  = 0x04,     // Configuration            r/w     0x00000000
        SCU_PSTAT                   = 0x08,     // Power Status             r/w     0x00000000
        SCU_IARSS                   = 0x0c      // Invalidate All Reg Secure State
    };

public:
    void enable() {
        scu(0) |= 0x1;
    }

    void secure_invalidate() {
        // This function invalidates the SCU copy of the TAG RAMs for the core.  Typically only done at start-up.
        // Possible flow:
        //- Invalidate L1 caches
        //- Invalidate SCU copy of TAG RAMs
        //- Join SMP
        Reg32 cpu = CPU::id() << 2;     // convert into bit offset (four bits per core)
        Reg32 ways = 0x0f << cpu;       // all four ways (shifted into the correct CPU field)
        scu(SCU_IARSS) = ways;
    }

    void enable_cache_coherence() {
        scu(SCU_CONFIG) |= 0xf0;
    }

protected:
    volatile Reg32 & scu(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
