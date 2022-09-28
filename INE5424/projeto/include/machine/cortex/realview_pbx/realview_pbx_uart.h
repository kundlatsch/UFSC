// EPOS Realview PBX (ARM Cortex-A9) UART Mediator Declarations

#ifndef __realview_pbx_uart_h
#define __realview_pbx_uart_h

#include <machine/uart.h>
#include <machine/cortex/engine/pl011.h>
#include <system/memory_map.h>

__BEGIN_SYS

class UART_Engine: public UART_Common
{
private:
    static const unsigned int UNITS = Traits<UART>::UNITS;

public:
    UART_Engine(unsigned int unit, unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits): _unit(unit), _pl011(new(reinterpret_cast<void *>(Memory_Map::UART0_BASE + 0x1000 * unit)) PL011) {
        assert(unit < UNITS);
        config(baud_rate, data_bits, parity, stop_bits);
    }

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        _pl011->disable();
        _pl011->config(baud_rate, data_bits, parity, stop_bits);
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

    void power(const Power_Mode & mode) {}

    static void init() {}

private:
    unsigned int _unit;
    PL011 * _pl011;
};

__END_SYS

#endif
