// EPOS ARMv7 MMU Mediator Initialization

#include <architecture/mmu.h>
#include <system.h>

#ifdef __cortex_a53__

__BEGIN_SYS

void ARMv7_MMU::init()
{
    db<Init, MMU>(TRC) << "MMU::init()" << endl;
    db<Init, MMU>(INF) << "MMU::init()" << System::info() << hex << ", base=" << System::info()->pmm.free1_base << ", top=" << System::info()->pmm.free1_top << endl;
    free(System::info()->pmm.free1_base, pages(System::info()->pmm.free1_top - System::info()->pmm.free1_base));
   
    // Remember the master page directory (created during SETUP)
    _master = current();
    db<Init, MMU>(INF) << "MMU::master page directory=" << _master << endl;
}

__END_SYS

#endif
