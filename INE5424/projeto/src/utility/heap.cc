// EPOS Heap Utility Implementation

#include <utility/heap.h>

extern "C" { void _panic(); }

__BEGIN_UTIL

// Methods
void Heap::out_of_memory()
{
    db<Heaps>(ERR) << "Heap::alloc(this=" << this << "): out of memory!" << endl;

    _panic();
}

__END_UTIL
