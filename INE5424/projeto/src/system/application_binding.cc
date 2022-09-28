// EPOS Applicaiton Component Implementation

#include <utility/ostream.h>
#include <utility/heap.h>
#include <machine.h>
#include <memory.h>
#include <process.h>
#include <system.h>
#include <syscall/stub_thread.h>

__BEGIN_SYS
OStream kout;
OStream kerr;
__END_SYS


// Bindings
extern "C" {
    // Libc legacy
    void _panic() { _API::Stub_Thread::exit(-1); }
    void _exit(int s) { _API::Stub_Thread::exit(s); for(;;); }

    void __exit() { _API::Stub_Thread::exit(_SYS::CPU::fr()); }  // must be handled by the Page Fault handler for user-level tasks
    //void __cxa_pure_virtual() { db<void>(ERR) << "Pure Virtual method called!" << endl; }
}

__USING_SYS;
// Bindings
extern "C" {
    void _syscall(void * m) { CPU::syscall(m); }

    // OStream
    void _print(const char * s) {
        // Message msg(Message::ENTITY::DISPLAY, Message::PRINT, reinterpret_cast<int>(s));
        Message msg(Message::ENTITY::DISPLAY, Message::PRINT);
        msg.act();
    }
}