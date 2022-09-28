// EPOS PC Keyboard Mediator Initialization

#include <system/config.h>

#ifdef __KEYBOARD_H

#include <machine/ic.h>
#include <machine/keyboard.h>

__BEGIN_SYS

void PS2_Keyboard::init()
{
    db<Init, Keyboard>(TRC) << "Keyboard::init()" << endl;

    Engine::disable();

    IC::int_vector(IC::INT_KEYBOARD, int_handler);
    IC::enable(IC::INT_KEYBOARD);

    Engine::flush();
    Engine::int_enable();

    Engine::enable();
    Engine::leds(0);
}

__END_SYS

#endif
