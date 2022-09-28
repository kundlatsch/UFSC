// EPOS IA32 MMU Mediator Initialization

#include <architecture/mmu.h>
#include <system.h>

__BEGIN_SYS

void MMU::init()
{
    db<Init, MMU>(TRC) << "MMU::init()" << endl;

    System_Info * si = System::info();

    db<Init, MMU>(INF) << "MMU::memory={base=" << reinterpret_cast<void *>(si->bm.mem_base) << ",size="
                       << (si->bm.mem_top - si->bm.mem_base) / 1024 << "KB}" << endl;
    db<Init, MMU>(INF) << "MMU::free1={base=" << reinterpret_cast<void *>(si->pmm.free1_base) << ",size="
                       << (si->pmm.free1_top - si->pmm.free1_base) / 1024 << "KB}" << endl;
    db<Init, MMU>(INF) << "MMU::free2={base=" << reinterpret_cast<void *>(si->pmm.free2_base) << ",size="
                       << (si->pmm.free2_top - si->pmm.free2_base) / 1024 << "KB}" << endl;
    db<Init, MMU>(INF) << "MMU::free3={base=" << reinterpret_cast<void *>(si->pmm.free3_base) << ",size="
                       << (si->pmm.free3_top - si->pmm.free3_base) / 1024 << "KB}" << endl;

    // BIG NOTE HERE: INIT (i.e. this program) will be part of the free
    // storage after the following is executed, but it will remain alive
    // This only works because the _free.insert_merging() only
    // touches the first page of each chunk and INIT is not there

    if(colorful) {
        int f1b = si->pmm.free1_base;
        int f1t = si->pmm.free1_top;
        int f2b = si->pmm.free2_base;
        int f2t = si->pmm.free2_top;
        int f3b = si->pmm.free3_base;
        int f3t = si->pmm.free3_top;

        // Insert a bulk of memory large enough to contain the System's heap into _free[WHITE] lists
        int size = Traits<System>::HEAP_SIZE;
        if((f1t - f1b) > size) {
            white_free(f1b, pages(f1b + size));
            f1b += size;
            size = 0;
        } else {
            white_free(f1b, pages(f1t - f1b));
            size -= (f1t - f1b);
            f1b = f1t = 0;
        }
        if(size > 0) {
            if((f2t - f2b) > size) {
                white_free(f2b, pages(f2b + size));
                f2b += size;
                size = 0;
            } else {
                white_free(f2b, pages(f2t - f2b));
                size -= (f2t - f2b);
                f2b = f2t = 0;
            }
        }
        if(size > 0) {
            if((f3t - f3b) > size) {
                white_free(f3b, pages(f3b + size));
                f3b += size;
                size = 0;
            } else {
                white_free(f3b, pages(f3t - f3b));
                size -= (f3t - f3b);
                f3b = f3t = 0;
            }
        }
        if((size > 0) || (_free[WHITE].grouped_size() * MMU::PAGE_SIZE < Traits<System>::HEAP_SIZE))
            db<Init, MMU>(ERR) << "MMU::int: System's heap size (Traits<System>::HEAP_SIZE=" << Traits<System>::HEAP_SIZE << ") is larger than memory!" << endl;

        // Insert the remaining free memory into the _free[color] lists
        int frame = f1b;
        while(frame < f1t) {
            free(frame);
            frame += MMU::PAGE_SIZE;
        }

        frame = f2b;
        while(frame < f2t) {
            free(frame);
            frame += MMU::PAGE_SIZE;
        }

        frame = f3b;
        while(frame < f3t) {
            free(frame);
            frame += MMU::PAGE_SIZE;
        }
    } else {
        // Insert all free memory into the _free[WHITE] list
        free(si->pmm.free1_base, pages(si->pmm.free1_top - si->pmm.free1_base));
        free(si->pmm.free2_base, pages(si->pmm.free2_top - si->pmm.free2_base));
        free(si->pmm.free3_base, pages(si->pmm.free3_top - si->pmm.free3_base));
    }

    // Remember the master page directory (created during SETUP)
    _master = current();
    db<Init, MMU>(INF) << "MMU::master page directory=" << _master << endl;
}

__END_SYS

