// EPOS MMU Mediator Common Package

#ifndef __mmu_h
#define __mmu_h

#include <architecture/cpu.h>
#include <utility/string.h>
#include <utility/list.h>

__BEGIN_SYS

template<unsigned int DIRECTORY_BITS, unsigned int PAGE_BITS, unsigned int OFFSET_BITS>
class MMU_Common
{
protected:
    MMU_Common() {}

protected:
    // CPU imports
    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Phy_Addr Phy_Addr;

    // Page constants
    static const unsigned long PAGE_SHIFT = OFFSET_BITS;
    static const unsigned long PAGE_SIZE = 1 << PAGE_SHIFT;
    static const unsigned long DIRECTORY_SHIFT = OFFSET_BITS + PAGE_BITS;

public:
    // Memory page
    typedef unsigned char Page[PAGE_SIZE];
    typedef Page Frame;

    // Page_Table and Page_Directory entries
    typedef Phy_Addr PT_Entry;
    typedef Phy_Addr PD_Entry;

    // Page Flags
    class Flags
    {
    public:
        enum {
            PRE  = 1 << 0, // Present
            RD   = 1 << 1, // Readable
            RW   = 1 << 2, // Writable
            EX   = 1 << 3, // Executable
            USR  = 1 << 4, // Access Control (0=supervisor, 1=user)
            CD   = 1 << 5, // Cache disable (0=cacheable, 1=non-cacheable)
            CWT  = 1 << 6, // Cache mode (0=write-back, 1=write-through)
            CT   = 1 << 7, // Contiguous (0=non-contiguous, 1=contiguous)
            IO   = 1 << 8, // Memory Mapped I/O (0=memory, 1=I/O)
            SYS  = (PRE | RD | RW | EX),
            APP  = (PRE | RD | RW | EX | USR),
            SHR  = (PRE | RD | RW | EX | USR),
            APPC = (PRE | RD | EX | USR),
            APPD = (PRE | RD | RW | USR)
        };

    public:
        Flags() {}
        Flags(const Flags & f) : _flags(f._flags) {}
        Flags(unsigned int f) : _flags(f) {}

        operator unsigned int() const { return _flags; }

        friend Debug & operator<<(Debug & db, Flags f) { db << hex << f._flags << dec; return db; }

    private:
        unsigned int _flags;
    };

    // Number of entries in Page_Table and Page_Directory
    static const unsigned int PT_ENTRIES = 1 << PAGE_BITS;
    static const unsigned int PD_ENTRIES = 1 << DIRECTORY_BITS;

public:
    constexpr static unsigned long pages(unsigned long bytes) { return (bytes + sizeof(Page) - 1) / sizeof(Page); }
    constexpr static unsigned long page_tables(unsigned long pages) { return sizeof(Page) > sizeof(long) ? (pages + PT_ENTRIES - 1) / PT_ENTRIES : 0; }

    constexpr static unsigned long offset(const Log_Addr & addr) { return addr & (sizeof(Page) - 1); }
    constexpr static unsigned long indexes(const Log_Addr & addr) { return addr & ~(sizeof(Page) - 1); }

    constexpr static unsigned long page(const Log_Addr & addr) { return (addr >> PAGE_SHIFT) & (PT_ENTRIES - 1); }
    constexpr static unsigned long directory(const Log_Addr & addr) { return addr >> DIRECTORY_SHIFT; }

    constexpr static Log_Addr align_page(const Log_Addr & addr) { return (addr + sizeof(Page) - 1) & ~(sizeof(Page) - 1); }
    constexpr static Log_Addr align_directory(const Log_Addr & addr) { return (addr + PT_ENTRIES * sizeof(Page) - 1) &  ~(PT_ENTRIES * sizeof(Page) - 1); }

    constexpr static Log_Addr directory_bits(const Log_Addr & addr) { return (addr & ~((1 << DIRECTORY_BITS) - 1)); }
};

class No_MMU: public MMU_Common<0, 0, 0>
{
    friend class CPU;
    friend class Setup;

private:
    typedef Grouping_List<unsigned int> List;

public:
    // Page Flags
    typedef Flags Page_Flags;

    // Page_Table
    class Page_Table {
        friend OStream & operator<<(OStream & os, Page_Table & pt) {
            os << "{}";
            return os;
        }
    };

    // Chunk (for Segment)
    class Chunk
    {
    public:
        Chunk() {}
        Chunk(unsigned int bytes, Flags flags, Color color = WHITE): _phy_addr(alloc(bytes)), _bytes(bytes), _flags(flags) {}
        Chunk(Phy_Addr phy_addr, unsigned int bytes, Flags flags): _phy_addr(phy_addr), _bytes(bytes), _flags(flags) {}
        Chunk(Phy_Addr pt, unsigned int from, unsigned int to, Flags flags): _phy_addr(0), _bytes(0), _flags(flags) {}

        ~Chunk() { free(_phy_addr, _bytes); }

        unsigned int pts() const { return 0; }
        Flags flags() const { return _flags; }
        Page_Table * pt() const { return 0; }
        unsigned int size() const { return _bytes; }
        Phy_Addr phy_address() const { return _phy_addr; } // always CT
        int resize(unsigned int amount) { return 0; } // no resize in CT

    private:
        Phy_Addr _phy_addr;
        unsigned int _bytes;
        Flags _flags;
    };

    // Page Directory
    typedef Page_Table Page_Directory;

    // Directory (for Address_Space)
    class Directory
    {
    public:
        Directory() {}
        Directory(Page_Directory * pd) {}

        Page_Table * pd() const { return 0; }

        void activate() {}

        Log_Addr attach(const Chunk & chunk) { return chunk.phy_address(); }
        Log_Addr attach(const Chunk & chunk, Log_Addr addr) { return (addr == chunk.phy_address())? addr : Log_Addr(false); }
        void detach(const Chunk & chunk) {}
        void detach(const Chunk & chunk, Log_Addr addr) {}

        Phy_Addr physical(Log_Addr addr) { return addr; }
    };

    // DMA_Buffer (straightforward without paging)
    class DMA_Buffer: public Chunk
    {
    public:
        DMA_Buffer(unsigned int s): Chunk(s, Flags::CT) {}

        Log_Addr log_address() const { return phy_address(); }

        friend Debug & operator<<(Debug & db, const DMA_Buffer & b) {
            db << "{phy=" << b.phy_address() << ",log=" << b.log_address() << ",size=" << b.size() << ",flags=" << b.flags() << "}";
            return db;
        }
    };

    // Class Translation performs manual logical to physical address translations for debugging purposes only
    class Translation
    {
    public:
        Translation(Log_Addr addr, bool pt = false, Page_Directory * pd = 0): _addr(addr) {}

        friend OStream & operator<<(OStream & os, const Translation & t) {
            os << "{addr=" << static_cast<void *>(t._addr) << ",pd=0" << ",pd[???]=0" << ",pt[???]=0" << ",*addr=" << hex << *static_cast<unsigned int *>(t._addr) << "}";
            return os;
        }

    private:
        Log_Addr _addr;
    };

public:
    No_MMU() {}

    static Phy_Addr alloc(unsigned int bytes = 1, Color color = WHITE) {
        Phy_Addr phy(false);
        if(bytes) {
            List::Element * e = _free.search_decrementing(bytes);
            if(e)
                phy = reinterpret_cast<unsigned long>(e->object()) + e->size();
            else
                db<MMU>(ERR) << "MMU::alloc() failed!" << endl;
        }
        db<MMU>(TRC) << "MMU::alloc(bytes=" << bytes << ") => " << phy << endl;

        return phy;
    };

    static Phy_Addr calloc(unsigned int bytes = 1, Color color = WHITE) {
        Phy_Addr phy = alloc(bytes);
        memset(phy, 0, bytes);
        return phy;
    }

    static void free(Phy_Addr addr, unsigned int n = 1) {
        db<MMU>(TRC) << "MMU::free(addr=" << addr << ",n=" << n << ")" << endl;

        // No unaligned addresses if the CPU doesn't support it
        assert(Traits<CPU>::unaligned_memory_access || !(addr % (Traits<CPU>::WORD_SIZE / 8)));

        // Free blocks must be large enough to contain a list element
        assert(n > sizeof (List::Element));

        if(addr && n) {
            List::Element * e = new (addr) List::Element(addr, n);
            List::Element * m1, * m2;
            _free.insert_merging(e, &m1, &m2);
        }
    }

    static unsigned int allocable(Color color = WHITE) { return _free.head() ? _free.head()->size() : 0; }

    static Page_Directory * volatile current() { return 0; }

    static Phy_Addr physical(Log_Addr addr) { return addr; }

    static PT_Entry phy2pte(Phy_Addr frame, Flags flags) { return frame; }
    static Phy_Addr pte2phy(PT_Entry entry) { return entry; }
    static PD_Entry phy2pde(Phy_Addr frame) { return frame; }
    static Phy_Addr pde2phy(PD_Entry entry) { return entry; }

    static Log_Addr phy2log(Phy_Addr phy) { return phy; }
    static Phy_Addr log2phy(Log_Addr log) { return log; }

private:
    static Phy_Addr pd() { return 0; }
    static void pd(Phy_Addr pd) {}

    static void flush_tlb() {}
    static void flush_tlb(Log_Addr addr) {}

    static void init();

private:
    static List _free;
};


__END_SYS

#endif

#if defined(__MMU_H) && !defined(__mmu_common_only__)
#include __MMU_H
#endif
