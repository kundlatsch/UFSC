// EPOS Cortex Initialization

#include <machine.h>

__BEGIN_SYS

void Machine::pre_init(System_Info * si)
{
    Engine::pre_init();

    if(CPU::id() == 0)
        Display::init();

    db<Init, Machine>(TRC) << "Machine::pre_init()" << endl;

    if(Traits<IC>::enabled)
        IC::init();
}

void Machine::init()
{
    db<Init, Machine>(TRC) << "Machine::init()" << endl;

    Engine::init();

    if(Traits<Timer>::enabled)
        Timer::init();

#ifdef __USB_H
    if(Traits<USB>::enabled)
        USB::init();
#endif

#ifdef __NIC_H
#ifdef __ethernet__
    if(Traits<Ethernet>::enabled)
        Initializer<Ethernet>::init();
#endif
#ifdef __ieee802_15_4__
    if(Traits<IEEE802_15_4>::enabled)
        Initializer<IEEE802_15_4>::init();
#endif
#endif

#ifdef __MODEM_H
    if(Traits<Modem>::enabled)
        Initializer<Modem>::init();
#endif
}

__END_SYS
