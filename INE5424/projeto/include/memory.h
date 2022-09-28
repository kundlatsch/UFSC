// EPOS Memory Declarations

#ifndef __memory_h
#define __memory_h

#include <architecture.h>

__BEGIN_SYS

class Address_Space: private MMU::Directory
{
    friend class Init_System;   // for Address_Space(pd)
    friend class Thread;        // for Address_Space(pd)
    friend class Scratchpad;    // for Address_Space(pd)
    friend class Task;

public:
    using MMU::Directory::activate;

public:
    typedef CPU::Phy_Addr Phy_Addr;
    typedef CPU::Log_Addr Log_Addr;

public:
    Address_Space();
    Address_Space(MMU::Page_Directory * pd);
    ~Address_Space();

    using MMU::Directory::pd;

    Log_Addr attach(Segment * seg);
    Log_Addr attach(Segment * seg, Log_Addr addr);
    void detach(Segment * seg);
    void detach(Segment * seg, Log_Addr addr);

    Phy_Addr physical(Log_Addr address);
};


class Segment: public MMU::Chunk
{
    friend class Thread;        // for Segment(pt)

private:
    typedef MMU::Chunk Chunk;

public:
    typedef CPU::Phy_Addr Phy_Addr;
    typedef MMU::Flags Flags;

public:
    Segment(unsigned int bytes, Flags flags = Flags::APP);
    Segment(Phy_Addr phy_addr, unsigned int bytes, Flags flags);
    ~Segment();

    unsigned int size() const;
    Phy_Addr phy_address() const;
    int resize(int amount);
};

__END_SYS

#endif
