// EPOS ARM Cortex Common Memory Map

#ifndef __cortex_memory_map_h
#define __cortex_memory_map_h

#include <system/memory_map.h>

__BEGIN_SYS

struct Cortex_Memory_Map
{
    static const unsigned int NOT_USED = Traits<Machine>::NOT_USED;

    enum {
        // Physical Memory
        RAM_BASE        = Traits<Machine>::RAM_BASE,
        RAM_TOP         = Traits<Machine>::RAM_TOP,
        MIO_BASE        = Traits<Machine>::MIO_BASE,
        MIO_TOP         = Traits<Machine>::MIO_TOP,

        // Physical Memory at Boot
        BOOT            = Traits<Machine>::BOOT,
        SETUP           = Traits<Machine>::SETUP,
        IMAGE           = Traits<Machine>::IMAGE,

        // Logical Address Space
        INIT            = Traits<Machine>::INIT,
        PHY_MEM         = Traits<Machine>::PHY_MEM,
        IO              = Traits<Machine>::IO,
        SYS             = Traits<Machine>::SYS
    };
};

__END_SYS

#endif
