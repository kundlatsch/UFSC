// EPOS SiFive-U (RISC-V) Run-Time System Information

#ifndef __riscv_sifive_u_info_h
#define __riscv_sifive_u_info_h

#include <system/info.h>

__BEGIN_SYS

struct System_Info: public System_Info_Common
{
    Boot_Map bm;
    Physical_Memory_Map pmm;
    Kernel_Load_Map lm;
};

__END_SYS

#endif
