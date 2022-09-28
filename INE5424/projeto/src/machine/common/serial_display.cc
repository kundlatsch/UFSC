// EPOS Serial Display Mediator Implementation

#include <machine/display.h>

__BEGIN_SYS

// Class attributes
Serial_Display::Engine Serial_Display::_engine(UNIT);
int Serial_Display::_line;
int Serial_Display::_column;

__END_SYS
