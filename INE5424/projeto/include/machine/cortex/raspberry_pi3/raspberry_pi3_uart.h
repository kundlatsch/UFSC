// EPOS Paspberry Pi3 (ARM Cortex-A53) UART Mediator Declarations

#ifndef __raspberry_pi3_uart_h
#define __raspberry_pi3_uart_h

#include <machine/uart.h>
#include <architecture/cpu.h>
#include <machine/cortex/engine/cortex_a53/bcm_uart.h>
#include <system/memory_map.h>

__BEGIN_SYS

class UART_Engine: public UART_Common
{
private:
    static const unsigned int UNITS = Traits<UART>::UNITS;

public:
    // The usage of GPIO_BASE is a peculiarity of the Raspberry Pi3B. The PL011 UART (UART_BASE) is not used in our port to EPOS,
    // as it is mapped by default to the Bluetooth of Raspberry Pi3B and not to the GPIO pins. 
    // Instead, we use the mini-UART. When intializing the mini-UART, first we need to configure the GPIO Function Select registers.
    // Those register's base is pointed by GPIO_BASE. After they are set-up, the mini-UART base is used to finish its configuration,
    // with the register's base at AUX_BASE (equal to GPIO_BASE + 0x15000). 
    // In this sense, the BCM_UART pointer is set to the GPIO_BASE, and the mini-UART base (AUX_BASE) is handled by adding 0x15000
    // to the offsets of each register at the engine implementation.
    UART_Engine(unsigned int unit, unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits)
    : _unit(unit), _uart(new(reinterpret_cast<void *>(CPU::sctlr() & CPU::MMU_ENABLE ? Memory_Map::GPIO_BASE : 0x3f200000)) BCM_UART) {
        assert(unit < UNITS);
        power(FULL);  // physically enable the UART in SysCtrl before configuring it
        config(baud_rate, data_bits, parity, stop_bits);
    }

    void config(unsigned int baud_rate, unsigned int data_bits, unsigned int parity, unsigned int stop_bits) {
        _uart->disable();
        _uart->config(baud_rate, data_bits, parity, stop_bits);
        _uart->enable();
    }

    void config(unsigned int * baud_rate, unsigned int * data_bits, unsigned int * parity, unsigned int * stop_bits) {
        _uart->config(baud_rate, data_bits, parity, stop_bits);
    }

    char rxd() { return _uart->rxd(); }
    void txd(char c) { return _uart->txd(c); }

    bool rxd_ok() { return _uart->rxd_ok(); }
    bool txd_ok() { return _uart->txd_ok(); }

    bool rxd_full() { return _uart->rxd_full(); }
    bool txd_empty() { return _uart->txd_empty(); }

    void int_enable(bool receive = true, bool send = true, bool line = true, bool modem = true) {
        _uart->int_enable(receive, send, line, modem);
    }
    void int_disable(bool receive = true, bool send = true, bool line = true, bool modem = true) {
        _uart->int_disable(receive, send, line, modem);
    }

    void reset() { _uart->reset(); }
    void loopback(bool flag) {} // there is no loopback support on miniUART

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
        case SLEEP:
            // TODO: clock_uart(mode)
            break;
        case OFF:
            break;
        }
    }

    static void init() {}

private:
    unsigned int _unit;
    BCM_UART * _uart;
};

__END_SYS

#endif
