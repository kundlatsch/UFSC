// EPOS SiFive-U (RISC-V) Memory Map

#ifndef __riscv_sifive_u_memory_map_h
#define __riscv_sifive_u_memory_map_h


#include <system/memory_map.h>

__BEGIN_SYS

struct Memory_Map
{
    enum {
        NOT_USED        = Traits<Machine>::NOT_USED,

        // Physical Memory
        RAM_BASE        = Traits<Machine>::RAM_BASE,
        RAM_TOP         = Traits<Machine>::RAM_TOP,
        MIO_BASE        = Traits<Machine>::MIO_BASE,
        MIO_TOP         = Traits<Machine>::MIO_TOP,
        BOOT_STACK      = RAM_BASE + 512 * 1024 - sizeof(long), // RAM_BASE + 512 KB - 4 (will be used as the stack pointer, not the base)

        // Memory-mapped devices
        TEST_BASE       = 0x00100000, // SiFive test engine
        RTC_BASE        = 0x00101000, // Goldfish RTC
        UART_BASE       = 0x10000000, // NS16550A UART
        CLINT_BASE      = 0x02000000, // SiFive CLINT
        TIMER_BASE      = 0x02004000, // CLINT Timer
        PLIIC_CPU_BASE  = 0x0c000000, // SiFive PLIC

        // Logical Address Space
        BOOT            = Traits<System>::multitask ? Traits<Machine>::BOOT : NOT_USED,
        IMAGE           = Traits<Machine>::IMAGE,
        SETUP           = Traits<System>::multitask ? Traits<Machine>::SETUP : NOT_USED,
        INIT            = Traits<System>::multitask ? Traits<Machine>::INIT : NOT_USED,

        APP_LOW         = Traits<System>::multitask ? Traits<Machine>::APP_LOW : Traits<Machine>::SETUP,
        APP_CODE        = APP_LOW,
        APP_DATA        = Traits<System>::multitask ? APP_LOW + 4 * 1024 * 1024 : APP_LOW,
        APP_HIGH        = Traits<Machine>::APP_HIGH,

        PHY_MEM         = Traits<Machine>::PHY_MEM,
        IO              = Traits<Machine>::IO,

        SYS             = Traits<Machine>::SYS,
        SYS_CODE        = Traits<System>::multitask ? SYS + 0x00000000 : NOT_USED,
        SYS_INFO        = Traits<System>::multitask ? SYS + 0x00100000 : NOT_USED,
        SYS_PT          = Traits<System>::multitask ? SYS + 0x00101000 : NOT_USED,
        SYS_PD          = Traits<System>::multitask ? SYS + 0x00102000 : NOT_USED,
        SYS_DATA        = Traits<System>::multitask ? SYS + 0x00103000 : NOT_USED,
        SYS_STACK       = Traits<System>::multitask ? SYS + 0x00200000 : NOT_USED,
        SYS_HEAP        = Traits<System>::multitask ? SYS + 0x00400000 : NOT_USED
    };
};

__END_SYS

#endif
