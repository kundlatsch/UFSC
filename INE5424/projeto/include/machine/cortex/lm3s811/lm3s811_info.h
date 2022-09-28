// EPOS LM3S811 (ARM Cortex-M3) Run-Time System Information

#ifndef __lm3s811_info_h
#define __lm3s811_info_h

#include <system/info.h>

__BEGIN_SYS

struct System_Info: public System_Info_Common
{
    Boot_Map bm;
    Library_Load_Map lm;
};

__END_SYS

#endif
