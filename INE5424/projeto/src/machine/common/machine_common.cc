// EPOS Machine Mediator Common Package Implementation

#include <machine/machine.h>

extern "C" unsigned int __bss_start;    // defined by GCC
extern "C" unsigned int _end;           // defined by GCC
extern "C" void _bss_clear() __attribute__ ((alias("_ZN4EPOS1S14Machine_Common9clear_bssEv")));;

__BEGIN_SYS

void Machine_Common::clear_bss()
{
    unsigned int * ptr = &__bss_start;

    while(ptr < &_end)
        *ptr++ = 0;
}

__END_SYS
