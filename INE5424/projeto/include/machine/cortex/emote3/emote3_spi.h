// EPOS EPOSMoteIII (ARM Cortex-M3) SPI Mediator Declarations

#ifndef __emote3_spi_h
#define __emote3_spi_h

#include <architecture/cpu.h>
#include <machine/spi.h>
#include <machine/cortex/engine/pl022.h>
#include <machine/cortex/engine/pl061.h>
#include "emote3_sysctrl.h"
#include "emote3_ioctrl.h"
#include <system/memory_map.h>

__BEGIN_SYS

class SPI_Engine: public SPI_Common
{
private:
    static const unsigned int UNITS = Traits<SPI>::UNITS;

    typedef CPU::Reg32 Reg32;

public:
    SPI_Engine(unsigned int unit, unsigned int clock, const Protocol & protocol, const Mode & mode, unsigned int bit_rate, unsigned int data_bits): _unit(unit) {
        assert(unit < UNITS);
        _pl022 = new(reinterpret_cast<void *>(Memory_Map::SSI0_BASE + 0x1000 * unit)) PL022;
        config(clock, protocol, mode, bit_rate, data_bits);
    }

    void config(unsigned int clock, const Protocol & protocol, const Mode & mode, unsigned int bit_rate, unsigned int data_bits) {
        _pl022->disable();

        scr()->clock_spi(_unit);
        ioc()->enable_spi(_unit, (mode == MASTER));

        _pl022->config(clock, protocol, mode, bit_rate, data_bits);

        // Set GPIO pins A2, A3, A4 and A5 to peripheral mode
        PL061 * pl061 = new(reinterpret_cast<void *>(Memory_Map::GPIOA_BASE)) PL061;
        pl061->select_pin_function(PL061::PIN2 | PL061::PIN3 | PL061::PIN4 | PL061::PIN5, PL061::FUN_ALTERNATE);

        _pl022->enable();
    }

    int get() { return _pl022->get(); }
    bool try_get(int * data) { return _pl022->try_get(reinterpret_cast<Reg32 *>(data)); }
    void put(int data) { _pl022->put(data); }
    bool try_put(int data) { return _pl022->try_put(data); }

    void flush() { while(_pl022->busy()); }
    bool ready_to_get() { return _pl022->ready_to_get(); }
    bool ready_to_put() { return _pl022->ready_to_put(); }

    void int_enable(bool receive = true, bool transmit = true, bool time_out = true, bool overrun = true) {
        _pl022->int_enable((receive ? PL022::RXIM : 0) | (transmit ? PL022::TXIM : 0) | (time_out ? PL022::RTIM : 0) | (overrun ? PL022::RORIM : 0));
    }
    void int_disable(bool receive = true, bool transmit = true, bool time_out = true, bool overrun = true) {
        int_enable(!receive, !transmit, !time_out, !overrun);
    }

    void power(const Power_Mode & mode) {
        switch(mode) {
        case ENROLL:
            break;
        case DISMISS:
            break;
        case SAME:
            break;
        case FULL:
        case LIGHT:
            scr()->clock_spi(_unit);
            _pl022->enable();
            break;
        case SLEEP:
        case OFF:
            _pl022->disable();
            scr()->unclock_spi(_unit);
            break;
        }
    }

    static void init() {}

private:
    static SysCtrl * scr() { return reinterpret_cast<SysCtrl *>(Memory_Map::SCR_BASE); }
    static IOCtrl * ioc() { return reinterpret_cast<IOCtrl *>(Memory_Map::IOC_BASE); }

private:
    unsigned int _unit;
    PL022 * _pl022;
};

__END_SYS

#endif
