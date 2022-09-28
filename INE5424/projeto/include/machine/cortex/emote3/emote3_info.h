// EPOS EPOSMote III (ARM Cortex-M3) Run-Time System Information

#ifndef __emote3_info_h
#define __emote3_info_h

#include <system/info.h>

__BEGIN_SYS

struct System_Info: public System_Info_Common
{
    Boot_Map bm;
    Library_Load_Map lm;
};

__END_SYS

#endif
