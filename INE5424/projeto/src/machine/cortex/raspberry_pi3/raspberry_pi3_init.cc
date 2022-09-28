// EPOS Raspberry Pi3 (ARM Cortex-A53) Initialization

#include <system/config.h>
#include <machine.h>
#include <architecture/cpu.h>

__BEGIN_SYS

void Raspberry_Pi3::pre_init()
{
    BCM_Mailbox * mbox = reinterpret_cast<BCM_Mailbox *>(Memory_Map::MBOX_CTRL_BASE);

    // SMP initialization
    if(CPU::id() == 0) {
        // This is only a flat segment register that allows mapping the start point for the secondary cores
        // For the Send Event to take place, the start point is required to be non 0
        // Writing 0 to this register will trigger no effects at the target CPUs
        for(unsigned int i = 1; i < Traits<Build>::CPUS; i++) {
            mbox->start(i, Traits<Machine>::RESET);
            if(Traits<Machine>::hysterically_debugged)
                delay(1000000);
        }
    }
}

__END_SYS
