// EPOS PC Scratchpad Memory Initialization

#include <machine/scratchpad.h>
#include <system.h>
#include <memory.h>

__BEGIN_SYS

void Scratchpad::init()
{
    db<Init, Scratchpad>(TRC) << "Scratchpad::init(a=" << CPU::Phy_Addr(ADDRESS) << ",s=" << SIZE << ")" << endl;

    _segment = new (SYSTEM) Segment(CPU::Phy_Addr(ADDRESS), SIZE, MMU::Flags::CD);
    _heap = new (SYSTEM) Heap(Address_Space(MMU::current()).attach(_segment), _segment->size());
}

__END_SYS
