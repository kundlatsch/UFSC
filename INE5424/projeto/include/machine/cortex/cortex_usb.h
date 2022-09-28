// EPOS ARM Cortex USB Mediator Declarations

#ifndef __cortex_usb_h
#define __cortex_usb_h

#include <architecture/cpu.h>
#include <machine/ic.h>
#include <machine/usb.h>
#include __HEADER_MMOD(usb)

__BEGIN_SYS

class USB: private USB_Engine
{
    friend class Machine;
    friend class IC;

private:
    typedef USB_Engine Engine;

public:
    USB(unsigned int unit) {} // only one USB is supported!

    using Engine::get;
    using Engine::put;

    using Engine::read;
    using Engine::write;

    using Engine::flush;
    using Engine::ready_to_get;
    using Engine::ready_to_put;

    using Engine::int_enable;
    using Engine::int_disable;
    using Engine::power;

private:
    static void init() {
        Engine usb;
        usb.init();
        IC::int_vector(IC::INT_USB0, &int_handler);
        IC::enable(IC::INT_USB0);
    }

    static void int_handler(IC::Interrupt_Id i);
    static void eoi(IC::Interrupt_Id int_id);
};

__END_SYS

#endif
