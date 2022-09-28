// EPOS Cortex USB Mediator Implementation

#include <machine/ic.h>
#include <machine/usb.h>

#ifdef __USB_H

__BEGIN_SYS

void USB::int_handler(IC::Interrupt_Id i) {
    Engine usb; usb.handle_int(i);
}

void USB::eoi(IC::Interrupt_Id int_id) {
    Engine usb; usb.eoi();
}

__END_SYS

#endif
