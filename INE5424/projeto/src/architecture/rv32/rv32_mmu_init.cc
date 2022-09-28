// EPOS RISC-V 32 MMU Mediator Initialization

#include <architecture/mmu.h>
#include <system.h>

__BEGIN_SYS

void RV32S_MMU::init()
{
    db<Init, MMU>(TRC) << "MMU::init()" << endl;

    free(System::info()->pmm.free1_base, pages(System::info()->pmm.free1_top - System::info()->pmm.free1_base));

    // Remember the master page directory (created during SETUP)
    _master = current();
    db<Init, MMU>(INF) << "MMU::master page directory=" << _master << endl;
}

__END_SYS

