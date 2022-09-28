// EPOS RISC-V 32 MMU Mediator Declarations

#ifndef __rv32_mmu_h
#define __rv32_mmu_h

#include <architecture/mmu.h>
#include <system/memory_map.h>

__BEGIN_SYS

class RV32S_MMU: public MMU_Common<10, 10, 12>
{
    friend class CPU;
    friend class Setup;

private:
    typedef Grouping_List<Frame> List;

    static const bool colorful = Traits<MMU>::colorful;
    static const unsigned int COLORS = Traits<MMU>::COLORS;
    static const unsigned int RAM_BASE = Memory_Map::RAM_BASE;
    static const unsigned int APP_LOW = Memory_Map::APP_LOW;
    static const unsigned int PHY_MEM = Memory_Map::PHY_MEM;
    static const unsigned int SYS = Memory_Map::SYS;
    static const unsigned int IO = Memory_Map::IO;

public:
    // Page Flags
    class Page_Flags
    {
    public:
        enum {
            V    = 1 << 0, // Valid
            R    = 1 << 1, // Readable
            W    = 1 << 2, // Writable
            X    = 1 << 3, // Executable
            U    = 1 << 4, // User accessible
            G    = 1 << 5, // Global (mapped in multiple PTs)
            A    = 1 << 6, // Accessed
            D    = 1 << 7, // Dirty
            CT   = 1 << 8, // Contiguous (reserved for use by supervisor RSW)
            MIO  = 1 << 9, // I/O (reserved for use by supervisor RSW)
            APP  = (V | R | W | X),
            APPC = (V | R | X),
            APPD = (V | R | W),
            SYS  = (V | R | W | X),
            IO   = (SYS | MIO),
            DMA  = (SYS | CT),
            MASK = (1 << 10) - 1
        };

    public:
        Page_Flags() {}
        Page_Flags(unsigned int f) : _flags(f) {}
        Page_Flags(Flags f) : _flags(V |
                                     ((f & Flags::RD)  ? R  : 0) |
                                     ((f & Flags::RW)  ? W  : 0) |
                                     ((f & Flags::EX)  ? X  : 0) |
//                                     ((f & Flags::USR) ? U  : 0) |
                                     ((f & Flags::CWT) ? 0  : 0) |
                                     ((f & Flags::CD)  ? 0  : 0) |
                                     ((f & Flags::CT)  ? CT : 0) |
                                     ((f & Flags::IO)  ? IO : 0) ) {}

        operator unsigned int() const { return _flags; }

        friend Debug & operator<<(Debug & db, const Page_Flags & f) { db << hex << f._flags; return db; }

    private:
        unsigned int _flags;
    };

    // Page_Table
    class Page_Table
    {
    public:
        Page_Table() {}

        PT_Entry & operator[](unsigned int i) { return _entry[i]; }
        Page_Table & log() { return *static_cast<Page_Table *>(phy2log(this)); }

        void map(int from, int to, Page_Flags flags, Color color) {
            Phy_Addr * addr = alloc(to - from, color);
            if(addr)
                remap(addr, from, to, flags);
            else
                for( ; from < to; from++) {
                    Log_Addr * pte = phy2log(&_entry[from]);
                    *pte = phy2pte(alloc(1, color), flags);
                }
        }

        void map_contiguous(int from, int to, Page_Flags flags, Color color) {
            remap(alloc(to - from, color), from, to, flags);
        }

        void remap(Phy_Addr addr, int from, int to, Page_Flags flags) {
            addr = align_page(addr);
            for( ; from < to; from++) {
                Log_Addr * pte = phy2log(&_entry[from]);
                *pte = phy2pte(addr, flags);
                addr += sizeof(Page);
            }
        }

        void unmap(int from, int to) {
            for( ; from < to; from++) {
                free(_entry[from]);
                Log_Addr * pte = phy2log(&_entry[from]);
                *pte = 0;
            }
        }

        friend OStream & operator<<(OStream & os, Page_Table & pt) {
            os << "{\n";
            int brk = 0;
            for(unsigned int i = 0; i < PT_ENTRIES; i++)
                if(pt[i]) {
                    os << "[" << i << "]=" << pte2phy(pt[i]) << "  ";
                    if(!(++brk % 4))
                        os << "\n";
                }
            os << "\n}";
            return os;
        }

    private:
        PT_Entry _entry[PT_ENTRIES]; // the Phy_Addr in each entry passed through phy2pte()
    };

    // Chunk (for Segment)
    class Chunk
    {
    public:
        Chunk() {}

        Chunk(unsigned int bytes, Flags flags, Color color = WHITE)
        : _from(0), _to(pages(bytes)), _pts(page_tables(_to - _from)), _flags(Page_Flags(flags)), _pt(calloc(_pts, WHITE)) {
            if(_flags & Page_Flags::CT)
                _pt->map_contiguous(_from, _to, _flags, color);
            else
                _pt->map(_from, _to, _flags, color);
        }

        Chunk(Phy_Addr phy_addr, unsigned int bytes, Flags flags)
        : _from(0), _to(pages(bytes)), _pts(page_tables(_to - _from)), _flags(Page_Flags(flags)), _pt(calloc(_pts, WHITE)) {
            _pt->remap(phy_addr, _from, _to, flags);
        }

        Chunk(Phy_Addr pt, unsigned int from, unsigned int to, Flags flags)
        : _from(from), _to(to), _pts(page_tables(_to - _from)), _flags(flags), _pt(pt) {}

        ~Chunk() {
            if(!(_flags & Page_Flags::IO)) {
                if(_flags & Page_Flags::CT)
                    free((*_pt)[_from], _to - _from);
                else
                    for( ; _from < _to; _from++)
                        free((*_pt)[_from]);
            }
            free(_pt, _pts);
        }

        unsigned int pts() const { return _pts; }
        Page_Flags flags() const { return _flags; }
        Page_Table * pt() const { return _pt; }
        unsigned int size() const { return (_to - _from) * sizeof(Page); }

        Phy_Addr phy_address() const {
            return (_flags & Page_Flags::CT) ? Phy_Addr(indexes((*_pt)[_from])) : Phy_Addr(false);
        }

        int resize(unsigned int amount) {
            if(_flags & Page_Flags::CT)
                return 0;

            unsigned int pgs = pages(amount);

            Color color = colorful ? phy2color(_pt) : WHITE;

            unsigned int free_pgs = _pts * PT_ENTRIES - _to;
            if(free_pgs < pgs) { // resize _pt
                unsigned int pts = _pts + page_tables(pgs - free_pgs);
                Page_Table * pt = calloc(pts, color);
                memcpy(phy2log(pt), phy2log(_pt), _pts * sizeof(Page));
                free(_pt, _pts);
                _pt = pt;
                _pts = pts;
            }

            _pt->map(_to, _to + pgs, _flags, color);
            _to += pgs;

            return pgs * sizeof(Page);
        }

    private:
        unsigned int _from;
        unsigned int _to;
        unsigned int _pts;
        Page_Flags _flags;
        Page_Table * _pt; // this is a physical address
    };

    // Page Directory
    typedef Page_Table Page_Directory;

    // Directory (for Address_Space)
    class Directory
    {
    public:
        Directory() : _pd(calloc(1, WHITE)), _free(true) {
            for(unsigned int i = directory(IO); i < directory(APP_LOW); i++)
                (*_pd)[i] = (*_master)[i];
            
            for(unsigned int i = directory(SYS); i < PD_ENTRIES; i++)
                (*_pd)[i] = (*_master)[i];
        }

        Directory(Page_Directory * pd) : _pd(pd), _free(false) {}

        ~Directory() { if(_free) free(_pd); }

        Phy_Addr pd() const { return _pd; }

        void activate() const { RV32S_MMU::pd(_pd); }

        Log_Addr attach(const Chunk & chunk, unsigned int from = directory(APP_LOW)) {
            for(unsigned int i = from; i < PD_ENTRIES; i++)
                if(attach(i, chunk.pt(), chunk.pts(), chunk.flags()))
                    return i << DIRECTORY_SHIFT;
            return Log_Addr(false);
        }

        Log_Addr attach(const Chunk & chunk, Log_Addr addr) {
            unsigned int from = directory(addr);
            if(attach(from, chunk.pt(), chunk.pts(), chunk.flags()))
                return from << DIRECTORY_SHIFT;
            return Log_Addr(false);
        }

        void detach(const Chunk & chunk) {
            for(unsigned int i = 0; i < PD_ENTRIES; i++) {
                if(indexes(pte2phy((*_pd)[i])) == indexes(chunk.pt())) {
                    detach(i, chunk.pt(), chunk.pts());
                    return;
                }
            }
            db<MMU>(WRN) << "MMU::Directory::detach(pt=" << chunk.pt() << ") failed!" << endl;
        }

        void detach(const Chunk & chunk, Log_Addr addr) {
            unsigned int from = directory(addr);
            if(indexes(pte2phy((*_pd)[from])) != indexes(chunk.pt())) {
                db<MMU>(WRN) << "MMU::Directory::detach(pt=" << chunk.pt() << ",addr=" << addr << ") failed!" << endl;
                return;
            }
            detach(from, chunk.pt(), chunk.pts());
        }

        Phy_Addr physical(Log_Addr addr) {
            PD_Entry pde = (*_pd)[directory(addr)];
            Page_Table * pt = static_cast<Page_Table *>(pde2phy(pde));
            PT_Entry pte = pt->log()[page(addr)];
            return pte | offset(addr);
        }

    private:
        bool attach(unsigned int from, const Page_Table * pt, unsigned int n, Page_Flags flags) {
            for(unsigned int i = from; i < from + n; i++)
                if(_pd->log()[i])
                    return false;
            for(unsigned int i = from; i < from + n; i++, pt++)
                _pd->log()[i] = phy2pde(Phy_Addr(pt));
            return true;
        }

        void detach(unsigned int from, const Page_Table * pt, unsigned int n) {
            for(unsigned int i = from; i < from + n; i++) {
                (*_pd)[i] = 0;
                flush_tlb(i << DIRECTORY_SHIFT);
            }
        }

    private:
        Page_Directory * _pd;  // this is a physical address, but operator*() returns a logical address
        bool _free;
    };

    // DMA_Buffer
    class DMA_Buffer: public Chunk
    {
    public:
        DMA_Buffer(unsigned int s) : Chunk(s, Page_Flags::DMA) {
            Directory dir(current());
            _log_addr = dir.attach(*this);
            db<MMU>(TRC) << "MMU::DMA_Buffer() => " << *this << endl;
        }

        DMA_Buffer(unsigned int s, Log_Addr d): Chunk(s, Page_Flags::DMA) {
            Directory dir(current());
            _log_addr = dir.attach(*this);
            memcpy(_log_addr, d, s);
            db<MMU>(TRC) << "MMU::DMA_Buffer(phy=" << *this << " <= " << d << endl;
        }

        Log_Addr log_address() const { return _log_addr; }

        friend OStream & operator<<(OStream & os, const DMA_Buffer & b) {
            os << "{phy=" << b.phy_address() << ",log=" << b.log_address() << ",size=" << b.size() << ",flags=" << b.flags() << "}";
            return os;
        }

    private:
        Log_Addr _log_addr;
    };

    // Class Translation performs manual logical to physical address translations for debugging purposes only
    class Translation
    {
    public:
        Translation(Log_Addr addr, bool pt = false, Page_Directory * pd = 0): _addr(addr), _show_pt(pt), _pd(pd) {}

        friend OStream & operator<<(OStream & os, const Translation & t) {
            Page_Directory * pd = t._pd ? t._pd : current();
            PD_Entry pde = pd->log()[directory(t._addr)];
            Page_Table * pt = static_cast<Page_Table *>(pde2phy(pde));
            PT_Entry pte = pt->log()[page(t._addr)];

            os << "{addr=" << static_cast<void *>(t._addr) << ",pd=" << pd << ",pd[" << directory(t._addr) << "]=" << pde << ",pt=" << pt;
            if(t._show_pt)
                os << "=>" << pt->log();
            os << ",pt[" << page(t._addr) << "]=" << pte << ",f=" << pte2phy(pte) << ",*addr=" << hex << *static_cast<unsigned int *>(t._addr) << "}";
            return os;
        }

    private:
        Log_Addr _addr;
        bool _show_pt;
        Page_Directory * _pd;
    };

public:
    RV32S_MMU() {}

    static Phy_Addr alloc(unsigned int frames = 1, Color color = WHITE) {
        Phy_Addr phy(false);

        if(frames) {
            List::Element * e = _free[color].search_decrementing(frames);
            if(e) {
                phy = e->object() + e->size();
                db<MMU>(TRC) << "MMU::alloc(frames=" << frames << ",color=" << color << ") => " << phy << endl;
            } else
                if(colorful)
                    db<MMU>(INF) << "MMU::alloc(frames=" << frames << ",color=" << color << ") => failed!" << endl;
                else
                    db<MMU>(WRN) << "MMU::alloc(frames=" << frames << ",color=" << color << ") => failed!" << endl;
        }

        return phy;
    }

    static Phy_Addr calloc(unsigned int frames = 1, Color color = WHITE) {
        Phy_Addr phy = alloc(frames, color);
        memset(phy2log(phy), 0, sizeof(Frame) * frames);
        return phy;
    }

    static void free(Phy_Addr frame, int n = 1) {
        // Clean up MMU flags in frame address
        frame = indexes(frame);
        Color color = colorful ? phy2color(frame) : WHITE;

        db<MMU>(TRC) << "MMU::free(frame=" << frame << ",color=" << color << ",n=" << n << ")" << endl;

        if(frame && n) {
            List::Element * e = new (phy2log(frame)) List::Element(frame, n);
            List::Element * m1, * m2;
            _free[color].insert_merging(e, &m1, &m2);
        }
    }

    static void white_free(Phy_Addr frame, int n) {
        // Clean up MMU flags in frame address
        frame = indexes(frame);

        db<MMU>(TRC) << "MMU::free(frame=" << frame << ",color=" << WHITE << ",n=" << n << ")" << endl;

        if(frame && n) {
            List::Element * e = new (phy2log(frame)) List::Element(frame, n);
            List::Element * m1, * m2;
            _free[WHITE].insert_merging(e, &m1, &m2);
        }
    }

    static unsigned int allocable(Color color = WHITE) { return _free[color].head() ? _free[color].head()->size() : 0; }

    static Page_Directory * volatile current() { return static_cast<Page_Directory * volatile>(pd()); }

    static Phy_Addr physical(Log_Addr addr) {
        Page_Directory * pd = current();
        Page_Table * pt = pd->log()[directory(addr)];
        return pt->log()[page(addr)] | offset(addr);
    }

    static PT_Entry phy2pte(Phy_Addr frame, Page_Flags flags) { return (frame >> 2) | flags; }
    static Phy_Addr pte2phy(PT_Entry entry) { return (entry & ~Page_Flags::MASK) << 2; }
    static PD_Entry phy2pde(Phy_Addr frame) { return (frame >> 2) | Page_Flags::V; }
    static Phy_Addr pde2phy(PD_Entry entry) { return (entry & ~Page_Flags::MASK) << 2; }

    static Log_Addr phy2log(Phy_Addr phy) { return Log_Addr((RAM_BASE == PHY_MEM) ? phy : (RAM_BASE > PHY_MEM) ? phy - (RAM_BASE - PHY_MEM) : phy + (PHY_MEM - RAM_BASE)); }
    static Phy_Addr log2phy(Log_Addr log) { return Phy_Addr((RAM_BASE == PHY_MEM) ? log : (RAM_BASE > PHY_MEM) ? log + (RAM_BASE - PHY_MEM) : log - (PHY_MEM - RAM_BASE)); }

    static Color phy2color(Phy_Addr phy) { return static_cast<Color>(colorful ? ((phy >> PAGE_SHIFT) & 0x7f) % COLORS : WHITE); } // TODO: what is 0x7f

    static Color log2color(Log_Addr log) {
        if(colorful) {
            Page_Directory * pd = current();
            Page_Table * pt = pd->log()[directory(log)];
            Phy_Addr phy = pt->log()[page(log)] | offset(log);
            return static_cast<Color>(((phy >> PAGE_SHIFT) & 0x7f) % COLORS);
        } else
            return WHITE;
    }

private:
    static Phy_Addr pd() { return CPU::satp() << 12; }
    static void pd(Phy_Addr pd) { CPU::satp((1 << 31) | (pd >> 12)); }

    static void flush_tlb() { CPU::flush_tlb(); }
    static void flush_tlb(Log_Addr addr) { CPU::flush_tlb(addr); }

    static void init();

private:
    static List _free[colorful * COLORS + 1]; // +1 for WHITE
    static Page_Directory * _master;
};

class MMU: public IF<Traits<System>::multitask, RV32S_MMU, No_MMU>::Result {};

__END_SYS

#endif
