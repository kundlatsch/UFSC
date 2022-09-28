// EPOS RISC-V 32 CPU Mediator Initialization

#include <architecture.h>

extern "C" { void __epos_library_app_entry(void); }

__BEGIN_SYS

void CPU::init()
{
    db<Init, CPU>(TRC) << "CPU::init()" << endl;

    if(CPU::id() == 0) {
        if(Traits<MMU>::enabled)
            MMU::init();
        else
            db<Init, MMU>(WRN) << "MMU is disabled!" << endl;
    }

#ifdef __TSC_H
    if(Traits<TSC>::enabled)
        TSC::init();
#endif
}

__END_SYS
