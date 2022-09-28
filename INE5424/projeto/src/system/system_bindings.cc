// EPOS System Scaffold and System Component Implementation

#include <utility/ostream.h>
#include <utility/heap.h>
#include <machine.h>
#include <memory.h>
#include <process.h>
#include <system.h>
#include <syscall/agent.h>

// Bindings
extern "C" {
    __USING_SYS;
    // Libc legacy
    void _panic() { Machine::panic(); }
    void _exit(int s) { Thread::exit(s); for(;;); }
    void __exit() { Thread::exit(CPU::fr()); }  // must be handled by the Page Fault handler for user-level tasks
    void __cxa_pure_virtual() { db<void>(ERR) << "Pure Virtual method called!" << endl; }
    void _syscall(void *m) { CPU::syscall(m); } 
    void _sysexec() { Agent::_exec(); } 

    // Utility-related methods that differ from kernel and user space.
    // OStream
    void _print(const char * s) { Display::puts(s); }
}