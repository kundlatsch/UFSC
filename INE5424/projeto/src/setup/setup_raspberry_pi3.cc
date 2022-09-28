// EPOS Raspberry Pi3 (Cortex-A53) SETUP

#include <architecture.h>
#include <machine.h>
#include <utility/elf.h>
#include <utility/string.h>

extern "C" {
    void _start();

    // SETUP entry point is the Vector Table and resides in the .init section (not in .text), so it will be linked first and will be the first function after the ELF header in the image.
    void _entry() __attribute__ ((used, naked, section(".init")));
    void _vector_table() __attribute__ ((naked));
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
    static const unsigned long RAM_BASE         = Memory_Map::RAM_BASE;
    static const unsigned long RAM_TOP          = Memory_Map::RAM_TOP;
    static const unsigned long MIO_BASE         = Memory_Map::MIO_BASE;
    static const unsigned long MIO_TOP          = Memory_Map::MIO_TOP;
    static const unsigned long IMAGE            = Memory_Map::IMAGE;
    static const unsigned long SETUP            = Memory_Map::SETUP;
    static const unsigned long FLAT_PAGE_TABLE  = Memory_Map::FLAT_PAGE_TABLE;

    // Logical memory map
    static const unsigned long APP_LOW          = Memory_Map::APP_LOW;
    static const unsigned long PHY_MEM          = Memory_Map::PHY_MEM;
    static const unsigned long IO               = Memory_Map::IO;
    static const unsigned long SYS              = Memory_Map::SYS;
    static const unsigned long SYS_INFO         = Memory_Map::SYS_INFO;
    static const unsigned long SYS_PT           = Memory_Map::SYS_PT;
    static const unsigned long SYS_PD           = Memory_Map::SYS_PD;
    static const unsigned long SYS_CODE         = Memory_Map::SYS_CODE;
    static const unsigned long SYS_DATA         = Memory_Map::SYS_DATA;
    static const unsigned long SYS_STACK        = Memory_Map::SYS_STACK;
    static const unsigned long SYS_HEAP         = Memory_Map::SYS_HEAP;
    static const unsigned long SYS_HIGH         = Memory_Map::SYS_HIGH;
    static const unsigned long APP_CODE         = Memory_Map::APP_CODE;
    static const unsigned long APP_DATA         = Memory_Map::APP_DATA;

    // Architecture Imports
    typedef CPU::Reg Reg;
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::Log_Addr Log_Addr;
    typedef MMU::Page Page;
    typedef MMU::Page_Flags Flags;
    typedef MMU::Page_Table Page_Table;
    typedef MMU::Page_Directory Page_Directory;
    typedef MMU::PT_Entry PT_Entry;
    typedef MMU::PD_Entry PD_Entry;

public:
    Setup();

private:
    void build_lm();
    void build_pmm();

    void say_hi();

    void setup_pt(PT_Entry * pts, Phy_Addr base, unsigned int size, unsigned int n_pts, Flags flag, bool print = false);
    void setup_sys_pt();
    void setup_app_pt();
    void setup_sys_pd();

    void setup_flat_paging();

    void enable_paging();

    void load_parts();
    void adjust_perms();
    void call_next();

    void panic() { Machine::panic(); }

private:
    char * bi;
    System_Info * si;

    static volatile bool paging_ready;
};

volatile bool Setup::paging_ready = false;

Setup::Setup()
{
    CPU::int_disable(); // interrupts will be re-enabled at init_end
    Display::init();

    bi = reinterpret_cast<char *>(IMAGE);
    si = reinterpret_cast<System_Info *>(&__boot_time_system_info);

    db<Setup>(TRC) << "Setup(bi=" << reinterpret_cast<void *>(bi) << ",sp=" << CPU::sp() << ")" << endl;
    db<Setup>(INF) << "Setup:si=" << *si << endl;

    if(si->bm.n_cpus > Traits<Machine>::CPUS)
        si->bm.n_cpus = Traits<Machine>::CPUS;

    // Reserve memory for the FLAT_PAGE_TABLE if needed by hidding the respective memory from the system
    if(FLAT_PAGE_TABLE != Memory_Map::NOT_USED)
        si->bm.mem_top = FLAT_PAGE_TABLE - 1;

    if(CPU::id() == 0) { // bootstrap CPU (BSP)

        if(Traits<System>::multitask) {

            // Build the memory model
            build_lm();
            build_pmm();

            // Print basic facts about this EPOS instance
            if(!si->lm.has_app) {
                db<Setup>(ERR) << "No APPLICATION in boot image, you don't need EPOS!" << endl;
                panic();
            }
            if(!si->lm.has_sys)
                db<Setup>(INF) << "No SYSTEM in boot image, assuming EPOS is a library!" << endl;
            say_hi();

            // Configure the memory model defined above
            setup_sys_pt();
            setup_app_pt();
            setup_sys_pd();

            // Enable paging
            enable_paging();

            // Load EPOS parts (e.g. INIT, SYSTEM, APPLICATION)
            load_parts();

        } else { // library mode

            // Print basic facts about this EPOS instance
            say_hi();

            // Configure a flat memory model for the single task in the system
            setup_flat_paging();

            // Enable paging
            enable_paging();

        }

        // Signalizes other CPUs that paging is up
        paging_ready = true;

    } else { // additional CPUs (APs)

        // Wait for the Boot CPU to setup page tables
        while(!paging_ready);
        enable_paging();

    }

    // SETUP ends here, so let's transfer control to next stage (INIT or APP)
    call_next();
}

void Setup::setup_flat_paging()
{
    db<Setup>(TRC) << "Setup::setup_flat_paging()" << endl;

#ifdef __arch_armv7__
    
    CPU::Reg * pt = reinterpret_cast<CPU::Reg *>(FLAT_PAGE_TABLE);
    for(CPU::Reg i = MMU_Common<12,0,20>::directory(RAM_BASE); i < MMU_Common<12,0,20>::directory(RAM_BASE) + MMU_Common<12,0,20>::pages(RAM_TOP - RAM_BASE); i++)
        pt[i] = (i << 20) | ARMv7_MMU::Section_Flags::FLAT_MEMORY_MEM;
    pt[MMU_Common<12,0,20>::directory(MIO_BASE)] = (MMU_Common<12,0,20>::directory(MIO_BASE) << 20) | ARMv7_MMU::Section_Flags::FLAT_MEMORY_DEV;
    for(CPU::Reg i = MMU_Common<12,0,20>::directory(MIO_BASE) + 1; i < MMU_Common<12,0,20>::directory(MIO_BASE) + MMU_Common<12,0,20>::pages(MIO_TOP - MIO_BASE); i++)
        pt[i] = (i << 20) | ARMv7_MMU::Section_Flags::FLAT_MEMORY_PER;

#else

    // Single-level mapping, 32 MB blocks
    static const unsigned long BLOCK_SIZE       = 32 * 1024 * 1024; // 32 MB
    static const unsigned long PD_ENTRIES       = 128; //(Math::max(RAM_TOP, MIO_TOP) - Math::min(RAM_BASE, MIO_BASE)) / 32 * 1024 * 1024; // 128 for 4GB

    CPU::Reg mem = RAM_BASE;
    CPU::Reg * pd = reinterpret_cast<CPU::Reg *>(FLAT_PAGE_TABLE);

    for(unsigned int i = 0; i < PD_ENTRIES; i++, mem += BLOCK_SIZE)
        pd[i] = reinterpret_cast<CPU::Reg>(mem) | ARMv8_MMU::Page_Flags::FLAT_MEM_BLOCK;

//    // Two-level mapping, 16KB granule
//    // To used this code, remember to adjust FLAT_PAGE_TABLE to account for all the associated page tables (e.g. (RAM_TOP - (PD_ETRIES + 1) * 16 * 1024) & ~(0x3fff)
//    static const unsigned long PAGE_SIZE        = 16 * 1024; // 16 KB
//    static const unsigned long PD_ENTRIES       = 128; //(Math::max(RAM_TOP, MIO_TOP) - Math::min(RAM_BASE, MIO_BASE)) / 32 * 1024 * 1024; // 128 for 4GB
//    static const unsigned long PT_ENTRIES       = PAGE_SIZE / sizeof(long); // 2 K entries
//    static const unsigned long PD_MASK          = ((PAGE_SIZE - 1) | (0xfUL << 52));
//
//    CPU::Reg mem = RAM_BASE;
//    CPU::Reg * pts = reinterpret_cast<CPU::Reg *>(FLAT_PAGE_TABLE);
//
//    CPU::Reg * pd = reinterpret_cast<CPU::Reg *>(pts); // PD is the first page in pts_mem
//    pts += PAGE_SIZE;
//    for(unsigned int i = 0; i < PD_ENTRIES; i++, pts += PT_ENTRIES)
//        pd[i] = reinterpret_cast<CPU::Reg>(pts) | ARMv8_MMU::Page_Flags::PAGE_DESCRIPTOR | ARMv8_MMU::Page_Flags::OUTER_SHAREABLE | ARMv8_MMU::Page_Flags::ACCESS | ARMv8_MMU::Page_Flags::EL1_XN | ARMv8_MMU::Page_Flags::XN;
//
//    for(unsigned int i = 0; i < PD_ENTRIES; i++) {
//        CPU::Reg * pt = reinterpret_cast<CPU::Reg *>(pd[i] & ~PD_MASK);
//        db<Setup>(TRC) << "PT[" << i << "]: " << hex << pt << endl;
//        for(unsigned int j = 0; j < PT_ENTRIES; j++, mem += PAGE_SIZE)
//            pt[j] = reinterpret_cast<CPU::Reg>(mem) | ARMv8_MMU::Page_Flags::PAGE_DESCRIPTOR | ARMv8_MMU::Page_Flags::INNER_SHAREABLE | ARMv8_MMU::Page_Flags::SEL_MAIR_ATTR0 | ARMv8_MMU::Page_Flags::ACCESS;
//    }

    CPU::mair_el1((CPU::ATTR_DEVICE_nGnRnE) << 0 * CPU::ATTR_OFFSET | // first attribute_index
        (CPU::ATTR_NORMAL_WT) << 1 * CPU::ATTR_OFFSET | // second attribute index
        (CPU::ATTR_NORMAL_WB) << 2 * CPU::ATTR_OFFSET | // second attribute index
        (CPU::ATTR_NORMAL_NON_CACHE) << 3 * CPU::ATTR_OFFSET); // second attribute index

#endif

}

void Setup::build_lm()
{
    db<Setup>(TRC) << "Setup::build_lm()" << endl;

    // Get boot image structure
    si->lm.has_stp = (si->bm.setup_offset != -1u);
    si->lm.has_ini = (si->bm.init_offset != -1u);
    si->lm.has_sys = (si->bm.system_offset != -1u);
    si->lm.has_app = (si->bm.application_offset != -1u);
    si->lm.has_ext = (si->bm.extras_offset != -1u);

    // Check SETUP integrity and get the size of its segments
    si->lm.stp_entry = 0;
    si->lm.stp_segments = 0;
    si->lm.stp_code = ~0U;
    si->lm.stp_code_size = 0;
    si->lm.stp_data = ~0U;
    si->lm.stp_data_size = 0;
    if(si->lm.has_stp) {
        ELF * stp_elf = reinterpret_cast<ELF *>(&bi[si->bm.setup_offset]);
        if(!stp_elf->valid()) {
            db<Setup>(ERR) << "SETUP ELF image is corrupted!" << endl;
            panic();
        }

        si->lm.stp_entry = stp_elf->entry();
        si->lm.stp_segments = stp_elf->segments();
        si->lm.stp_code = stp_elf->segment_address(0);
        si->lm.stp_code_size = stp_elf->segment_size(0);
        if(stp_elf->segments() > 1) {
            for(int i = 1; i < stp_elf->segments(); i++) {
                if(stp_elf->segment_type(i) != PT_LOAD)
                    continue;
                if(stp_elf->segment_address(i) < si->lm.stp_data)
                    si->lm.stp_data = stp_elf->segment_address(i);
                si->lm.stp_data_size += stp_elf->segment_size(i);
            }
        }
    }

    // Check INIT integrity and get the size of its segments
    si->lm.ini_entry = 0;
    si->lm.ini_segments = 0;
    si->lm.ini_code = ~0U;
    si->lm.ini_code_size = 0;
    si->lm.ini_data = ~0U;
    si->lm.ini_data_size = 0;
    if(si->lm.has_ini) {
        ELF * ini_elf = reinterpret_cast<ELF *>(&bi[si->bm.init_offset]);
        if(!ini_elf->valid()) {
            db<Setup>(ERR) << "INIT ELF image is corrupted!" << endl;
            panic();
        }

        si->lm.ini_entry = ini_elf->entry();
        si->lm.ini_segments = ini_elf->segments();
        si->lm.ini_code = ini_elf->segment_address(0);
        si->lm.ini_code_size = ini_elf->segment_size(0);
        if(ini_elf->segments() > 1) {
            for(int i = 1; i < ini_elf->segments(); i++) {
                if(ini_elf->segment_type(i) != PT_LOAD)
                    continue;
                if(ini_elf->segment_address(i) < si->lm.ini_data)
                    si->lm.ini_data = ini_elf->segment_address(i);
                si->lm.ini_data_size += ini_elf->segment_size(i);
            }
        }
    }

    // Check SYSTEM integrity and get the size of its segments
    si->lm.sys_entry = 0;
    si->lm.sys_segments = 0;
    si->lm.sys_code = ~0U;
    si->lm.sys_code_size = 0;
    si->lm.sys_data = ~0U;
    si->lm.sys_data_size = 0;
    si->lm.sys_stack = SYS_STACK;
    si->lm.sys_stack_size = Traits<System>::STACK_SIZE * si->bm.n_cpus;
    if(si->lm.has_sys) {
        ELF * sys_elf = reinterpret_cast<ELF *>(&bi[si->bm.system_offset]);
        if(!sys_elf->valid()) {
            db<Setup>(ERR) << "OS ELF image is corrupted!" << endl;
            panic();
        }

        si->lm.sys_entry = sys_elf->entry();
        si->lm.sys_segments = sys_elf->segments();
        si->lm.sys_code = sys_elf->segment_address(0);
        si->lm.sys_code_size = sys_elf->segment_size(0);
        if(sys_elf->segments() > 1) {
            for(int i = 1; i < sys_elf->segments(); i++) {
                if(sys_elf->segment_type(i) != PT_LOAD)
                    continue;
                if(sys_elf->segment_address(i) < si->lm.sys_data)
                    si->lm.sys_data = sys_elf->segment_address(i);
                si->lm.sys_data_size += sys_elf->segment_size(i);
            }
        }

        // CODE and DATA Segments are concatenated, only code seg is available...
        if(si->lm.sys_code != SYS_CODE) {
            db<Setup>(ERR) << "OS code segment address (" << reinterpret_cast<void *>(si->lm.sys_code) << ") does not match the machine's memory map (" << reinterpret_cast<void *>(SYS_CODE) << ")!" << endl;
            panic();
        }

        if(si->lm.sys_code + si->lm.sys_code_size > si->lm.sys_data) {
            db<Setup>(ERR) << "OS code segment is too large!" << endl;
            panic();
        }

        if(si->lm.sys_data != SYS_DATA) {
            db<Setup>(ERR) << "OS data segment address (" << reinterpret_cast<void *>(si->lm.sys_data) << ") does not match the machine's memory map (" << reinterpret_cast<void *>(SYS_DATA) << ")!" << endl;
            panic();
        }

        if(si->lm.sys_data + si->lm.sys_data_size > si->lm.sys_stack) {
            db<Setup>(ERR) << "OS data segment is too large!" << endl;
            panic();
        }

        if(MMU::page_tables(MMU::pages(si->lm.sys_stack_size)) > 1) {
            db<Setup>(ERR) << "OS stack segment is too large!" << endl;
            panic();
        }
    }

    // Check APPLICATION integrity and get the size of its segments
    si->lm.app_entry = 0;
    si->lm.app_segments = 0;
    si->lm.app_code = ~0U;
    si->lm.app_code_size = 0;
    si->lm.app_data = ~0U;
    si->lm.app_data_size = 0;
    si->lm.app_extra = ~0U;
    si->lm.app_extra_size = 0;
    if(si->lm.has_app) {
        ELF * app_elf = reinterpret_cast<ELF *>(&bi[si->bm.application_offset]);
        if(!app_elf->valid()) {
            db<Setup>(ERR) << "APP ELF image is corrupted!" << endl;
            panic();
        }
        si->lm.app_entry = app_elf->entry();
        si->lm.app_segments = app_elf->segments();
        si->lm.app_code = app_elf->segment_address(0);
        if(si->lm.app_code != MMU::align_directory(si->lm.app_code)) {
            db<Setup>(ERR) << "Unaligned APP CODE image:" << hex << si->lm.app_code << endl;
            panic();
        }
        si->lm.app_code_size = app_elf->segment_size(0);
        if(app_elf->segments() > 1) {
            for(int i = 1; i < app_elf->segments(); i++) {
                if(app_elf->segment_type(i) != PT_LOAD)
                    continue;
                if(app_elf->segment_address(i) < si->lm.app_data)
                    si->lm.app_data = app_elf->segment_address(i);
                si->lm.app_data_size += app_elf->segment_size(i);
            }
        }
        if(si->lm.app_data == ~0U) {
            db<Setup>(WRN) << "APP ELF image has no data segment!" << endl;
            si->lm.app_data = MMU::align_page(APP_DATA);
        }
        if(Traits<System>::multiheap) { // Application heap in data segment
            si->lm.app_data_size = MMU::align_page(si->lm.app_data_size);
            si->lm.app_stack = si->lm.app_data + si->lm.app_data_size;
            si->lm.app_data_size += MMU::align_page(Traits<Application>::STACK_SIZE);
            si->lm.app_heap = si->lm.app_data + si->lm.app_data_size;
            si->lm.app_data_size += MMU::align_page(Traits<Application>::HEAP_SIZE);
        }
        if(si->lm.has_ext) { // Check for EXTRA data in the boot image
            si->lm.app_extra = si->lm.app_data + si->lm.app_data_size;
            si->lm.app_extra_size = si->bm.img_size - si->bm.extras_offset;
            if(Traits<System>::multiheap)
                si->lm.app_extra_size = MMU::align_page(si->lm.app_extra_size);
            si->lm.app_data_size += si->lm.app_extra_size;
        }
    }
}

void Setup::build_pmm()
{
    db<Setup>(TRC) << "Setup::build_pmm() => top_page=" << MMU::pages(si->bm.mem_top) << endl;

    // Allocate (reserve) memory for all entities we have to setup.
    // We'll start at the highest address to make possible a memory model
    // on which the application's logical and physical address spaces match.
    Phy_Addr top_page = MMU::pages(si->bm.mem_top);

    if(Traits<CPU>::WORD_SIZE == 32) {
        // System Page Directory (4 x sizeof(Page)) -- up to 4k PD entries of 32 bits, moreover we need a page aligned to 16 KB 
        top_page -= 4;
        si->pmm.sys_pd = top_page * sizeof(Page);

        // Page tables to map the System address space
        top_page -= 1;
        si->pmm.sys_pt = top_page * sizeof(Page);
    } else {
        // System Page Directory (1 x sizeof(Page)) -- up to 2048 PD entries of 64 bits, moreover we need a page aligned to 16 KB 
        top_page -= 1;
        si->pmm.sys_pd = top_page * sizeof(Page);

        // Page tables to map the System address space
        top_page -= MMU::pages(SYS_HIGH + 1 - SYS);
        si->pmm.sys_pt = top_page * sizeof(Page);
    }

    // Page tables to map the whole physical memory
    // = NP/NPTE_PT * sizeof(Page)
    //   NP = size of physical memory in pages
    //   NPTE_PT = number of page table entries per page table, e.g., 256 in this config (12, 8, 12)
    db<Setup>(INF) << "Setup::build_pmm(): mmu::pages for phy_mem_pts;" << MMU::page_tables(MMU::pages(si->bm.mem_top - si->bm.mem_base)) << ", and curr top_page=" << top_page << endl;
    top_page -= MMU::page_tables(MMU::pages(si->bm.mem_top - si->bm.mem_base));
    si->pmm.phy_mem_pts = top_page * sizeof(Page);

    // Page tables to map the IO address space
    // = NP/NPTE_PT * sizeof(Page)
    // NP = size of I/O address space in pages
    // NPTE_PT = number of page table entries per page table
    top_page -= MMU::page_tables(MMU::pages(si->bm.mio_top - si->bm.mio_base));
    si->pmm.io_pts = top_page * sizeof(Page);

    // Page tables to map the first APPLICATION code segment
    top_page -= MMU::page_tables(MMU::pages(si->lm.app_code_size));
    si->pmm.app_code_pts = top_page * sizeof(Page);

    // Page tables to map the first APPLICATION data segment (which contains heap, stack and extra)
    top_page -= MMU::page_tables(MMU::pages(si->lm.app_data_size));
    si->pmm.app_data_pts = top_page * sizeof(Page);

    // System Info (1 x sizeof(Page))
    if (SYS_INFO != Traits<Machine>::NOT_USED)
        top_page -= 1;
    si->pmm.sys_info = top_page * sizeof(Page);

    // SYSTEM code segment -- For this test, everything will be in physical memory 
    top_page -= MMU::pages(si->lm.sys_code_size);
    si->pmm.sys_code = top_page * sizeof(Page);

    // SYSTEM data segment
    top_page -= MMU::pages(si->lm.sys_data_size);
    si->pmm.sys_data = top_page * sizeof(Page);

    // The memory allocated so far will "disappear" from the system as we set mem_top as follows:
    si->pmm.usr_mem_base = si->bm.mem_base;
    si->pmm.usr_mem_top = top_page * sizeof(Page);

    // APPLICATION code segment
    top_page -= MMU::pages(si->lm.app_code_size);
    si->pmm.app_code = top_page * sizeof(Page);

    // APPLICATION data segment (contains stack, heap and extra)
    top_page -= MMU::pages(si->lm.app_data_size);
    si->pmm.app_data = top_page * sizeof(Page);

    // SYSTEM stack segment -- We use boot stack right after sys_pt
    top_page -= MMU::pages(si->lm.sys_stack_size);
    si->pmm.sys_stack = top_page * sizeof(Page);

    // Free chunks (passed to MMU::init)
    si->pmm.free1_base = si->lm.stp_code + si->lm.stp_code_size + si->lm.stp_data_size; // vector table should not be deleted!
    si->pmm.free1_top = top_page * sizeof(Page); // we will free the stack here
    db<Setup>(TRC) << "Top page = " << top_page << endl;

    // Test if we didn't overlap SETUP and the boot image
    if(si->pmm.usr_mem_top <= si->lm.stp_code + si->lm.stp_code_size + si->lm.stp_data_size) {
        db<Setup>(ERR) << "SETUP would have been overwritten!" << endl;
        panic();
    }
}

void Setup::say_hi()
{
    db<Setup>(TRC) << "Setup::say_hi()" << endl;
    db<Setup>(INF) << "System_Info=" << *si << endl;
    db<Setup>(INF) << "Boot_Stack=" << reinterpret_cast<void *>(Memory_Map::BOOT_STACK) << " (sp=" << CPU::sp() << ")" << endl;

    kout << endl;

    kout << "This is EPOS!\n" << endl;
    kout << "Setting up this machine as follows: " << endl;
    kout << "  Mode:         " << ((Traits<Build>::MODE == Traits<Build>::LIBRARY) ? "library" : (Traits<Build>::MODE == Traits<Build>::BUILTIN) ? "built-in" : "kernel") << endl;
    kout << "  Processor:    " << Traits<Machine>::CPUS << " x Cortex A53 (ARMv" << ((Traits<CPU>::WORD_SIZE == 32) ? "7" : "8") << "-A) at " << Traits<CPU>::CLOCK / 1000000 << " MHz (BUS clock = " << Traits<CPU>::CLOCK / 1000000 << " MHz)" << endl;
    kout << "  Machine:      Raspberry Pi3" << endl;
    kout << "  Memory:       " << (si->bm.mem_top - si->bm.mem_base) / 1024 << " KB [" << reinterpret_cast<void *>(si->bm.mem_base) << ":" << reinterpret_cast<void *>(si->bm.mem_top) << "]" << endl;
    kout << "  User memory:  " << (si->pmm.usr_mem_top - si->pmm.usr_mem_base) / 1024 << " KB [" << reinterpret_cast<void *>(si->pmm.usr_mem_base) << ":" << reinterpret_cast<void *>(si->pmm.usr_mem_top) << "]" << endl;
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
    if(si->lm.has_stp)
        kout << "  Setup:        " << si->lm.stp_code_size + si->lm.stp_data_size << " bytes" << endl;
    if(si->lm.has_ini)
        kout << "  Init:         " << si->lm.ini_code_size + si->lm.ini_data_size << " bytes" << endl;
    if(si->lm.has_sys)
        kout << "  OS code:      " << si->lm.sys_code_size << " bytes" << "\tdata: " << si->lm.sys_data_size << " bytes" << "    stack: " << si->lm.sys_stack_size << " bytes" << endl;
    if(si->lm.has_app)
        kout << "  APP code:     " << si->lm.app_code_size << " bytes" << "\tdata: " << si->lm.app_data_size << " bytes" << endl;
    if(si->lm.has_ext)
        kout << "  Extras:       " << si->lm.app_extra_size << " bytes" << endl;

    kout << endl;
}

void Setup::setup_pt(PT_Entry * pts, Phy_Addr base, unsigned int size, unsigned int n_pts, Flags flag, bool print)
{
    // n_pts equal to the number of PDs necessary to map the requested PTEs (given by size) from the memory base 
    // Each PTE maps one Page (4k), 
    // Each Page can have 4 pages with 256 ptes each
    // Thus, for each PD, map 256 pte until all requested ptes are mapped
    for(unsigned int i = 0; i < size; i++) {
        pts[i] = MMU::phy2pte((base + i * sizeof(Page)), flag);
        if(Traits<Setup>::hysterically_debugged && print)
            db<Setup>(INF) << "pts[" << i << "]=" << pts[i] << ",addr="<< &pts[i] << endl;
    }
}

void Setup::setup_sys_pt()
{
    db<Setup>(TRC) << "Setup::setup_sys_pt(pmm="
                   << "{si="      << reinterpret_cast<void *>(si->pmm.sys_info)
                   << ",pt="      << reinterpret_cast<void *>(si->pmm.sys_pt)
                   << ",pd="      << reinterpret_cast<void *>(si->pmm.sys_pd)
                   << ",sysc={b=" << reinterpret_cast<void *>(si->pmm.sys_code) << ",s=" << MMU::pages(si->lm.sys_code_size) << "}"
                   << ",sysd={b=" << reinterpret_cast<void *>(si->pmm.sys_data) << ",s=" << MMU::pages(si->lm.sys_data_size) << "}"
                   << ",syss={b=" << reinterpret_cast<void *>(si->pmm.sys_stack) << ",s=" << MMU::pages(si->lm.sys_stack_size) << "}"
                   << "})" << endl;

    // Get the physical address for the SYSTEM Page Table
    PT_Entry * sys_pt = reinterpret_cast<PT_Entry *>(si->pmm.sys_pt);

    // Clear the System Page Table
    memset(sys_pt, 0, sizeof(Page));

    // System Info
    sys_pt[MMU::directory(SYS_INFO - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_INFO)] = MMU::phy2pte(si->pmm.sys_info, Flags::SYS);
    db<Setup>(INF) << "SYS_INFO PT = " << MMU::directory(SYS_INFO - SYS) * (MMU::PT_ENTRIES-1) + MMU::page(SYS_INFO) << endl;

    // Set an entry to this page table, so the system can access it later -- 1 page
    sys_pt[MMU::directory(SYS_PT - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_PT)]     = MMU::phy2pte(si->pmm.sys_pt, Flags::SYS);
    db<Setup>(INF) << "SYS_PT PT = " << MMU::directory(SYS_PT - SYS) * (MMU::PT_ENTRIES-1) + MMU::page(SYS_PT) << endl;

    // System Page Directory -- 4 Pages for directory
    sys_pt[MMU::directory(SYS_PD - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_PD)]   = MMU::phy2pte(si->pmm.sys_pd, Flags::SYS);
#ifdef __arch_armv7__
    sys_pt[MMU::directory(SYS_PD - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_PD)+1] = MMU::phy2pte(si->pmm.sys_pd + sizeof(Page), Flags::SYS);
    sys_pt[MMU::directory(SYS_PD - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_PD)+2] = MMU::phy2pte(si->pmm.sys_pd + 2 * sizeof(Page), Flags::SYS);
    sys_pt[MMU::directory(SYS_PD - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_PD)+3] = MMU::phy2pte(si->pmm.sys_pd + 3 * sizeof(Page), Flags::SYS);
#endif
    // SYSTEM code
    setup_pt(reinterpret_cast<PT_Entry *>(&sys_pt[MMU::directory(SYS_CODE - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_CODE)]), si->pmm.sys_code, MMU::pages(si->lm.sys_code_size), MMU::page_tables(MMU::pages(si->lm.sys_code_size)), Flags::SYS, true);
    db<Setup>(INF) << "SYS_CODE PT = " << MMU::directory(SYS_CODE - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_CODE) << ",size=" << si->lm.sys_code_size << endl;
    
    // SYSTEM data
    setup_pt(reinterpret_cast<PT_Entry *>(&sys_pt[MMU::directory(SYS_DATA - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_DATA)]), si->pmm.sys_data, MMU::pages(si->lm.sys_data_size), MMU::page_tables(MMU::pages(si->lm.sys_data_size)), Flags::SYS, true);
    db<Setup>(INF) << "SYS_DATA PT = " << MMU::directory(SYS_DATA - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_DATA) << ",size=" << si->lm.sys_data_size << endl;

    // SYSTEM stack (used only during init and for the ukernel model)
    setup_pt(reinterpret_cast<PT_Entry *>(&sys_pt[MMU::directory(SYS_STACK - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_STACK)]), si->pmm.sys_stack, MMU::pages(si->lm.sys_stack_size), MMU::page_tables(MMU::pages(si->lm.sys_stack_size)), Flags::SYS, true);
    db<Setup>(INF) << "SYS_STACK PT = " << MMU::directory(SYS_STACK - SYS) * (MMU::PT_ENTRIES) + MMU::page(SYS_STACK) << ",size=" << si->lm.sys_stack_size << endl;
    
    db<Setup>(TRC) << "SYS_PT=" << *reinterpret_cast<Page_Table *>(sys_pt) << endl;
}

void Setup::setup_app_pt()
{
    db<Setup>(TRC) << "Setup::setup_app_pt(appc={b=" << reinterpret_cast<void *>(si->pmm.app_code) << ",s=" << MMU::pages(si->lm.app_code_size) << "}"
                   << ",appd={b=" << reinterpret_cast<void *>(si->pmm.app_data) << ",s=" << MMU::pages(si->lm.app_data_size) << "}"
                   << ",appe={b=" << reinterpret_cast<void *>(si->pmm.app_extra) << ",s=" << MMU::pages(si->lm.app_extra_size) << "}"
                   << "})" << endl;

    // Get the physical address for the first APPLICATION Page Tables
    PT_Entry * app_code_pt = reinterpret_cast<PT_Entry *>(si->pmm.app_code_pts);
    PT_Entry * app_data_pt = reinterpret_cast<PT_Entry *>(si->pmm.app_data_pts);

    // Clear the first APPLICATION Page Tables
    memset(app_code_pt, 0, MMU::page_tables(MMU::pages(si->lm.app_code_size)) * sizeof(Page_Table));
    memset(app_data_pt, 0, MMU::page_tables(MMU::pages(si->lm.app_data_size)) * sizeof(Page_Table));

    // APPLICATION code
    setup_pt(reinterpret_cast<PT_Entry *>(&app_code_pt[MMU::page(si->lm.app_code)]), si->pmm.app_code, MMU::pages(si->lm.app_code_size), MMU::page_tables(MMU::pages(si->lm.app_code_size)), Flags::APP);

    // APPLICATION data (contains stack, heap and extra)
    setup_pt(reinterpret_cast<PT_Entry *>(&app_data_pt[MMU::page(si->lm.app_data)]), si->pmm.app_data, MMU::pages(si->lm.app_data_size), MMU::page_tables(MMU::pages(si->lm.app_data_size)), Flags::APP);

    db<Setup>(INF) << "APPC_PT=" << *reinterpret_cast<Page_Table *>(app_code_pt) << endl;
    db<Setup>(INF) << "APPD_PT=" << *reinterpret_cast<Page_Table *>(app_data_pt) << endl;
}

void Setup::setup_sys_pd()
{
    db<Setup>(TRC) << "setup_sys_pd(bm="
                   << "{memb="  << reinterpret_cast<void *>(si->bm.mem_base)
                   << ",memt="  << reinterpret_cast<void *>(si->bm.mem_top)
                   << ",miob="  << reinterpret_cast<void *>(si->bm.mio_base)
                   << ",miot="  << reinterpret_cast<void *>(si->bm.mio_top)
                   << "{si="    << reinterpret_cast<void *>(si->pmm.sys_info)
                   << ",spt="   << reinterpret_cast<void *>(si->pmm.sys_pt)
                   << ",spd="   << reinterpret_cast<void *>(si->pmm.sys_pd)
                   << ",mem="   << reinterpret_cast<void *>(si->pmm.phy_mem_pts)
                   << ",io="    << reinterpret_cast<void *>(si->pmm.io_pts)
                   << ",umemb=" << reinterpret_cast<void *>(si->pmm.usr_mem_base)
                   << ",umemt=" << reinterpret_cast<void *>(si->pmm.usr_mem_top)
                   << ",sysc="  << reinterpret_cast<void *>(si->pmm.sys_code)
                   << ",sysd="  << reinterpret_cast<void *>(si->pmm.sys_data)
                   << ",syss="  << reinterpret_cast<void *>(si->pmm.sys_stack)
                   << ",apct="  << reinterpret_cast<void *>(si->pmm.app_code_pts)
                   << ",apdt="  << reinterpret_cast<void *>(si->pmm.app_data_pts)
                   << ",fr1b="  << reinterpret_cast<void *>(si->pmm.free1_base)
                   << ",fr1t="  << reinterpret_cast<void *>(si->pmm.free1_top)
                   << ",fr2b="  << reinterpret_cast<void *>(si->pmm.free2_base)
                   << ",fr2t="  << reinterpret_cast<void *>(si->pmm.free2_top)
                   << "})" << endl;

    // Get the physical address for the System Page Directory
    PT_Entry * sys_pd = reinterpret_cast<PT_Entry *>(si->pmm.sys_pd);

    // Clear the System Page Directory
    memset(sys_pd, 0, sizeof(Page));

    // Calculate the number of page tables needed to map the physical memory
    unsigned int mem_size = MMU::pages(si->bm.mem_top - si->bm.mem_base);
    unsigned int n_pts = MMU::page_tables(mem_size);
    db<Setup>(INF) << "mem_size="<< mem_size << ",n_pts=" << n_pts << reinterpret_cast<void *>( si->pmm.phy_mem_pts) << ",syspd=" << reinterpret_cast<void *>( si->pmm.sys_pd) << "Size pte=" << sizeof(PT_Entry) << endl;
    // Map the whole physical memory into the page tables pointed by phy_mem_pts
    PT_Entry * pts = reinterpret_cast<PT_Entry *>(si->pmm.phy_mem_pts);

    db<Setup>(INF) << &pts[0] <<  " to "<< &pts[(n_pts-1) * sizeof(Page_Table) + MMU::PT_ENTRIES] << endl;

    setup_pt(pts, si->bm.mem_base, mem_size, n_pts, Flags::SYS);

    db<Setup>(INF) << "PHY_MEM_PT("<< (void *) pts <<")=" << *reinterpret_cast<Page_Table *>(pts) << endl; // if this print is removed, the config goes wrong
    db<Setup>(INF) << "mem_size="<< mem_size <<",n_pts=" << n_pts << endl;

    db<Setup>(INF) << "pts done" << endl;
    // Attach all physical memory starting at PHY_MEM
    if (PHY_MEM != RAM_BASE) {
        assert((MMU::directory(MMU::align_directory(PHY_MEM)) + n_pts) < (MMU::PD_ENTRIES - 3)); // check if it would overwrite the OS
        for(unsigned int i = MMU::directory(MMU::align_directory(PHY_MEM)), j = 0; i < MMU::directory(MMU::align_directory(PHY_MEM)) + n_pts; i++, j++)
            sys_pd[i] = MMU::phy2pde(si->pmm.phy_mem_pts + j * sizeof(Page_Table));
        db<Setup>(INF) << "sys pd PHY_MEM  done" << endl;
    }

    // Attach the portion of the physical memory used by Setup at SETUP
    sys_pd[MMU::directory(SETUP)] =  MMU::phy2pde(si->pmm.phy_mem_pts);
    db<Setup>(INF) << "sys pd on SETUP directory = " << MMU::directory(SETUP) << endl;

    // Attach all physical memory starting at RAM_BASE
    assert((MMU::directory(MMU::align_directory(RAM_BASE)) + n_pts) < (MMU::PD_ENTRIES - 2)); // check if it would overwrite the OS
    for(unsigned int i = MMU::directory(MMU::align_directory(RAM_BASE)), j = 0; i < MMU::directory(MMU::align_directory(RAM_BASE)) + n_pts; i++, j++)
        sys_pd[i] = MMU::phy2pde(si->pmm.phy_mem_pts + j * sizeof(Page_Table));
    db<Setup>(INF) << "sys pd RAM_BASE done, dir= " << MMU::directory(RAM_BASE) << endl;

    // Calculate the number of page tables needed to map the IO address space
    unsigned int io_size = MMU::pages(si->bm.mio_top - si->bm.mio_base);
    n_pts = MMU::page_tables(io_size);

    db<Setup>(INF) << "starting io pts, npts=" << n_pts << endl;
    // Map IO address space into the page tables pointed by io_pts
    pts = reinterpret_cast<PT_Entry *>(si->pmm.io_pts);
    setup_pt(pts, si->bm.mio_base, io_size, n_pts, Flags::IO);

    db<Setup>(INF) << "IO_PTS("<< (void *) pts <<")=" << *reinterpret_cast<Page_Table *>(pts) << endl; // if this print is removed, the config g
    db<Setup>(INF) << "io pts done" << endl;

    // Attach devices' memory at Memory_Map::IO
    assert((MMU::directory(MMU::align_directory(IO)) + n_pts) < (MMU::PD_ENTRIES - 1)); // check if it would overwrite the OS
    for(unsigned int i = MMU::directory(MMU::align_directory(IO)), j = 0; i < MMU::directory(MMU::align_directory(IO)) + n_pts; i++, j++)
        sys_pd[i] = MMU::phy2pde((si->pmm.io_pts + j * sizeof(Page_Table)));
    db<Setup>(INF) << "sys pd for io pts done" << endl;

    db<Setup>(INF) << "attach SYS pt on sys pd[sys]:" << MMU::directory(SYS) 
                    << ", with sys_pt[0] = " <<  hex << *(reinterpret_cast<int *>( si->pmm.sys_pt)) << endl;
    db<Setup>(INF) << "sys_pd[sys+1];" << MMU::directory(SYS)+1 
                    << ", with sys_pt[1] = " << hex << *((int *) (si->pmm.sys_pt+sizeof(Page_Table))) << endl;
    // Attach the OS (i.e. sys_pt)
    // One sys_pt for code
    sys_pd[MMU::directory(SYS)] = MMU::phy2pde(si->pmm.sys_pt);
    // Two sys_pt for data
    sys_pd[MMU::directory(SYS) + 1] = MMU::phy2pde(si->pmm.sys_pt + sizeof(Page_Table));
    sys_pd[MMU::directory(SYS) + 2] = MMU::phy2pde(si->pmm.sys_pt + 2 * sizeof(Page_Table));
    db<Setup>(INF) << "attach SYS on sys pd done" << endl;

    // Attach the first APPLICATION CODE (i.e. app_code_pt)
    n_pts = MMU::page_tables(MMU::pages(si->lm.app_code_size));
    for(unsigned int i = MMU::directory(MMU::align_directory(si->lm.app_code)), j = 0; i < MMU::directory(MMU::align_directory(si->lm.app_code)) + n_pts; i++, j++)
        sys_pd[i] = MMU::phy2pde(si->pmm.app_code_pts + j * sizeof(Page_Table));

    // Attach the first APPLICATION DATA (i.e. app_data_pt, containing heap, stack and extra)
    n_pts = MMU::page_tables(MMU::pages(si->lm.app_data_size));
    for(unsigned int i = MMU::directory(MMU::align_directory(si->lm.app_data)), j = 0; i < MMU::directory(MMU::align_directory(si->lm.app_data)) + n_pts; i++, j++)
        sys_pd[i] = MMU::phy2pde(si->pmm.app_data_pts + j * sizeof(Page_Table));

    db<Setup>(INF) << "SYS_PD=" << *reinterpret_cast<Page_Table *>(sys_pd) << endl;
}

void Setup::enable_paging()
{
    db<Setup>(TRC) << "Setup::enable_paging()" << endl;
    if(Traits<Setup>::hysterically_debugged) {
        db<Setup>(INF) << "pc=" << CPU::pc() << endl;
        db<Setup>(INF) << "sp=" << CPU::sp() << endl;
    }

#ifdef __arch_armv7__

    // MNG_DOMAIN for no page permission verification, CLI_DOMAIN otherwise
    // CPU::dacr((Traits<System>::multitask) ? CPU::CLI_DOMAIN : CPU::MNG_DOMAIN); 
    CPU::dacr(CPU::MNG_DOMAIN);

    CPU::dsb();
    CPU::isb();

    // Clear TTBCR for the system to use ttbr0 instead of 1
    CPU::ttbcr(0);
    // Set ttbr0 with base address
    CPU::ttbr0((Traits<System>::multitask) ? si->pmm.sys_pd : FLAT_PAGE_TABLE);

    // Enable MMU through SCTLR and ACTLR
    CPU::actlr(CPU::actlr() | CPU::SMP); // Set SMP bit
    CPU::sctlr((CPU::sctlr() | CPU::DCACHE | CPU::ICACHE | CPU::MMU_ENABLE) & ~(CPU::AFE));

    CPU::dsb();
    CPU::isb();

    // MMU now enabled - Virtual address system now active
    // Branch Prediction Enable
    CPU::sctlr(CPU::sctlr() | (1 << 11)); // Z bit

    // Flush TLB to ensure we've got the right memory organization
    MMU::flush_tlb();

    // Adjust pointers that will still be used to their logical addresses
    Display::init(); // adjust the pointers in Display by calling init 

    if(Traits<Setup>::hysterically_debugged) {
        db<Setup>(INF) << "pc=" << CPU::pc() << endl;
        db<Setup>(INF) << "sp=" << CPU::sp() << endl;
    }

#else 

    // Configure paging with two levels and 16KB pages via TTBRC
    CPU::ttbcr(CPU::TTBR1_DISABLE | CPU::TTBR0_WALK_INNER_SHAREABLE | CPU::TTBR0_WALK_OUTER_WB_WA | CPU::TTBR0_WALK_INNER_WB_WA | CPU::TTBR0_TG0_16KB | CPU::TTBR0_SIZE_4GB);
    CPU::isb();

    // Tell the MMU where our translation tables are
    db<Setup>(INF) << "SYS_PD=" << hex << (unsigned long)((Traits<System>::multitask) ? si->pmm.sys_pd : FLAT_PAGE_TABLE) << endl;
    CPU::ttbr0((unsigned long)((Traits<System>::multitask) ? si->pmm.sys_pd : FLAT_PAGE_TABLE));

    CPU::dsb();
    CPU::isb();

    CPU::sctlr(CPU::sctlr() | CPU::MMU_ENABLE | CPU::DCACHE | CPU::ICACHE);
    CPU::isb();

    Display::init();
#endif
}

void Setup::load_parts()
{
    db<Setup>(TRC) << "Setup::load_parts()" << endl;

    // Relocate System_Info
    if(sizeof(System_Info) > sizeof(Page))
        db<Setup>(WRN) << "System_Info is bigger than a page (" << sizeof(System_Info) << ")!" << endl;

    if(Traits<Setup>::hysterically_debugged)
        db<Setup>(INF) << "Setup:SYS_INFO: " << MMU::Translation(SYS_INFO) << endl;
    memcpy(reinterpret_cast<void *>(SYS_INFO), si, sizeof(System_Info));
    si = reinterpret_cast<System_Info *>(SYS_INFO);

    // Load INIT
    if(si->lm.has_ini) {
        db<Setup>(TRC) << "Setup::load_init()" << endl;
        ELF * ini_elf = reinterpret_cast<ELF *>(&bi[si->bm.init_offset]);
        if(Traits<Setup>::hysterically_debugged) {
            db<Setup>(INF) << "Setup:ini_elf: " << MMU::Translation(ini_elf) << endl;
            db<Setup>(INF) << "Setup:ini_elf[0]: " << MMU::Translation(ini_elf->segment_address(0)) << endl;
            db<Setup>(INF) << "Setup:ini_elf[0].size: " << ini_elf->segment_size(0) << endl;
        }
        if(ini_elf->load_segment(0) < 0) {
            db<Setup>(ERR) << "INIT code segment was corrupted during SETUP!" << endl;
            panic();
        }
        for(int i = 1; i < ini_elf->segments(); i++)
            if(ini_elf->load_segment(i) < 0) {
                db<Setup>(ERR) << "INIT data segment was corrupted during SETUP!" << endl;
                panic();
            }
    }

    // Load SYSTEM
    if(si->lm.has_sys) {
        db<Setup>(TRC) << "Setup::load_os()" << endl;
        ELF * sys_elf = reinterpret_cast<ELF *>(&bi[si->bm.system_offset]);
        if(Traits<Setup>::hysterically_debugged) {
            db<Setup>(INF) << "Setup:sys_elf: " << MMU::Translation(sys_elf) << endl;
            db<Setup>(INF) << "Setup:sys_elf[0]: " << MMU::Translation(sys_elf->segment_address(0)) << endl;
            db<Setup>(INF) << "Setup:sys_elf[0].size: " << sys_elf->segment_size(0) << endl;
        }
        if(sys_elf->load_segment(0) < 0 || sys_elf->load_segment(1) < 0) {
            db<Setup>(ERR) << "OS code segment was corrupted during SETUP!" << endl;
            panic();
        }
        for(int i = 2; i < sys_elf->segments(); i++) {
            if(sys_elf->load_segment(i) < 0) {
                db<Setup>(ERR) << "OS data segment was corrupted during SETUP!" << endl;
                panic();
            }
        }
    }

    // Load APP
    if(si->lm.has_app) {
        db<Setup>(TRC) << "Setup::load_app()" << endl;
        ELF * app_elf = reinterpret_cast<ELF *>(&bi[si->bm.application_offset]);
        if(Traits<Setup>::hysterically_debugged) {
            db<Setup>(INF) << "Setup:app_elf: " << (void*)app_elf << endl;
            db<Setup>(INF) << "Setup:app_elf: " << MMU::Translation(app_elf) << endl;
            db<Setup>(INF) << "Setup:app_elf[0]: " << MMU::Translation(app_elf->segment_address(0)) << endl;
            db<Setup>(INF) << "Setup:app_elf[0].size: " << app_elf->segment_size(0) << endl;
        }
        if(app_elf->load_segment(0) < 0) {
            db<Setup>(ERR) << "Application code segment was corrupted during SETUP!" << endl;
            panic();
        }
        for(int i = 1; i < app_elf->segments(); i++) {
            if(app_elf->load_segment(i) < 0) {
                db<Setup>(ERR) << "Application data segment was corrupted during SETUP!" << endl;
                panic();
            }
        }
    }

    // Load EXTRA
    if(si->lm.has_ext) {
        db<Setup>(TRC) << "Setup::load_extra()" << endl;
        if(Traits<Setup>::hysterically_debugged)
            db<Setup>(INF) << "Setup:APP_EXTRA:" << MMU::Translation(si->lm.app_extra) << endl;
        memcpy(reinterpret_cast<void *>(si->lm.app_extra), &bi[si->bm.extras_offset], si->lm.app_extra_size);
    }
}

void Setup::adjust_perms()
{
    db<Setup>(TRC) << "Setup::adjust_perms(appc={b=" << reinterpret_cast<void *>(si->pmm.app_code) << ",s=" << MMU::pages(si->lm.app_code_size) << "}"
                   << ",appd={b=" << reinterpret_cast<void *>(si->pmm.app_data) << ",s=" << MMU::pages(si->lm.app_data_size) << "}"
                   << ",appe={b=" << reinterpret_cast<void *>(si->pmm.app_extra) << ",s=" << MMU::pages(si->lm.app_extra_size) << "}"
                   << "})" << endl;


    // Get the logical address of the first APPLICATION Page Tables
    PT_Entry * app_code_pt = MMU::phy2log(reinterpret_cast<PT_Entry *>(si->pmm.app_code_pts));
    PT_Entry * app_data_pt = MMU::phy2log(reinterpret_cast<PT_Entry *>(si->pmm.app_data_pts));

    unsigned int i;
    PT_Entry aux;

    // APPLICATION code
    for(i = 0, aux = si->pmm.app_code; i < MMU::pages(si->lm.app_code_size); i++, aux = aux + sizeof(Page))
        app_code_pt[MMU::page(APP_CODE) + i] = MMU::phy2pte(aux, Flags::APPC);

    // APPLICATION data (contains stack, heap and extra)
    for(i = 0, aux = si->pmm.app_data; i < MMU::pages(si->lm.app_data_size); i++, aux = aux + sizeof(Page))
        app_data_pt[MMU::page(APP_DATA) + i] = MMU::phy2pte(aux, Flags::APPD);
}

void Setup::call_next()
{
    // Check for next stage and obtain the entry point
    Log_Addr ip;
    Reg cpu_id = CPU::id();

    if(Traits<System>::multitask) {
        int cpu_id = CPU::id();
        if(si->lm.has_ini) {
            if(cpu_id == 0) {
                db<Setup>(TRC) << "Executing system's global constructors ..." << endl;
                reinterpret_cast<void (*)()>(reinterpret_cast<void *>(si->lm.sys_entry))();
            }
            ip = si->lm.ini_entry;
        } else if(si->lm.has_sys)
            ip = si->lm.sys_entry;
        else
            ip = si->lm.app_entry;

        // Arrange a stack for each CPU to support stage transition
        Log_Addr sp = SYS_STACK + Traits<Machine>::STACK_SIZE * (cpu_id + 1);

        db<Setup>(TRC) << "Setup::call_next(ip=" << ip << ",sp=" << sp << ") => ";
        if(si->lm.has_ini)
            db<Setup>(TRC) << "INIT" << endl;
        else if(si->lm.has_sys)
            db<Setup>(TRC) << "SYSTEM" << endl;
        else
            db<Setup>(TRC) << "APPLICATION" << endl;

        CPU::sp(sp);
    } else
        ip = &_start;

    db<Setup>(INF) << "SETUP ends here!" << endl;

    // Set SP and call next stage
    static_cast<void (*)()>(ip)();

    if(Traits<System>::multitask && (cpu_id == 0)) {
        // This will only happen when INIT was called and Thread was disabled
        // Note we don't have the original stack here anymore!
        reinterpret_cast<void (*)()>(si->lm.app_entry)();
    }

    // SETUP is now part of the free memory and this point should never be
    // reached, but, just in case ... :-)
    panic();
}

__END_SYS

using namespace EPOS::S;

#ifdef __arch_armv7__

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
    // QEMU get us here in SVC mode with interrupt disabled, but the real Raspberry Pi3 starts in hypervisor mode, so we must switch to SVC mode
    if(!Traits<Machine>::emulated) {
        CPU::Reg cpsr = CPU::psr();
        cpsr &= ~CPU::FLAG_M;           // clear mode bits
        cpsr |= CPU::MODE_SVC;          // set supervisor flag
        CPU::psrc(cpsr);                // enter supervisor mode
        CPU::Reg address = CPU::ra();
        CPU::elr_hyp(address);
        CPU::tmp_to_psr();
    }

    // Configure a stack for SVC mode, which will be used until the first Thread is created
    CPU::mode(CPU::MODE_SVC); // enter SVC mode (with interrupts disabled)
    CPU::sp(Memory_Map::BOOT_STACK + Traits<Machine>::STACK_SIZE * (CPU::id() + 1) - sizeof(long));

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

        // Adjust VBAR
        CPU::vbar(Memory_Map::VECTOR_TABLE);

        // Clear the BSS (SETUP was linked to CRT0, but entry point didn't go through BSS clear)
        Machine::clear_bss();
    } else {
        BCM_Mailbox * mbox = reinterpret_cast<BCM_Mailbox *>(Memory_Map::MBOX_CTRL_BASE);
        mbox->eoi(0);
        mbox->enable();
    }

    CPU::fpu_enable();
    CPU::flush_caches();
    CPU::flush_branch_predictors();
    CPU::flush_tlb();
    CPU::actlr(CPU::actlr() | CPU::DCACHE_PREFETCH); // enable Dside prefetch

    _setup();
}

#else

void _entry()
{
    // Manual D13.2.102 explains the cold start of an ARMv8, with RVBAR_EL3 defined by the SoC's manufaturer
    // Broadcom uses 0 and the GPU initialized that memory region with code that end up jumping to 0x80000 (i.e. _entry)

    // Set SP to avoid early faults
    CPU::sp(Memory_Map::BOOT_STACK + Traits<Machine>::STACK_SIZE * (CPU::id() + 1)); // set stack

    ASM("b _reset");
}

void _vector_table()
{
    // Manual D1.10.2, page D1-1430 shows four configurations in terms of the exception level an exception can came from:
    // current level with SP_EL0, current level with SP_ELx, lower level AArch64, and lower level AArch32.
    // Additionally, there are four exception types: synchronous exception, IRQ, FIQ, and SError.
    // Therefore, each exception level maps the four exception type handlers aligned by 128 bytes (enough room to write simple handlers).

    // Our strategy is to forward all interrupts to _int_entry via
    ASM("                                                                       \t\n\
                        .balign 128                                             \t\n\
        .sync_curr_sp0: str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #0                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .irq_curr_sp0:  str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #1                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
         .fiq_curr_sp0: str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #2                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
         .error_curr_sp0:str x30, [sp,#-8]!                                     \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #3                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .sync_curr_spx: str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #4                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .irq_curr_spx:  str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #5                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .fiq_curr_spx:  str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #6                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
         .error_curr_spx:str x30, [sp,#-8]!                                     \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #7                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .sync_lower64:  str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x28, [sp,#-8]!                                      \t\n\
                                                                                \t\n\
                        mrs x28, CurrentEL                                      \t\n\
                        and x28, x28, #12                                       \t\n\
                        lsr x28, x28, #2                                        \t\n\
                                                                                \t\n\
                        cmp x28, #2                                             \t\n\
                        beq .1                                                  \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        blr x29                                                 \t\n\
                        b .2                                                    \t\n\
                    .1:                                                         \t\n\
                        dsb   ishst                                             \t\n\
                        tlbi  alle1                                             \t\n\
                        dsb   ish                                               \t\n\
                       isb                                                      \t\n\
                    .2:                                                         \t\n\
                        ldr x28, [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                         .balign 128                                            \t\n\
        .irq_lower64:   str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #9                                              \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                         .balign 128                                            \t\n\
        .fiq_lower64:   str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #10                                             \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .error_lower64: str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #11                                             \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                         .balign 128                                            \t\n\
        .sync_lower32:  str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #12                                             \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .irq_lower32:   str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #13                                             \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .fiq_lower32:   str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #14                                             \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
                                                                                \t\n\
                        .balign 128                                             \t\n\
        .error_lower32: str x30, [sp,#-8]!                                      \t\n\
                        str x29, [sp,#-8]!                                      \t\n\
                        str x0,  [sp,#-8]!                                      \t\n\
                        ldr x29, .ic_entry                                      \t\n\
                        mov x0, #15                                             \t\n\
                        blr x29                                                 \t\n\
                        ldr x0,  [sp], #8                                       \t\n\
                        ldr x29, [sp], #8                                       \t\n\
                        ldr x30, [sp], #8                                       \t\n\
                        eret                                                    \t\n\
        .ic_entry: .dword 0x0                                                   \t");
 }

void _reset()
{
    if(CPU::id() == 0) {
        // Relocated the vector table, which has 4 entries for each of the 4 scenarios, all 128 bytes aligned, plus an 8 bytes pointer, totaling 2056 bytes
        CPU::Reg * src = reinterpret_cast<CPU::Reg *>(&_vector_table);
        CPU::Reg * dst = reinterpret_cast<CPU::Reg *>(Memory_Map::VECTOR_TABLE);
        for(int i = 0; i < (2056 / 8); i++)
            dst[i] = src[i];
        // Set el1 vbar
        CPU::vbar_el1(static_cast<CPU::Phy_Addr>(Memory_Map::VECTOR_TABLE));

        // Activate aarch64
        CPU::hcr(CPU::EL1_AARCH64_EN | CPU::SWIO_HARDWIRED);

        // We start at EL2, but must set EL1 SP for a smooth transition, including further exception/interrupt handling
        CPU::spsr_el2(CPU::FLAG_D | CPU::FLAG_A | CPU::FLAG_I | CPU::FLAG_F | CPU::FLAG_EL1 | CPU::FLAG_SP_ELn);
        CPU::Reg el1_addr = CPU::pc();
        el1_addr += 16; // previous instruction, this instruction, and the next one;
        CPU::elr_el2(el1_addr);
        CPU::eret();
        CPU::sp(Memory_Map::BOOT_STACK + Traits<Machine>::STACK_SIZE * (CPU::id() + 1)); // set stack

        // Clear the BSS (SETUP was linked to CRT0, but entry point didn't go through BSS clear)
        Machine::clear_bss();
    } else {
        // we want secondary cores to be held here.
        while(true)
            CPU::halt();
    }

    _setup();
}

#endif

void _setup()
{
    CPU::int_disable(); // interrupts will be re-enabled at init_end

    Setup setup;
}
