// EPOS Raspberry Pi3 (ARM Cortex-A53) Run-Time System Information

#ifndef __raspberry_pi3_info_h
#define __raspberry_pi3_info_h

#include <system/info.h>

__BEGIN_SYS

// Raspberry Pi 3's SETUP always uses a kernel System Info because it arranges for a flat memory mapping using the MMU even for single-task configurations
struct System_Info: public System_Info_Common
{
    Boot_Map bm;
    Physical_Memory_Map pmm;
    Kernel_Load_Map lm;
};

__END_SYS

#endif
