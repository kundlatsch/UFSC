// EPOS Cortex-A Interrupt Controller Initialization

#include <architecture/cpu.h>
#include <machine/ic.h>
#include <system/memory_map.h>

#ifdef __cortex_a__

__BEGIN_SYS

extern "C" {
    void _undefined_instruction();
    void _software_interrupt();
    void _prefetch_abort();
    void _data_abort();
    void _reserved();
    void _int_entry();
    void _fiq();
}

void IC::init()
{
    db<Init, IC>(TRC) << "IC::init()" << endl;

    CPU::int_disable(); // will be reenabled at Thread::init() by Context::load()
    Engine::init();

    disable(); // will be enabled on demand as handlers are registered

#ifdef __arch_armv7__
    CPU::FSR ** vt = reinterpret_cast<CPU::FSR **>(Memory_Map::VECTOR_TABLE + 32); // add 32 bytes to skip the "ldr" instructions and get to the function pointers
    vt[0] = _reserved; // _reset is only defined for SETUP
    vt[1] = _undefined_instruction;
    vt[2] = _software_interrupt;
    vt[3] = _prefetch_abort;
    vt[4] = _data_abort;
    vt[5] = _reserved;
    vt[6] = _int_entry;
    vt[7] = _fiq;
#else
    // Vector table has 4 entries for each of the 4 cases, all aligned 128 bytes.
    // 4x4x128 = 2048. The brench address is right after the vector table.
    CPU::FSR ** vt = reinterpret_cast<CPU::FSR **>(Memory_Map::VECTOR_TABLE + 2048);
    vt[0] = _int_entry;
#endif

    // Set all interrupt handlers to int_not()
    for(Interrupt_Id i = 0; i < INTS; i++)
        _int_vector[i] = int_not;
}

__END_SYS

#endif
