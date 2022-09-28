// EPOS Realview-PBX (ARM Cortex-A9) SETUP

// If emulating on QEMU, uboot has a vector table in ROM at 0x00000000.
// It relocates EPOS image (.bin), which has this vector table at the
// first address, to 0x00010000 and jumps to it.

#include <architecture.h>
#include <machine.h>

extern "C" {
    void _start();

    // SETUP entry point is the Vector Table and resides in the .init section (not in .text), so it will be linked first and will be the first function after the ELF header in the image.
    void _entry() __attribute__ ((naked, nothrow, section(".init")));
    void _reset() __attribute__ ((naked)); // so it can be safely reached from the vector table
    void _setup(); // just to create a Setup object

    // LD eliminates this variable while performing garbage collection, that's why the used attribute.
    char __boot_time_system_info[sizeof(EPOS::S::System_Info)] __attribute__ ((used)) = "<System_Info placeholder>"; // actual System_Info will be added by mkbi!
}

__BEGIN_SYS

extern OStream kout, kerr;

class Setup
{
private:
    // Physical memory map
    static const unsigned int RAM_BASE          = Memory_Map::RAM_BASE;
    static const unsigned int RAM_TOP           = Memory_Map::RAM_TOP;
    static const unsigned int MIO_BASE          = Memory_Map::MIO_BASE;
    static const unsigned int MIO_TOP           = Memory_Map::MIO_TOP;
    static const unsigned int VECTOR_TABLE      = Memory_Map::VECTOR_TABLE;
    static const unsigned int FLAT_PAGE_TABLE   = Memory_Map::FLAT_PAGE_TABLE;
    static const unsigned int BOOT_STACK        = Memory_Map::BOOT_STACK;
    static const unsigned int APP_LOW           = Memory_Map::APP_LOW;
    static const unsigned int APP_HIGH          = Memory_Map::APP_HIGH;
    static const unsigned int SYS_FLAGS         = Memory_Map::SYS_FLAGS;

    // Architecture Imports
    typedef CPU::Reg Reg;
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::Log_Addr Log_Addr;

public:
    Setup();

private:
    void say_hi();
    void setup_flat_paging();
    void enable_paging();
    void call_next();
    void panic() { Machine::panic(); }

    static SCU * scu() { return reinterpret_cast<SCU *>(Memory_Map::SCU_BASE); }
    static GIC_CPU * gic_cpu() { return reinterpret_cast<GIC_CPU *>(Memory_Map::GIC_CPU_BASE); }
    static GIC_Distributor * gic_distributor() { return reinterpret_cast<GIC_Distributor *>(Memory_Map::GIC_DIST_BASE); }

private:
    System_Info * si;

    static volatile bool paging_ready;
};

volatile bool Setup::paging_ready = false;

Setup::Setup()
{
    CPU::int_disable(); // interrupts will be re-enabled at init_end
    Display::init();

    si = reinterpret_cast<System_Info *>(&__boot_time_system_info);

    db<Setup>(TRC) << "Setup()" << endl;
    db<Setup>(INF) << "Setup:sp=" << CPU::sp() << endl;
    db<Setup>(INF) << "Setup:si=" << *si << endl;

    if(si->bm.n_cpus > Traits<Machine>::CPUS)
        si->bm.n_cpus = Traits<Machine>::CPUS;

    if(CPU::id() == 0) { // bootstrap CPU (BSP)
        // Print a SETUP summary
        say_hi();

        // Configure a flat memory model for the single task in the system
        setup_flat_paging();

        // Enable paging
        enable_paging();

        // Signalizes other CPUs that paging is up
        paging_ready = true;

        // Enable the Snoop Control Unit for SMP
        if(Traits<System>::multicore)
            scu()->enable();
    } else { // additional CPUs (APs)
        // Wait for the Boot CPU to setup page tables
        while(!paging_ready);
        enable_paging();
    }

    // Configure the Snoop Control Unit for SMP
    if(Traits<System>::multicore) {
        scu()->secure_invalidate();
        CPU::actlr(CPU::actlr() | CPU::SMP); // enable SMP
        CPU::actlr(CPU::actlr() | CPU::FW);  // enable the broadcasting of cache & TLB maintenance operations to other SMP cores
        scu()->enable_cache_coherence();
    }

    // Configure GIC for SMP, set SYS_FLAGS to the address where secondary CPUS will start executing and then wake them up
    // QEMU Bug: we already moved the vector table from Traits<Machine>::RESET to Memory_Map::VECTOR_TABLE and Traits<Machine>::RESET
    //   (and SETUP as a whole will soon be part of the free memory for multitasking configurations), but QEMU puts non-BSP cores on halt in a loop
    //   with "tst r1, r1", so the wake up address cannot be 0. A barrier at Machine::pre_init() ensures Traits<Machine>::RESET won't be overwritten
    //   before all cores get there.
    if(Traits<System>::multicore && (CPU::id() == 0)) {
        gic_distributor()->init();
        gic_cpu()->init();
        *reinterpret_cast<unsigned long *>(SYS_FLAGS) = Traits<Machine>::RESET;
        gic_distributor()->smp_init(Traits<Machine>::CPUS);
    }

    // SETUP ends here, so let's transfer control to next stage (INIT or APP)
    call_next();
}

void Setup::setup_flat_paging()
{
    // Single level with a single 16KB page table containing 4096 entries, each designating a 1 MB region
    db<Setup>(TRC) << "Setup::setup_flat_paging()" << endl;

    CPU::Reg * pt = reinterpret_cast<CPU::Reg *>(FLAT_PAGE_TABLE);
    for(CPU::Reg i = MMU_Common<12,0,20>::directory(RAM_BASE); i < MMU_Common<12,0,20>::directory(RAM_BASE) + MMU_Common<12,0,20>::pages(RAM_TOP - RAM_BASE); i++)
        pt[i] = (i << 20) | ARMv7_MMU::Section_Flags::FLAT_MEMORY_MEM;
    for(CPU::Reg i = MMU_Common<12,0,20>::directory(MIO_BASE); i < MMU_Common<12,0,20>::directory(MIO_BASE) + MMU_Common<12,0,20>::pages(MIO_TOP - MIO_BASE) - 1; i++)
        pt[i] = (i << 20) | ARMv7_MMU::Section_Flags::FLAT_MEMORY_DEV;
}

void Setup::enable_paging()
{
    db<Setup>(TRC) << "Setup::enable_paging()" << endl;
    if(Traits<Setup>::hysterically_debugged) {
        db<Setup>(INF) << "pc=" << CPU::pc() << endl;
        db<Setup>(INF) << "sp=" << CPU::sp() << endl;
    }

    // MNG_DOMAIN for no page permission verification, CLI_DOMAIN otherwise
    CPU::dacr(CPU::CLI_DOMAIN);

    CPU::dsb();
    CPU::isb();

    // Clear TTBCR for the system to use ttbr0 instead of 1
    CPU::ttbcr(0);
    // Set ttbr0 with base address
    CPU::ttbr0(FLAT_PAGE_TABLE);

    // Enable MMU through SCTLR and ACTLR
    CPU::actlr(CPU::actlr() | CPU::SMP); // Set SMP bit
    CPU::sctlr((CPU::sctlr() | CPU::DCACHE | CPU::ICACHE | CPU::MMU_ENABLE) & ~(CPU::AFE));

    CPU::dsb();
    CPU::isb();

    // MMU now enabled - Virtual address system now active
    // Branch Prediction Enable
    CPU::sctlr(CPU::sctlr() | CPU::BRANCH_PRED);

    // Flush TLB to ensure we've got the right memory organization
    MMU::flush_tlb();

    // Adjust pointers that will still be used to their logical addresses
    Display::init(); // adjust the pointers in Display by calling init

    if(Traits<Setup>::hysterically_debugged) {
        db<Setup>(INF) << "pc=" << CPU::pc() << endl;
        db<Setup>(INF) << "sp=" << CPU::sp() << endl;
    }
}

void Setup::say_hi()
{
    db<Setup>(TRC) << "Setup::say_hi()" << endl;
    db<Setup>(INF) << "System_Info=" << *si << endl;

    kout << "This is EPOS!\n" << endl;
    kout << "Setting up this machine as follows: " << endl;
    kout << "  Mode:         " << ((Traits<Build>::MODE == Traits<Build>::LIBRARY) ? "library" : (Traits<Build>::MODE == Traits<Build>::BUILTIN) ? "built-in" : "kernel") << endl;
    kout << "  Processor:    " << Traits<Machine>::CPUS << " x Cortex A9 at " << Traits<CPU>::CLOCK / 1000000 << " MHz (BUS clock = " << Traits<CPU>::CLOCK / 1000000 << " MHz)" << endl;
    kout << "  Machine:      Realview PBX" << endl;
    kout << "  Memory:       " << (si->bm.mem_top - si->bm.mem_base) / 1024 << " KB [" << reinterpret_cast<void *>(si->bm.mem_base) << ":" << reinterpret_cast<void *>(si->bm.mem_top) << "]" << endl;
    kout << "  I/O space:    " << (si->bm.mio_top - si->bm.mio_base) / 1024 << " KB [" << reinterpret_cast<void *>(si->bm.mio_base) << ":" << reinterpret_cast<void *>(si->bm.mio_top) << "]" << endl;
    kout << "  Node Id:      ";
    if(si->bm.node_id != -1)
        kout << si->bm.node_id << " (" << Traits<Build>::NODES << ")" << endl;
    else
        kout << "will get from the network!" << endl;
    kout << "  Position:     ";
    if(si->bm.space_x != -1)
        kout << "(" << si->bm.space_x << "," << si->bm.space_y << "," << si->bm.space_z << ")" << endl;
    else
        kout << "will get from the network!" << endl;
    if(si->lm.app_extra_size)
        kout << "  Extras:       " << si->lm.app_extra_size << " bytes" << endl;

    kout << endl;
}

void Setup::call_next()
{
    db<Setup>(TRC) << "Setup::call_next()" << endl;
    db<Setup>(INF) << "SETUP ends here!" << endl;

    // Check for next stage and obtain the entry point
    // In library mode, we'll continue using the BOOT stacks until we reach the first application
    _start();

    // SETUP is now part of the free memory and this point should never be
    // reached, but, just in case ... :-)
    panic();
}

__END_SYS

using namespace EPOS::S;

void _entry()
{
    // Interrupt Vector Table
    // We use and indirection table for the ldr instructions because the offset can be to far from the PC to be encoded
    ASM("               ldr pc, .reset                                          \t\n\
                        ldr pc, .ui                                             \t\n\
                        ldr pc, .si                                             \t\n\
                        ldr pc, .pa                                             \t\n\
                        ldr pc, .da                                             \t\n\
                        ldr pc, .reserved                                       \t\n\
                        ldr pc, .irq                                            \t\n\
                        ldr pc, .fiq                                            \t\n\
                                                                                \t\n\
                        .balign 32                                              \t\n\
        .reset:         .word _reset                                            \t\n\
        .ui:            .word 0x0                                               \t\n\
        .si:            .word 0x0                                               \t\n\
        .pa:            .word 0x0                                               \t\n\
        .da:            .word 0x0                                               \t\n\
        .reserved:      .word 0x0                                               \t\n\
        .irq:           .word 0x0                                               \t\n\
        .fiq:           .word 0x0                                               \t");
}

void _reset()
{
    // Configure a stack for SVC mode, which will be used until the first Thread is created
    CPU::mode(CPU::MODE_SVC); // enter SVC mode (with interrupts disabled)
    CPU::sp(Memory_Map::BOOT_STACK + Traits<Machine>::STACK_SIZE * (CPU::id() + 1) - sizeof(long));
    CPU::int_disable(); // interrupts will be re-enabled at init_end

    if(CPU::id() == 0) {
        // After a reset, we copy the vector table to 0x0000 to get a cleaner memory map (it is originally at 0x8000)
        // An alternative would be to set vbar address via mrc p15, 0, r1, c12, c0, 0
        CPU::r0(reinterpret_cast<CPU::Reg>(&_entry)); // load r0 with the source pointer
        CPU::r1(Memory_Map::VECTOR_TABLE); // load r1 with the destination pointer

        // Copy the first 32 bytes
        CPU::ldmia(); // load multiple registers from the memory pointed by r0 and auto-increment it accordingly
        CPU::stmia(); // store multiple registers to the memory pointed by r1 and auto-increment it accordingly

        // Repeat to copy the subsequent 32 bytes
        CPU::ldmia();
        CPU::stmia();

        // Clear the BSS (SETUP was linked to CRT0, but entry point didn't go through BSS clear)
        Machine::clear_bss();
    }

    // Set VBAR to point to the relocated the vector table
    CPU::vbar(Memory_Map::VECTOR_TABLE);

    CPU::flush_caches();
    CPU::flush_branch_predictors();
    CPU::flush_tlb();
    CPU::actlr(CPU::actlr() | CPU::DCACHE_PREFETCH); // enable D-side prefetch

    CPU::dsb();
    CPU::isb();

    _setup();
}

void _setup()
{
    Setup setup;
}
