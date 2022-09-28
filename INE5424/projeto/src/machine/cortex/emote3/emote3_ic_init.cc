// EPOS Cortex-M Interrupt Controller Initialization

#include <architecture/cpu.h>
#include <architecture/tsc.h>
#include <machine/ic.h>
#include <machine/timer.h>
#include <machine/usb.h>
#include <machine/gpio.h>

__BEGIN_SYS

IC::Interrupt_Handler IC::_eoi_vector[INTS];

void IC::init()
{
    db<Init, IC>(TRC) << "IC::init()" << endl;

    CPU::int_disable(); // will be reenabled at Thread::init()
    Engine::init();

    disable(); // will be enabled on demand as handlers are registered

    // Set all interrupt handlers to int_not()
    for(Interrupt_Id i = 0; i < INTS; i++)
        _int_vector[i] = int_not;

    // Initialize eoi vector (must be done at runtime because of .hex image format)
    for(unsigned int i = 0; i < INTS; i++)
        _eoi_vector[i] = 0;
    _eoi_vector[INT_USER_TIMER0] = User_Timer::eoi;
    _eoi_vector[INT_USER_TIMER1] = User_Timer::eoi;
    _eoi_vector[INT_USER_TIMER2] = User_Timer::eoi;
    _eoi_vector[INT_USER_TIMER3] = User_Timer::eoi;
#ifdef __IEEE802_15_4_H
    _eoi_vector[INT_NIC0_RX] = CC2538::eoi;
    _eoi_vector[INT_NIC0_TIMER] = CC2538RF::Timer::eoi;
#endif
#ifdef __USB_H
    _eoi_vector[INT_USB0] = USB::eoi;
#endif
    _eoi_vector[INT_GPIOA] = GPIO::eoi;
    _eoi_vector[INT_GPIOB] = GPIO::eoi;
    _eoi_vector[INT_GPIOC] = GPIO::eoi;
    _eoi_vector[INT_GPIOD] = GPIO::eoi;

    _int_vector[INT_HARD_FAULT] = hard_fault;

    // TSC is initialized before IC, so we register its interrupt now
    if(Traits<TSC>::enabled) {
        static const Interrupt_Id int_id =
            Traits<Timer>::UNITS == 1 ? INT_USER_TIMER0
          : Traits<Timer>::UNITS == 2 ? INT_USER_TIMER1
          : Traits<Timer>::UNITS == 3 ? INT_USER_TIMER2
                                      : INT_USER_TIMER3;

        int_vector(int_id, TSC::int_handler);
        enable(int_id);
    }
}

__END_SYS
