// EPOS PC PCI Mediator

#ifndef __pc_pci_h
#define __pc_pci_h

#include <machine/pci.h>
#include <system/memory_map.h>

__BEGIN_SYS

class PCI: public PCI_Common
{
    friend class Machine;

private:
    static const int MAX_BUS = Traits<PCI>::MAX_BUS;
    static const int MAX_DEV_FN = Traits<PCI>::MAX_DEV_FN;
    static const unsigned int MAX_REGION_SIZE = Traits<PCI>::MAX_REGION_SIZE;
    static const unsigned long LOG_IO_MEM = Memory_Map::PCI;

    // From Intel 82443BX manual
    // CONFADDR is a 32 bit register accessed only when referenced as a Dword.
    // A Byte or Word reference will "pass through" the Configuration Address
    // Register onto the PCI bus as an I/O cycle. The CONFADDR register
    // contains the Bus Number, Device Number, Function Number, and Register
    // Number for which a subsequent configuration access is intended.
    // [31]    -> config enable
    // [24:30] -> reserved
    // [11:15] -> device (5 bits)
    // [ 8:10] -> function (3 bits)
    // [ 2: 7] -> register (6 bits)
    // [ 0: 1] -> reserved
    static const int CONFADDR = 0xcf8;

    // CONFDATA is a 32 bit read/write window into configuration space.
    // The portion of configuration space that is referenced by CONFDATA is
    // determined by the contents of CONFADDR.
    static const int CONFDATA = 0xcfc;

public:
    PCI() {}

    static Locator scan(const Class_Id & c, int order);
    static Locator scan(const Vendor_Id & v, const Device_Id & d, int order);

    static void header(const Locator & l, Header * h);
    static Reg16 command(const Locator & l) {
        return cfg16(l.bus, l.dev_fn, COMMAND);
    }
    static void command(const Locator & l, Reg16 v) {
        cfg16(l.bus, l.dev_fn, COMMAND, v);
    }
    static Reg16 status(const Locator & l) {
        return cfg16(l.bus, l.dev_fn, STATUS);
    }
    static void  status(const Locator & l, Reg16 v) {
        cfg16(l.bus, l.dev_fn, STATUS, v);
    }

private:
    static void init();

    static int cmd(Reg8 bus, Reg8 dev_fn, Reg8 addr) {
        return 0x80000000 | (bus << 16) | (dev_fn << 8) | (addr & ~3);
    }

    static Reg8 cfg8(Reg8 bus, Reg8 dev_fn, Reg8 addr) {
        CPU::out32(CONFADDR, cmd(bus, dev_fn, addr));
        return CPU::in8(CONFDATA + (addr & 3));
    }
    static Reg16 cfg16(Reg8 bus, Reg8 dev_fn, Reg8 addr) {
        CPU::out32(CONFADDR, cmd(bus, dev_fn, addr));
        return CPU::in16(CONFDATA + (addr & 2));
    }
    static Reg32 cfg32(Reg8 bus, Reg8 dev_fn, Reg8 addr) {
        CPU::out32(CONFADDR, cmd(bus, dev_fn, addr));
        return CPU::in32(CONFDATA);
    }
    static void cfg8(Reg8 bus, Reg8 dev_fn, Reg8 addr, Reg8 value) {
        CPU::out32(CONFADDR, cmd(bus, dev_fn, addr));
        CPU::out8(CONFDATA + (addr & 3), value);
    }
    static void cfg16(Reg8 bus, Reg8 dev_fn, Reg8 addr, Reg16 value) {
        CPU::out32(CONFADDR, cmd(bus, dev_fn, addr));
        CPU::out16(CONFDATA + (addr & 2), value);
    }
    static void cfg32(Reg8 bus, Reg8 dev_fn, Reg8 addr, Reg32 value) {
        CPU::out32(CONFADDR, cmd(bus, dev_fn, addr));
        CPU::out32(CONFDATA, value);
    }

    static Reg16 vendor_id(Reg8 bus, Reg8 dev_fn) {
        return cfg16(bus, dev_fn, VENDOR_ID);
    }
    static Reg16 device_id(Reg8 bus, Reg8 dev_fn) {
        return cfg16(bus, dev_fn, DEVICE_ID);
    }
    static Reg16 class_id(Reg8 bus, Reg8 dev_fn) {
        return cfg16(bus, dev_fn, CLASS_ID);
    }

    static Log_Addr phy2log(const Phy_Addr & addr) {
        return LOG_IO_MEM + (addr - _phy_io_mem);
    }

  private:
    static Phy_Addr _phy_io_mem;
    static Reg32 _base_address[Region::N];
};

__END_SYS

#endif
