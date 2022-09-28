// EPOS ARMv7 Time-Stamp Counter Mediator Implementation

#include <architecture/tsc.h>

__BEGIN_SYS

#if defined(__mmod_emote3__) || defined(__mmod_lm3s811__)

volatile TSC::Time_Stamp TSC::_overflow = 0;

#endif

__END_SYS
