// EPOS Address_Space Implementation

#include <memory.h>
__BEGIN_SYS

// Methods
Address_Space::Address_Space(MMU::Page_Directory * pd) : MMU::Directory(pd)
{
    db<Address_Space> (TRC) << "Address_Space(pd=" << pd << ") [Directory::pd=" << Directory::pd() << "] => " << this << endl;
}

Address_Space::Address_Space()
{
    db<Address_Space> (TRC) << "Address_Space() [Directory::pd=" << Directory::pd() << "] => " << this << endl;
}

Address_Space::~Address_Space()
{
    db<Address_Space>(TRC) << "~Address_Space(this=" << this << ") [Directory::pd=" << Directory::pd() << "]" << endl;
}

Address_Space::Log_Addr Address_Space::attach(Segment * seg)
{
    Log_Addr tmp = Directory::attach(*seg);

    db<Address_Space>(TRC) << "Address_Space::attach(this=" << this << ",seg=" << seg << ") => " << tmp << endl;

    return tmp;
}

Address_Space::Log_Addr Address_Space::attach(Segment * seg, Log_Addr addr)
{
    Log_Addr tmp = Directory::attach(*seg, addr);

    db<Address_Space>(TRC) << "Address_Space::attach(this=" << this << ",seg=" << seg << ",addr=" << addr << ") => " << tmp << endl;

    return tmp;
}

void Address_Space::detach(Segment * seg)
{
    db<Address_Space>(TRC) << "Address_Space::detach(this=" << this << ",seg=" << seg << ")" << endl;

    Directory::detach(*seg);
}

void Address_Space::detach(Segment * seg, Log_Addr addr)
{
    db<Address_Space>(TRC) << "Address_Space::detach(this=" << this << ",seg=" << seg << ",addr=" << addr << ")" << endl;

    Directory::detach(*seg, addr);
}

Address_Space::Phy_Addr Address_Space::physical(Log_Addr address)
{
    return Directory::physical(address);
}

__END_SYS
