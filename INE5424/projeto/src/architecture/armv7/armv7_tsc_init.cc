// EPOS ARMv7 Time-Stamp Counter Mediator Initialization


#include <architecture/tsc.h>
#include <machine/timer.h>

#if defined(__cortex_a__)

__BEGIN_SYS

void TSC::init()
{
    db<Init, TSC>(TRC) << "TSC::init()" << endl;


    if(CPU::id() == 0) {
        // Disable counting before programming
        reg(GTCLR) = 0;

        // Set timer to 0
        reg(GTCTRL) = 0;
        reg(GTCTRH) = 0;

        // Re-enable counting
        reg(GTCLR) = 1;
    }
}

__END_SYS

#elif defined(__cortex_m__)

#include __HEADER_MMOD(sysctrl)
#include __HEADER_MMOD(memory_map)

__BEGIN_SYS

void TSC::init()
{
    db<Init, TSC>(TRC) << "TSC::init()" << endl;

    SysCtrl * scr = reinterpret_cast<SysCtrl *>(Memory_Map::SCR_BASE);
    GPTM * gptm = reinterpret_cast<GPTM *>(Memory_Map::TIMER0_BASE + 0x1000 * (Traits<Timer>::UNITS - 1));

    scr->clock_timer(Traits<Timer>::UNITS - 1);
    gptm->config(0xffffffff, true, (Traits<Build>::MODEL == Traits<Build>::LM3S811) ? false : true);

    // time-out interrupt will be registered later at IC::init(), because IC hasn't been initialized yet
}

__END_SYS

#endif
