// EPOS Raspberry Pi3 (ARM Cortex-A53) Memory Map

#ifndef __raspberry_pi3_memory_map_h
#define __raspberry_pi3_memory_map_h

#include <machine/cortex/cortex_memory_map.h>

__BEGIN_SYS

static const unsigned long MIO_OFFSET = Traits<System>::multitask ? (Traits<Machine>::IO - Traits<Machine>::MIO_BASE) : 0;
static constexpr unsigned long adjust_mio(unsigned long addr) { return addr + MIO_OFFSET; }
static constexpr unsigned long no_mio(unsigned long addr) { return addr - MIO_OFFSET; }

struct Memory_Map: public Cortex_Memory_Map
{
private:
    static const bool armv7     = (Traits<Build>::ARCHITECTURE == Traits<Build>::ARMv7);
    static const bool multitask = Traits<System>::multitask;
    static const bool emulated  = Traits<Build>::EXPECTED_SIMULATION_TIME;

public:
    enum : unsigned long {
        // Base addresses for memory-mapped control and I/O devices
        MBOX_COM_BASE   = adjust_mio(0x3ef00000), // RAM memory for device-os communication (must be mapped as device by the MMU)
        MBOX_CTRL_BASE  = adjust_mio(0x40000000), // BCM MailBox
        PPS_BASE        = adjust_mio(0x3f000000), // Private Peripheral Space
        TSC_BASE        = adjust_mio(0x3f003000),
        TIMER0_BASE     = emulated ? MBOX_CTRL_BASE : adjust_mio(0x3f003000), // System Timer (free running)
        DMA0_BASE       = adjust_mio(0x3f007000),
        IC_BASE         = adjust_mio(0x3f00b200),
        TIMER1_BASE     = adjust_mio(0x3f00b400), // ARM Timer (frequency relative to processor frequency)
        MBOX_BASE       = adjust_mio(0x3f00b800),
        ARM_MBOX0       = adjust_mio(0x3f00b880), // IOCtrl (MBOX 0) is also mapped on this address
        PM_BASE         = adjust_mio(0x3f100000), // Power Manager
        RAND_BASE       = adjust_mio(0x3f104000),
        GPIO_BASE       = adjust_mio(0x3f200000),
        UART_BASE       = adjust_mio(0x3f201000), // PrimeCell PL011 UART
        SD0_BASE        = adjust_mio(0x3f202000), // Custom sdhci controller
        AUX_BASE        = adjust_mio(0x3f215000), // mini UART + 2 x SPI master
        SD1_BASE        = adjust_mio(0x3f300000), // Arasan sdhci controller
        DMA1_BASE       = adjust_mio(0x3fe05000),
        VECTOR_TABLE    = RAM_BASE,
        BOOT_STACK      = VECTOR_TABLE + 4 * 1024, // will be used as the stack's base, not the stack pointer; SIZE = Traits<Build>::CPUS * Traits<Machine>::STACK_SIZE
        FLAT_PAGE_TABLE = multitask ? NOT_USED : (RAM_TOP - 16 * 1024) & ~(0x3fff),  // used only with No_MMU in LIBRARY mode; 32-bit: 16KB, 4096 4B entries, each pointing to 1 MB regions, thus mapping up to 4 GB; 64-bit: 16KB, 2048 8B entries, each pointing to 32 MB regions, thus mapping up to 64 GB; 16K-aligned for TTBR; 

        // Logical Address Space -- Need to be verified
        APP_LOW         = Traits<Machine>::APP_LOW,
        APP_HIGH        = Traits<Machine>::APP_HIGH,

        APP_CODE        = Traits<Machine>::APP_CODE,
        APP_DATA        = Traits<Machine>::APP_DATA,

        PHY_MEM         = Traits<Machine>::PHY_MEM,
        IO              = Traits<Machine>::IO,
        SYS             = Traits<Machine>::SYS,
        SYS_CODE        = multitask ? SYS + 0x00000000 : NOT_USED,
        SYS_INFO        = multitask ? SYS + (armv7 ? 0x00100000 : 0x02000000) : NOT_USED,
        SYS_PT          = multitask ? SYS + (armv7 ? 0x00101000 : 0x02004000) : NOT_USED, // 4KB = 256 + 256 + 256 entries to map from SYS to SYS_STACK | 16KB = 2048 (they are 512) entries to map from SYS to SYS_STACK
        SYS_PD          = multitask ? SYS + (armv7 ? 0x00102000 : 0x02008000) : NOT_USED, // 16KB mem == 4k PD entries | 128 PDs * 8bytes = 1KB to map 4 GB -> align to 16KB
        SYS_DATA        = multitask ? SYS + (armv7 ? 0x00106000 : 0x0200c000) : NOT_USED,
        SYS_STACK       = multitask ? SYS + (armv7 ? 0x00200000 : 0x04000000) : NOT_USED, // 16KB mem == STACK_SIZE
        SYS_HEAP        = multitask ? SYS + (armv7 ? 0x00300000 : 0x06000000) : NOT_USED,
        SYS_HIGH        = multitask ? SYS + (armv7 ? 0x003fffff : 0x07ffffff) : NOT_USED
    };
};

__END_SYS

#endif
