// EPOS LM3S811 (ARM Cortex-M3) Memory Map

#ifndef __lm3s811_memory_map_h
#define __lm3s811_memory_map_h

#include <machine/cortex/cortex_memory_map.h>

__BEGIN_SYS

struct Memory_Map: public Cortex_Memory_Map
{
    enum {
        // Base addresses for memory-mapped control and I/O devices
        WDT0_BASE                   = 0x40000000, // Watchdog Timer
        I2C0_BASE                   = 0x40002000, // I2C
        GPIOA_BASE                  = 0x40004000, // PrimeCell PL061 GPIO
        GPIOB_BASE                  = 0x40005000, // PrimeCell PL061 GPIO
        GPIOC_BASE                  = 0x40006000, // PrimeCell PL061 GPIO
        GPIOD_BASE                  = 0x40007000, // PrimeCell PL061 GPIO
        USART_BASE                  = 0x40008000, // PrimeCell PL022 Synchronous Serial Port
        UART0_BASE                  = 0x4000c000, // PrimeCell PL011 UART
        UART1_BASE                  = 0x4000d000, // PrimeCell PL011 UART
        I2C1_BASE                   = 0x40020000, // I2C
        I2C2_BASE                   = 0x40021000, // I2C
        GPIOE_BASE                  = 0x40024000, // PrimeCell PL061 GPIO
        PWM0_BASE                   = 0x40028000, // PWM
        TIMER0_BASE                 = 0x40030000, // GPTM
        TIMER1_BASE                 = 0x40031000, // GPTM
        TSC_BASE                    = TIMER1_BASE,
        TIMER2_BASE                 = 0x40032000, // GPTM
        ADC0_BASE                   = 0x40038000, // ADC
        FLASH0_BASE                 = 0x400fd000, // Flash Controller
        SCR_BASE                    = 0x400fe000, // System Control
        SCB_BASE                    = 0xe000e000, // System Control Block
        VECTOR_TABLE                = Traits<Machine>::APP_CODE,

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
