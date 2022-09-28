// EPOS Cortex-M3 System Control Block Module Mediator Declarations

#ifndef __cortex_m3_scb_h
#define __cortex_m3_scb_h

#include <architecture/cpu.h>
#include <system/memory_map.h>

__BEGIN_SYS

class SCB
{
    // This is a hardware object.
    // Use with something like "new (Memory_Map::SCB_BASE) SCB".

private:
    typedef CPU::Reg32 Reg32;

public:
    // Registers' offsets in System Control Space
    enum {                              // Description                                          Type    Value after reset
        MCR             = 0x000,        // Master Control Register                              -       0x00000000
        ICTR            = 0x004,        // Interrupt Controller Type Register                   RO      0x????????
        ACTLR           = 0x008,        // Auxiliary Control Register                           rw      0x????????
        STCTRL          = 0x010,        // SysTick Control and Status Register                  R/W     0x00000000
        STRELOAD        = 0x014,        // SysTick Reload Value Register                        R/W     0x00000000
        STCURRENT       = 0x018,        // SysTick Current Value Register                       R/WC    0x00000000
        IRQ_ENABLE0     = 0x100,        // Interrupt  0-31 Set Enable                           R/W     0x00000000
        IRQ_ENABLE1     = 0x104,        // Interrupt 32-63 Set Enable                           R/W     0x00000000
        IRQ_ENABLE2     = 0x108,        // Interrupt 64-95 Set Enable                           R/W     0x00000000
        IRQ_DISABLE0    = 0x180,        // Interrupt  0-31 Clear Enable                         R/W     0x00000000
        IRQ_DISABLE1    = 0x184,        // Interrupt 32-63 Clear Enable                         R/W     0x00000000
        IRQ_DISABLE2    = 0x188,        // Interrupt 64-95 Clear Enable                         R/W     0x00000000
        IRQ_PEND0       = 0x200,        // Interrupt  0-31 Set Pending                          R/W     0x00000000
        IRQ_PEND1       = 0x204,        // Interrupt 32-63 Set Pending                          R/W     0x00000000
        IRQ_PEND2       = 0x208,        // Interrupt 64-95 Set Pending                          R/W     0x00000000
        IRQ_UNPEND0     = 0x280,        // Interrupt  0-31 Clear Pending                        R/W     0x00000000
        IRQ_UNPEND1     = 0x284,        // Interrupt 32-63 Clear Pending                        R/W     0x00000000
        IRQ_UNPEND2     = 0x288,        // Interrupt 64-95 Clear Pending                        R/W     0x00000000
        IRQ_ACTIVE0     = 0x300,        // Interrupt  0-31 Active Bit                           R/W     0x00000000
        IRQ_ACTIVE1     = 0x304,        // Interrupt 32-63 Active Bit                           R/W     0x00000000
        IRQ_ACTIVE2     = 0x308,        // Interrupt 64-95 Active Bit                           R/W     0x00000000
        CPUID           = 0xd00,        // CPUID Base Register                                  ro      0x410fc231
        INTCTRL         = 0xd04,        // Interrupt Control and State Register                 rw      0x00000000
        VTOR            = 0xd08,        // Vector Table Offset Register                         rw      0x00000000
        AIRCR           = 0xd0c,        // Application Interrupt/Reset Control Register         rw
        SCR             = 0xd10,        // System Control Register                              rw      0x00000000
        CCR             = 0xd14,        // Configuration Control Register                       rw      0x00000000
        SHPR1           = 0xd18,        // System Handlers 4-7 Priority                         rw      0x00000000
        SHPR2           = 0xd1c,        // System Handlers 8-11 Priority                        rw      0x00000000
        SHPR3           = 0xd20,        // System Handlers 12-15 Priority                       rw      0x00000000
        SHCSR           = 0xd24,        // System Handler Control and State Register            rw      0x00000000
        CFSR            = 0xd28,        // Configurable Fault Status Register                   rw      0x00000000
        HFSR            = 0xd2c,        // Hard Fault Status Register
        SWTRIG          = 0xf00         // Software Trigger Interrupt Register                  WO      0x00000000
    };

    // Useful Bits in the Interrupt Control and State Register (INTCTRL)
    enum {                              // Description                                          Type    Value after reset
        ICSR_ACTIVE     = 1 <<  0,      // Active exceptions (IPSR mirror, 0 -> thread mode)    ro
        ICSR_PENDING    = 1 << 12,      // Pending exceptions (0 -> none)                       ro
        ICSR_ISRPENDING = 1 << 22,      // Pending NVIC IRQ                                     ro
        ICSR_SYSPENDING = 1 << 25       // Clear pending SysTick                                wo
    };

    // Useful Bits in the Application Interrupt/Reset Control Register (AIRCR)
    enum {                              // Description                                          Type    Value after reset
        VECTRESET       = 1 << 0,       // Reserved for debug                                   wo      0
        VECTCLRACT      = 1 << 1,       // Reserved for debug                                   wo      0
        SYSRESREQ       = 1 << 2,       // System Reset Request                                 wo      0
        VECTKEY         = 1 << 16,      // Register Key                                         rw      0xfa05
                                        // This field is used to guard against accidental
                                        // writes to this register.  0x05FA must be written
                                        // to this field in order to change the bits in this
                                        // register. On a read, 0xFA05 is returned.
    };

    // Useful Bits in the System Control Register (SCR)
    enum {                              // Description                                          Type    Value after reset
        SLEEPDEEP = 1 << 2,             // Makes the chip will enter power modes in accordance
                                        // to SysCtrl(PMCTL) when the operational mode initiator
                                        // (WFI instruction) is asserted.
    };

    // Useful Bits in the Configuration Control Register (CCR)
    enum {                              // Description                                          Type    Value after reset
        BASETHR         = 1 <<  0,      // Thread state can be entered at any level of int.     rw      0
        MAINPEND        = 1 <<  1,      // SWTRIG can be written to in user mode                rw      0
        UNALIGNED       = 1 <<  3,      // Trap on unaligned memory access                      rw      0
        DIV0            = 1 <<  4,      // Trap on division by zero                             rw      0
        BFHFNMIGN       = 1 <<  8,      // Ignore Precise Data Access Faults for pri -1 and -2  rw      0
        STKALIGN        = 1 <<  9       // Align stack point on exception entry to 8 butes      rw      0
    };

public:
    void reboot() {
        Reg32 val = scs(AIRCR) & (~((-1u / VECTKEY) * VECTKEY));
        val |= 0x05fa * VECTKEY | SYSRESREQ;
        scs(AIRCR) = val;
    }

    void sleepdeep() {
        scs(SCR) |= SLEEPDEEP;
    }

    void unsleepdeep() {
        scs(SCR) &= ~SLEEPDEEP;
    }

    void enter_thread_state() { scs(CCR) |= BASETHR; }

    void vector_table_offset() { scs(VTOR) = (Memory_Map::VECTOR_TABLE) & ~(1 << 29); }  // Must be 512-byte aligned!

private:
    volatile Reg32 & scs(unsigned int o) { return reinterpret_cast<volatile Reg32 *>(this)[o / sizeof(Reg32)]; }
};

__END_SYS

#endif
