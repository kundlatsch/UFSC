// EPOS PCI Mediator Common Package

#ifndef __pci_h
#define __pci_h

#include <architecture/cpu.h>

__BEGIN_SYS

class PCI_Common
{
protected:
    PCI_Common() {}

protected:
     typedef CPU::Reg8 Reg8;
     typedef CPU::Reg16 Reg16;
     typedef CPU::Reg32 Reg32;
     typedef CPU::Log_Addr Log_Addr;
     typedef CPU::Phy_Addr Phy_Addr;

public:
    // PCI config space register offsets for normal devices (type 0)
    enum {
        VENDOR_ID		= 0x00, // 16 bits
        DEVICE_ID		= 0x02, // 16 bits
        COMMAND			= 0x04, // 16 bits
        STATUS			= 0x06, // 16 bits
        REVISION_ID		= 0x08, //  8 bits
        CLASS_PROG		= 0x09, //  8 bits
        CLASS_ID		= 0x0a, // 16 bits
        CACHE_LINE_SIZE		= 0x0c, //  8 bits
        LATENCY_TIMER		= 0x0d, //  8 bits
        HEADER_TYPE		= 0x0e, //  8 bits
        BIST			= 0x0f, //  8 bits
        BASE_ADDRESS_0		= 0x10, // 32 bits
        BASE_ADDRESS_1		= 0x14, // 32 bits
        BASE_ADDRESS_2		= 0x18, // 32 bits
        BASE_ADDRESS_3		= 0x1c, // 32 bits
        BASE_ADDRESS_4		= 0x20, // 32 bits
        BASE_ADDRESS_5		= 0x24, // 32 bits
        CARDBUS_CIS		= 0x28, // 32 bits
        SUBSYSTEM_VENDOR_ID	= 0x2c, // 16 bits
        SUBSYSTEM_DEVICE_ID	= 0x2e, // 16 bits
        ROM_ADDRESS		= 0x30, // [11:31] address, [1:10] reserved
        				// 0x34-0x3b are reserved	
        INTERRUPT_LINE		= 0x3c, //  8 bits
        INTERRUPT_PIN		= 0x3d, //  8 bits
        MIN_GNT			= 0x3e, //  8 bits
        MAX_LAT			= 0x3f, //  8 bits
    };

    // PCI COMMAND register
    enum {
        COMMAND_IO		= 0x1,   // Enable response in I/O space
        COMMAND_MEMORY		= 0x2,   // Enable response in Memory space
        COMMAND_MASTER		= 0x4,   // Enable bus mastering
        COMMAND_SPECIAL		= 0x8,   // Enable response to special cycles
        COMMAND_INVALIDATE	= 0x10,  // Use memory write and invalidate
        COMMAND_VGA_PALETTE	= 0x20,  // Enable palette snooping
        COMMAND_PARITY		= 0x40,  // Enable parity checking
        COMMAND_WAIT		= 0x80,  // Enable address/data stepping
        COMMAND_SERR		= 0x100, // Enable SERR
        COMMAND_FAST_BACK	= 0x200, // Enable back-to-back writes
    };

    // PCI STATUS register
    enum {
        STATUS_CAP_LIST		= 0x10,   // Support Capability List
        STATUS_66MHZ		= 0x20,   // Support 66 Mhz PCI 2.1 bus
        STATUS_UDF		= 0x40,   // Support User Features (obsolete)
        STATUS_FAST_BACK	= 0x80,   // Accept fast-back to back
        STATUS_PARITY		= 0x100,  // Detected parity error
        STATUS_DEVSEL_MASK	= 0x600,  // DEVSEL timing
        STATUS_DEVSEL_FAST	= 0x000,
        STATUS_DEVSEL_MEDIUM	= 0x200,
        STATUS_DEVSEL_SLOW	= 0x400,
        STATUS_SIG_TARGET_ABORT	= 0x800,  // Set on target abort
        STATUS_REC_TARGET_ABORT	= 0x1000, // Master ack of "
        STATUS_REC_MASTER_ABORT	= 0x2000, // Set on master abort
        STATUS_SIG_SYSTEM_ERROR	= 0x4000, // Set when we drive SERR
        STATUS_DETECTED_PARITY	= 0x8000, // Set on parity error
    };

    // PCI HEADER types
    enum {
        HEADER_TYPE_NORMAL	= 0,
        HEADER_TYPE_BRIDGE	= 1,
        HEADER_TYPE_CARDBUS	= 2,
    };

    // PCI BIST register
    enum {
        BIST_CODE_MASK	= 0x0f, // Return result
        BIST_START	= 0x40, // 1 to start BIST, 2 secs or less
        BIST_CAPABLE	= 0x80, // 1 if BIST capable
    };

    // PCI REGION types, address and size masks
    enum PCI_Masks {  // GCC BUG (anonymous enum in templates)
        BASE_ADDRESS_SPACE_MASK		= 0x01, // 0 = memory, 1 = I/O
        BASE_ADDRESS_SPACE_MEM		= 0x00,
        BASE_ADDRESS_SPACE_IO		= 0x01,
        BASE_ADDRESS_MEM_TYPE_MASK	= 0x06,
        BASE_ADDRESS_MEM_TYPE_32	= 0x00, // 32 bit address
        BASE_ADDRESS_MEM_TYPE_1M	= 0x02, // Below 1M [obsolete]
        BASE_ADDRESS_MEM_TYPE_64	= 0x04, // 64 bit address
        BASE_ADDRESS_MEM_PREFETCH	= 0x08, // prefetchable?
        BASE_ADDRESS_MEM_MASK		= ~0x0fUL,
        BASE_ADDRESS_IO_MASK		= ~0x03UL
    };

    // PCI ROM mask
    enum {
        ROM_ADDRESS_MASK	= ~0x7ffUL
    };

    // PCI config space register offsets for PCI-to-PCI bridges (type 1)
    enum {
        PRIMARY_BUS		= 0x18, // Primary bus number
        SECONDARY_BUS		= 0x19, // Secondary bus number
        SUBORDINATE_BUS		= 0x1a, // Highest bus number behind bridge
        SEC_LATENCY_TIMER	= 0x1b, // Latency timer for secondary int.
        IO_BASE			= 0x1c, // I/O range behind the bridge
        IO_LIMIT		= 0x1d,
        SEC_STATUS		= 0x1e, // Secondary status register, (bit 14)
        MEMORY_BASE		= 0x20, // Memory range behind
        MEMORY_LIMIT		= 0x22,
        PREF_MEMORY_BASE	= 0x24, // Prefetchable memory range behind
        PREF_MEMORY_LIMIT	= 0x26,
        PREF_BASE_UPPER32	= 0x28, // Upper half of pref. mem. range
        PREF_LIMIT_UPPER32	= 0x2c,
        IO_BASE_UPPER16		= 0x30, // Upper half of I/O addresses
        IO_LIMIT_UPPER16	= 0x32,
        				// 0x34 same as for htype 0
        				// 0x35-0x3b is reserved
        ROM_ADDRESS1		= 0x38, // PCI_ROM_ADDRESS for htype 1
        				// 0x3c-0x3d are same as for htype 0
        BRIDGE_CONTROL		= 0x3e
    };

    // PCI BRIDGE CONTROL register
    enum {
        BRIDGE_CTL_PARITY	= 0x01, // Parity on secondary interface
        BRIDGE_CTL_SERR		= 0x02, // The same for SERR forwarding
        BRIDGE_CTL_NO_ISA	= 0x04, // Disable bridging of ISA ports
        BRIDGE_CTL_VGA		= 0x08, // Forward VGA addresses
        BRIDGE_CTL_MASTER_ABORT	= 0x20, // Report master aborts
        BRIDGE_CTL_BUS_RESET	= 0x40, // Secondary bus reset
        BRIDGE_CTL_FAST_BACK	= 0x80  // Fast Back2Back on secondary int.
    };

    // PCI bridging type
    enum {
        IO_RANGE_TYPE_MASK	= 0x0fUL, // I/O bridging type
        IO_RANGE_TYPE_16	= 0x00,
        IO_RANGE_TYPE_32	= 0x01,
        IO_RANGE_MASK		= ~0x0fUL
    };

    // PCI memory behing bridge type and mask
    enum {
        MEMORY_RANGE_TYPE_MASK	= 0x0fUL,
        MEMORY_RANGE_MASK	= ~0x0fUL
    };

    // PCI prefetchable memory behing bridge type and mask
    enum {
        PREF_RANGE_TYPE_MASK	= 0x0fUL,
        PREF_RANGE_TYPE_32	= 0x00,
        PREF_RANGE_TYPE_64	= 0x01,
        PREF_RANGE_MASK		= ~0x0fUL
    };

    typedef unsigned short Class_Id;
    typedef unsigned short Vendor_Id;
    typedef unsigned short Device_Id;

    struct Locator {
        static const Reg8 INVALID = ~0;

        Locator() {}
        Locator(Reg8 b, Reg8 d) : bus(b), dev_fn(d) {}
        Locator(Reg8 b, Reg8 d, Reg8 f) : bus(b), dev_fn((d << 3) | f) {}

        operator bool() { return (bus != INVALID); }

        friend Debug & operator<<(Debug & db, const Locator & l) {
            db << "[" << l.bus << ":" << (l.dev_fn >> 3) << "." << (l.dev_fn & 0x07) << "]";
            return db;
        }

        Reg8 bus;
        Reg8 dev_fn; // dev = 5 bits, fn = 3 bits
    };

    struct Region {
        static const unsigned int N = 6;

        operator bool() { return (size != 0); }

        friend Debug & operator<<(Debug & db, const Region & r) {
            db << "{" << (r.memory ? "mem" : "io") << ",phy=" << r.phy_addr;
            if(r.memory)
        	db << ",log=" << r.log_addr;
            db << ",size=" << (void *)r.size << "}";
            return db;
        }

        bool memory;
        Phy_Addr phy_addr;
        Log_Addr log_addr;
        Reg32 size;
    };

    struct Header {
        operator bool() { return locator; }

        friend Debug & operator<<(Debug & db, const Header & h) {
            db << h.locator
               << "={vnd_id=" << (void *)(int)h.vendor_id
               << ",dev_id=" << (void *)(int)h.device_id
               << ",cmd=" << (void *)(int)h.command
               << ",stat=" << (void *)(int)h.status << "}";
            return db;
        }

        Locator locator;
        Vendor_Id vendor_id;
        Device_Id device_id;
        Reg16 command;
        Reg16 status;
        Reg8 revision_id;
        Reg8 class_prog;
        Class_Id class_id;
        Reg8 cache_line_size;
        Reg8 latency_time;
        Reg8 type;
        Reg8 bist;
        Region region[Region::N];
        Reg32 cardbus_cis;
        Vendor_Id subsystem_vendor_id;
        Device_Id subsystem_device_id;
        Reg32 rom_address;
        Reg8 interrupt_line;
        Reg8 interrupt_pin;
        Reg8 min_gnt;
        Reg8 max_lat;
    };
};

__END_SYS

#endif

#if defined(__PCI_H) && !defined(__pci_common_only__)
#include __PCI_H
#endif
