// EPOS Zynq-7000 (Cortex-A9) Run-Time System Information

#ifndef __zynq_info_h
#define __zynq_info_h

#include <system/info.h>

__BEGIN_SYS

struct System_Info: public System_Info_Common
{
    Boot_Map bm;
    Library_Load_Map lm;
};

__END_SYS

#endif
