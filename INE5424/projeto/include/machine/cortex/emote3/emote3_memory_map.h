// EPOS EPOSMote III (ARM Cortex-M3) Memory Map

#ifndef __emote3_memory_map_h
#define __emote3_memory_map_h

#include <machine/cortex/cortex_memory_map.h>

__BEGIN_SYS

struct Memory_Map: public Cortex_Memory_Map
{
    enum {
        // Base addresses for memory-mapped control and I/O devices
        SSI0_BASE       = 0x40008000,
        SSI1_BASE       = 0x40009000,
        UART0_BASE      = 0x4000c000, // PL011 UART
        UART1_BASE      = 0x4000d000, // PL011 UART
        I2C_MASTER_BASE = 0x40020000,
        I2C_SLAVE_BASE  = 0x40028000,
        TIMER0_BASE     = 0x40030000, // GPTM
        TIMER1_BASE     = 0x40031000, // GPTM
        TIMER2_BASE     = 0x40032000, // GPTM
        TIMER3_BASE     = 0x40033000, // GPTM
        TSC_BASE        = TIMER3_BASE,
        RF_BASE         = 0x40088000,
        USB0_BASE       = 0x40089000,
        SCR_BASE        = 0x400d2000,
        FLASH_BASE      = 0x400d3000,
        IOC_BASE        = 0x400d4000,
        WATCHDOG_BASE   = 0x400d5000,
        ADC_BASE        = 0x400d7000,
        GPIOA_BASE      = 0x400d9000, // PL061 GPIO Port A
        GPIOB_BASE      = 0x400da000, // PL061 GPIO Port B
        GPIOC_BASE      = 0x400db000, // PL061 GPIO Port C
        GPIOD_BASE      = 0x400dc000, // PL061 GPIO Port D
        CCTEST_BASE     = 0x44010000,
        SCB_BASE        = 0xe000e000, // System Control Block
        VECTOR_TABLE    = Traits<Machine>::APP_CODE,

        // Logical Address Space
        BOOT_STACK      = Traits<Machine>::BOOT_STACK,

        APP_LOW         = Traits<Machine>::APP_LOW,
        APP_HIGH        = Traits<Machine>::APP_HIGH,
        APP_CODE        = Traits<Machine>::APP_CODE,
        APP_DATA        = Traits<Machine>::APP_DATA,

        SYS_CODE        = NOT_USED,
        SYS_INFO        = NOT_USED,
        SYS_DATA        = NOT_USED,
        SYS_STACK       = NOT_USED,
        SYS_HEAP        = NOT_USED
    };
};

__END_SYS

#endif
