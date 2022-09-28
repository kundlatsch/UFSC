// EPOS EPOSMoteIII (ARM Cortex-M3) Mediator Declarations

#ifndef __emote3_machine_h
#define __emote3_machine_h

#include <machine/machine.h>
#include <machine/cortex/engine/cortex_m3/scb.h>
#include <machine/cortex/engine/cortex_m3/systick.h>
#include "emote3_sysctrl.h"
#include "emote3_ioctrl.h"
#include <system/memory_map.h>
#include <system.h>
#include <utility/convert.h>

__BEGIN_SYS

class eMote3: private Machine_Common
{
    friend Machine;

private:
    typedef CPU::Reg32 Reg32;
    typedef CPU::Log_Addr Log_Addr;

    // Hooks to functions implemented in ROM (documented in the ROM User Guide, swru3333)
    enum {
        ROM_API_BASE    = 0x00000048,
    };

    enum {
        ROM_API_CRC32      = ROM_API_BASE + 0,
        ROM_API_FLASH_SIZE = ROM_API_BASE + 4,
        ROM_API_CHIP_ID    = ROM_API_BASE + 8,
        ROM_API_PAGE_ERASE = ROM_API_BASE + 12,
        ROM_API_PROG_FLASH = ROM_API_BASE + 16,
        ROM_API_REBOOT     = ROM_API_BASE + 20,
        ROM_API_MEMSET     = ROM_API_BASE + 24,
        ROM_API_MEMCPY     = ROM_API_BASE + 28,
        ROM_API_MEMCMP     = ROM_API_BASE + 32,
        ROM_API_MEMMOV     = ROM_API_BASE + 36,
    };

    // Unique IEEE Address at the flash Info Page
    enum {
        IEEE_ADDR       = 0x00280028,
    };

public:
    eMote3() {}

    using Machine_Common::delay;
    using Machine_Common::clear_bss;

    static void reboot() {
        // call ROM function to reboot
        typedef void (* volatile Reboot) (void);
        Reboot rom_function_reset = *reinterpret_cast<Reboot*>(ROM_API_REBOOT);
        rom_function_reset();
    }
    static void poweroff() { reboot(); }

    static const UUID & uuid() { return *reinterpret_cast<const UUID *>(IEEE_ADDR); }
    
    static void smp_barrier() {}

    static void smp_barrier_init(unsigned int n_cpus) { assert(n_cpus == 1); }

    static void power(const Power_Mode & mode) {
        // Change in power mode will only be effective when ASM("wfi") is executed
        switch(mode) {
        case ENROLL:
                break;
        case DISMISS:
                break;
        case SAME:
                break;
        case FULL: // Active Mode
        case LIGHT: // Sleep Mode
            scb()->unsleepdeep();
            break;
        case SLEEP: // Deep Sleep Power Mode 0
            scb()->sleepdeep();
            scr()->power_mode(SysCtrl::ACTIVE);
            break;
        case OFF: // Deep Sleep Power Mode 3
            scb()->sleepdeep();
            scr()->power_mode(SysCtrl::POWER_MODE_1);
            break;
       }
    }

private:
    static void pre_init();
    static void init();

private:
    static SCB * scb() { return reinterpret_cast<SCB *>(Memory_Map::SCB_BASE); }
    static SysCtrl * scr() { return reinterpret_cast<SysCtrl *>(Memory_Map::SCR_BASE); }
    static IOCtrl * ioc() { return reinterpret_cast<IOCtrl *>(Memory_Map::IOC_BASE); }
};

typedef eMote3 Machine_Model;

__END_SYS

#endif
