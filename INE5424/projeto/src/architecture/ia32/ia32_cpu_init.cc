// EPOS IA32 CPU Mediator Initialization

#include <architecture.h>
#include <machine/machine.h>
#include <system.h>

__BEGIN_SYS

void CPU::init()
{
    db<Init, CPU>(TRC) << "CPU::init()" << endl;

    _cpu_clock = System::info()->tm.cpu_clock;
    _cpu_current_clock = System::info()->tm.cpu_clock;
    _bus_clock = System::info()->tm.bus_clock;

    // Initialize the MMU
    if(CPU::id() == 0) {
        if(Traits<MMU>::enabled)
            MMU::init();
        else
            db<Init, MMU>(WRN) << "MMU is disabled!" << endl;
    }

    // Initialize the PMU	
    if(Traits<PMU>::enabled)
        PMU::init();

    // Initialize the CPU's Fast System Call mechanism
    // by setting up the corresponding MSRs
//    if(Traits<System>::mode == Traits<Build>::KERNEL) {
//        // IA32_SYSENTER_CS (MSR address 174H)
//        wrmsr(0x174, SEL_SYS_CODE);
//        // IA32_SYSENTER_ESP (MSR address 175H)
//        wrmsr(0x175, Memory_Map::SYS_STACK + Traits<System>::STACK_SIZE * (CPU::id() + 1) - 2 * sizeof(int));
//        // IA32_SYSENTER_EIP (MSR address 176H)
//        wrmsr(0x176, reinterpret_cast<Reg64>(&syscalled));
//        db<IA32>(INF) << "IA32::init() => MSR="
//            << "{MSR[CS]=" << hex << rdmsr(0x174)
//            << ",MSR[EIP]=" << hex << rdmsr(0x175)
//            << ",MSR[ESP]=" << hex << rdmsr(0x176)
//            << "}\n";
//    }
}

void CPU::smp_barrier_init(unsigned int cores) {
    // Core activation in IA32 is handled by the APIC
    _cores = cores;
    if(smp)
        APIC::remap();
};

__END_SYS
