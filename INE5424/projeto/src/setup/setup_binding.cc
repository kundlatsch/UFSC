// EPOS SETUP Binding

#include <machine.h>

__BEGIN_SYS

OStream kout, kerr;

__END_SYS

extern "C" {
    __USING_SYS;

    // Libc legacy
    void _panic() { Machine::panic(); }
    void _exit(int s) { db<Setup>(ERR) << "_exit(" << s << ") called!" << endl; for(;;); }
    void __cxa_pure_virtual() { db<void>(ERR) << "Pure Virtual method called!" << endl; }

    // Utility-related methods that differ from kernel and user space.
    // OStream
    void _print(const char * s) { Display::puts(s); }
}

