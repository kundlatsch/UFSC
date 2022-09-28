// EPOS Realview PBX (ARM Cortex-A9) Memory Map

#ifndef __model_memory_map_h
#define __model_memory_map_h

#include <machine/cortex/cortex_memory_map.h>

__BEGIN_SYS

struct Memory_Map: public Cortex_Memory_Map
{
    enum {
        // Base addresses for memory-mapped control and I/O devices
        SYS_FLAGS               = 0x10000030, // OS GP flags (used by QEMU to indicate the starting address of non-BSP cores)
        SYS_NVFLAGS             = 0x10000038, // OS GP flags (not cleared on reset)
        I2C_BASE                = 0x10002000, // Versatile I2C
        AAC_BASE                = 0x10004000, // PrimeCell PL041 Advanced Audio CODEC
        MMCI_BASE               = 0x10005000, // PrimeCell PL181 MultiMedia Card Interface
        KBD0_BASE               = 0x10006000, // PrimeCell PL050 PS2 Keyboard/Mouse Interface
        KBD1_BASE               = 0x10007000, // PrimeCell PL050 PS2 Keyboard/Mouse Interface
        UART0_BASE              = 0x10009000, // PrimeCell PL011 UART
        UART1_BASE              = 0x1000a000, // PrimeCell PL011 UART
        UART2_BASE              = 0x1000b000, // PrimeCell PL011 UART
        UART3_BASE              = 0x1000c000, // PrimeCell PL011 UART
        TIMER0_BASE             = 0x10011000, // ARM Dual-Timer Module SP804
        TIMER1_BASE             = 0x10012000, // ARM Dual-Timer Module SP804
        GPIOA_BASE              = 0x10013000, // PrimeCell PL061 GPIO
        GPIOB_BASE              = 0x10014000, // PrimeCell PL061 GPIO
        GPIOC_BASE              = 0x10015000, // PrimeCell PL061 GPIO
        RTC_BASE                = 0x10017000, // PrimeCell PL031 RTC
        LCD_BASE                = 0x10020000, // PrimeCell PL110 Color LCD Controller
        DMA_BASE                = 0x10030000, // PrimeCell PL080 DMA Controller

        PPS_BASE                = 0x1f000000, // A9 Private Peripheral Space
        SCU_BASE                = 0x1f000000, // MP Snoop Control Unit
        GIC_CPU_BASE            = 0x1f000100,
        GLOBAL_TIMER_BASE       = 0x1f000200,
        TSC_BASE                = GLOBAL_TIMER_BASE,
        PRIVATE_TIMER_BASE      = 0x1f000600,
        GIC_DIST_BASE           = 0x1f001000,

        VECTOR_TABLE            = RAM_BASE, // 8 x 4b instructions + 8 x 4b pointers
        BOOT_STACK              = VECTOR_TABLE + 4 * 1024, // will be used as the stack's base, not the stack pointer; SIZE = Traits<Build>::CPUS * Traits<Machine>::STACK_SIZE
        FLAT_PAGE_TABLE 	= Traits<System>::multitask ? NOT_USED : (RAM_TOP - 16 * 1024) & ~(0x3fff),  // used only with No_MMU in LIBRARY mode; 16KB, 4096 4B entries, each pointing to 1 MB regions, thus mapping up to 4 GB; 16K-aligned for TTBR;

        // Logical Address Space
        APP_LOW                 = Traits<Machine>::RESET,
        APP_HIGH                = BOOT_STACK,

        APP_CODE                = APP_LOW,
        APP_DATA                = APP_LOW,

        PHY_MEM                 = Traits<Machine>::PHY_MEM,
        IO                      = Traits<Machine>::IO,
        SYS                     = Traits<Machine>::SYS,
        SYS_CODE                = Traits<System>::multitask ? SYS + 0x00000000 : NOT_USED,
        SYS_INFO                = Traits<System>::multitask ? SYS + 0x00100000 : NOT_USED,
        SYS_PT                  = Traits<System>::multitask ? SYS + 0x00101000 : NOT_USED, // 4KB = 256 + 256 + 256 entries to map from SYS to SYS_STACK
        SYS_PD                  = Traits<System>::multitask ? SYS + 0x00102000 : NOT_USED, // 16KB mem == 4k PD entries
        SYS_DATA                = Traits<System>::multitask ? SYS + 0x00106000 : NOT_USED,
        SYS_STACK               = Traits<System>::multitask ? SYS + 0x00200000 : NOT_USED, // 16KB mem == STACK_SIZE
        SYS_HEAP                = Traits<System>::multitask ? SYS + 0x00300000 : NOT_USED
    };
};

__END_SYS

#endif
