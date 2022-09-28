// EPOS PC PCI Mediator

#include <machine/pci.h>

__BEGIN_SYS

// Class attributes
PCI::Phy_Addr PCI::_phy_io_mem;
PCI::Reg32 PCI::_base_address[Region::N] = {
    BASE_ADDRESS_0, BASE_ADDRESS_1,
    BASE_ADDRESS_2, BASE_ADDRESS_3,
    BASE_ADDRESS_4, BASE_ADDRESS_5
};

// Methods
void PCI::header(const PCI::Locator & l, PCI::Header * h)
{
    h->vendor_id = cfg16(l.bus, l.dev_fn, VENDOR_ID);
    if((h->vendor_id != 0) && (h->vendor_id != 0xffff)) {
        h->locator = l;
        h->device_id = cfg16(l.bus, l.dev_fn, DEVICE_ID);
        h->command = cfg16(l.bus, l.dev_fn, COMMAND);
        h->status = cfg16(l.bus, l.dev_fn, STATUS);
        h->revision_id = cfg8(l.bus, l.dev_fn, REVISION_ID);
        h->class_prog = cfg8(l.bus, l.dev_fn, CLASS_PROG);
        h->class_id = cfg16(l.bus, l.dev_fn, CLASS_ID);
        h->cache_line_size = cfg8(l.bus, l.dev_fn, CACHE_LINE_SIZE);
        h->latency_time = cfg8(l.bus, l.dev_fn, LATENCY_TIMER);
        h->type = cfg8(l.bus, l.dev_fn, HEADER_TYPE);
        h->bist = cfg8(l.bus, l.dev_fn, BIST);
        for(unsigned int i = 0; i < Region::N; i++) {
            CPU::int_disable();
            h->region[i].phy_addr = cfg32(l.bus, l.dev_fn, _base_address[i]);
            cfg32(l.bus, l.dev_fn, _base_address[i], ~0);
            h->region[i].size = cfg32(l.bus, l.dev_fn, _base_address[i]);
            cfg32(l.bus, l.dev_fn, _base_address[i], h->region[i].phy_addr);
            CPU::int_enable();
            if((h->region[i].phy_addr & BASE_ADDRESS_SPACE_MASK) || ((h->type & 0x7f) == HEADER_TYPE_BRIDGE || (h->type & 0x7f) == HEADER_TYPE_CARDBUS) || h->class_id == 257 || h->class_id == 1027 || h->class_id == 1536 || h->class_id == 1537 || h->class_id == 1920 || h->class_id == 3075 || h->class_id == 3077) { // I/O
                h->region[i].memory = false;
                h->region[i].phy_addr &= BASE_ADDRESS_IO_MASK;
                h->region[i].log_addr = ~0;
                h->region[i].size = ~(h->region[i].size & BASE_ADDRESS_IO_MASK) + 1;
            } else {
                h->region[i].memory = true;
                h->region[i].phy_addr &= BASE_ADDRESS_MEM_MASK;
                h->region[i].log_addr = phy2log(h->region[i].phy_addr);
                h->region[i].size = ~(h->region[i].size & BASE_ADDRESS_MEM_MASK) + 1;

                if(!h->region[i].phy_addr) { // Broken devices report false regions often starting at zero
                    h->region[i].memory = false;
                    db<PCI>(TRC) << "PCI: device " << l << " reports implausible region (starting at zero). Ignoring!" << endl;
                }

                if(h->region[i].size > MAX_REGION_SIZE) { // Broken devices report false regions often starting at zero and extending up to the PIC area
                    h->region[i].memory = false;
                    db<PCI>(WRN) << "PCI: device " << l << " reports implausible large region. Ignoring!" << endl;
                }
            }
            h->cardbus_cis = cfg32(l.bus, l.dev_fn, CARDBUS_CIS);
            h->subsystem_vendor_id = cfg16(l.bus, l.dev_fn, SUBSYSTEM_VENDOR_ID);
            h->subsystem_device_id = cfg16(l.bus, l.dev_fn, SUBSYSTEM_DEVICE_ID);
            h->rom_address = cfg32(l.bus, l.dev_fn, ROM_ADDRESS);
            h->interrupt_line = cfg8(l.bus, l.dev_fn, INTERRUPT_LINE);
            h->interrupt_pin = cfg8(l.bus, l.dev_fn, INTERRUPT_PIN);
            h->min_gnt = cfg8(l.bus, l.dev_fn, MIN_GNT);
            h->max_lat = cfg8(l.bus, l.dev_fn, MAX_LAT);
        }
    } else
        h->locator = Locator(Locator::INVALID, Locator::INVALID);
}

PCI::Locator PCI::scan(const PCI::Class_Id & c, int order)
{
    db<PCI>(TRC) << "PCI::scan(class=" << c << ",order=" << order << ")" << endl;

    for(int bus = 0 ; bus <= MAX_BUS; bus++)
        for(int dfn = 0; dfn <= MAX_DEV_FN; dfn++)
            if(class_id(bus, dfn) == c)
        	if(!order--)
        	    return Locator(bus, dfn);

    return Locator(Locator::INVALID, Locator::INVALID);
}

PCI::Locator PCI::scan(const PCI::Vendor_Id & v, const PCI::Device_Id & d, int order)
{
    db<PCI>(TRC) << "PCI::scan(vend=" << v << ",dev=" << d << ",order=" << order << ")" << endl;

    for(int bus = 0 ; bus <= MAX_BUS; bus++)
        for(int dfn = 0; dfn <= MAX_DEV_FN; dfn++)
            if((vendor_id(bus, dfn) == v) && (device_id(bus, dfn) == d))
        	if(!order--)
        	    return Locator(bus, dfn);

    return Locator(Locator::INVALID, Locator::INVALID);
}

__END_SYS
