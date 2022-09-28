// EPOS Scratchpad Memory Mediator Declarations

#ifndef __scratchpad_h
#define __scratchpad_h

#include <utility/heap.h>
#include <memory.h>

__BEGIN_SYS

// TODO: This design is a bit weird, since this is not a Common package (it's a base indeed). It could be refactored to have an "Engine" or to just leave ::init() to be implemented by each Machine
class Scratchpad_Base
{
    friend void * ::operator new(size_t, const EPOS::Scratchpad_Allocator &);
    friend void * ::operator new[](size_t, const EPOS::Scratchpad_Allocator &);

protected:
    Scratchpad_Base() {}

public:
    static void * alloc(unsigned int bytes) { return _heap->alloc(bytes); }

protected:
    static Segment * _segment;
    static Heap * _heap;
};

__END_SYS

#endif

#if defined(__SCRATCHPAD_H) && !defined(__scratchpad_common_only__)
#include __SCRATCHPAD_H

inline void * operator new(size_t bytes, const EPOS::Scratchpad_Allocator & allocator) {
    return _SYS::Scratchpad::_heap->alloc(bytes);
}

inline void * operator new[](size_t bytes, const EPOS::Scratchpad_Allocator & allocator) {
    return _SYS::Scratchpad::_heap->alloc(bytes);
}

#endif

