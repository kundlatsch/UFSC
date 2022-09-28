// EPOS PC Run-Time System Information

#ifndef __pc_info_h
#define __pc_info_h

#include <system/info.h>

__BEGIN_SYS

struct System_Info: public System_Info_Common
{
public:
    struct Physical_Memory_Map: public System_Info_Common::Physical_Memory_Map
    {
        PAddr idt;              // IDT
        PAddr gdt;              // GDT
        PAddr tss;              // TSSs (only for system call; 1 per CPU)
    };
    
    struct Time_Map
    {
        unsigned int cpu_clock;
        unsigned int bus_clock;
    };

public:
    Boot_Map bm;
    Physical_Memory_Map pmm;
    Kernel_Load_Map lm;
    Time_Map tm;
    volatile int cpu_status[Traits<Machine>::CPUS]; // CPUs initialization status
};

__END_SYS

#endif
