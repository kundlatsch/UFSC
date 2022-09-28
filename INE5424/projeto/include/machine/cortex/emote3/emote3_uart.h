// EPOS EPOSMoteIII (ARM Cortex-M3) UART Mediator Declarations

#ifndef __emote3_uart_h
#define __emote3_uart_h

#include <machine/uart.h>
#include <machine/gpio.h>
#include <machine/cortex/engine/pl011.h>
#include <machine/cortex/engine/pl061.h>
#include "emote3_sysctrl.h"
#include "emote3_ioctrl.h"
#include <system/memory_map.h>

__BEGIN_SYS

class UART_Engine: public UART_Common
{
private:
    static const unsigned int UNITS = Traits<UART>::UNITS;

public:
    UART_Engine(unsigned int unit, unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        assert(unit < UNITS);
        _unit = unit;
        _pl011 = new(reinterpret_cast<void *>(Memory_Map::UART0_BASE + 0x1000 * unit)) PL011;
        config(baud_rate, data_bits, parity, stop_bits);
    }

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        _pl011->disable();

        scr()->clock_uart(_unit);
        ioc()->enable_uart(_unit);

        _pl011->config(baud_rate, data_bits, parity, stop_bits);

        // Set GPIO pins to peripheral mode
        if(_unit == 0) {
            PL061 * pl061 = new(reinterpret_cast<void *>(Memory_Map::GPIOA_BASE)) PL061;
            pl061->select_pin_function(PL061::PIN0 | PL061::PIN1, PL061::FUN_ALTERNATE);
        } else {
            PL061 * pl061 = new(reinterpret_cast<void *>(Memory_Map::GPIOC_BASE)) PL061;
            pl061->select_pin_function(PL061::PIN3 | PL061::PIN4, PL061::FUN_ALTERNATE);
        }

        _pl011->enable();
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        _pl011->config(baud_rate, data_bits, parity, stop_bits);
    }

    char rxd() { return _pl011->rxd(); }
    void txd(char c) { return _pl011->txd(c); }

    bool rxd_ok() { return _pl011->rxd_ok(); }
    bool txd_ok() { return _pl011->txd_ok(); }

    bool rxd_full() { return _pl011->rxd_full(); }
    bool txd_empty() { return _pl011->txd_empty(); }

    void int_enable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        _pl011->int_enable(receive, transmit, line, modem);
    }
    void int_disable(bool receive = true, bool transmit = true, bool line = true, bool modem = true) {
        _pl011->int_disable(receive, transmit, line, modem);
    }

    void reset() { _pl011->reset(); }
    void loopback(bool flag) { _pl011->loopback(flag); }

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
            scr()->clock_uart(_unit);
            _pl011->enable();
            break;
        case SLEEP:
        case OFF:
            _pl011->disable();
            scr()->unclock_uart(_unit);
            break;
        }
    }

    static void init() {}

private:
    static SysCtrl * scr() { return reinterpret_cast<SysCtrl *>(Memory_Map::SCR_BASE); }
    static IOCtrl * ioc() { return reinterpret_cast<IOCtrl *>(Memory_Map::IOC_BASE); }

private:
    unsigned int _unit;
    PL011 * _pl011;
};

__END_SYS

#endif
