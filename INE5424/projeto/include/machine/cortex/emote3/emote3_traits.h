// EPOS EPOSMoteIII (ARM Cortex-M3) Metainfo and Configuration

#ifndef __emote3_traits_h
#define __emote3_traits_h

#include <system/config.h>

__BEGIN_SYS

class Machine_Common;
template<> struct Traits<Machine_Common>: public Traits<Build>
{
    static const bool debugged = Traits<Build>::debugged;
};

template<> struct Traits<Machine>: public Traits<Machine_Common>
{
    static const bool cpus_use_local_timer      = false;

    static const unsigned int NOT_USED          = 0xffffffff;
    static const unsigned int CPUS              = Traits<Build>::CPUS;

    // Physical Memory
    static const unsigned int ROM_BASE          = 0x00200000;
    static const unsigned int ROM_TOP           = 0x0027ffff;   // 512 KB
    static const unsigned int RAM_BASE          = 0x20000000;
    static const unsigned int RAM_TOP           = 0x20007fff;   // 32 KB
    static const unsigned int MIO_BASE          = 0x40000000;
    static const unsigned int MIO_TOP           = 0x440067ff;
    static const unsigned int BOOT_LOADER       = ROM_BASE;
    static const unsigned int BOOT_LOADER_SIZE  = 16 * 1024;

    // Physical Memory at Boot
    static const unsigned int BOOT              = NOT_USED;
    static const unsigned int BOOT_STACK        = RAM_TOP - 4;  // RAM_TOP - sizeof(int)
    static const unsigned int IMAGE             = ROM_BASE;     // image on FLASH (max 512 KB)
    static const unsigned int SETUP             = NOT_USED;

    // Logical Memory Map (this machine only supports mode LIBRARY, so these are indeed also physical addresses)
    static const unsigned int APP_LOW           = RAM_BASE;
    static const unsigned int APP_HIGH          = RAM_TOP;

    static const unsigned int APP_CODE          = BOOT_LOADER + BOOT_LOADER_SIZE;
    static const unsigned int APP_DATA          = APP_LOW;

    static const unsigned int INIT              = NOT_USED;
    static const unsigned int PHY_MEM           = NOT_USED;
    static const unsigned int IO                = NOT_USED;
    static const unsigned int SYS               = NOT_USED;

    // Default Sizes and Quantities
    static const unsigned int STACK_SIZE        = 3 * 1024;
    static const unsigned int HEAP_SIZE         = 3 * 1024;
    static const unsigned int MAX_THREADS       = 7;
};

template<> struct Traits<IC>: public Traits<Machine_Common>
{
    static const bool debugged = hysterically_debugged;

    static const unsigned int IRQS = 48;
    static const unsigned int INTS = 65;
};

template<> struct Traits<Timer>: public Traits<Machine_Common>
{
    static const bool debugged = hysterically_debugged;

    static const unsigned int UNITS = 4; // GPTM

    // Meaningful values for the timer frequency range from 100 to
    // 10000 Hz. The choice must respect the scheduler time-slice, i. e.,
    // it must be higher than the scheduler invocation frequency.
    static const int FREQUENCY = 1000; // Hz
};

template<> struct Traits<UART>: public Traits<Machine_Common>
{
    static const unsigned int UNITS = 2;

    static const unsigned int CLOCK = Traits<CPU>::CLOCK;

    static const unsigned int DEF_UNIT = 0;
    static const unsigned int DEF_BAUD_RATE = 115200;
    static const unsigned int DEF_DATA_BITS = 8;
    static const unsigned int DEF_PARITY = 0; // none
    static const unsigned int DEF_STOP_BITS = 1;

    static const unsigned int CM1101_UNIT = 0;
};

template<> struct Traits<GPIO>: public Traits<Machine_Common>
{
    static const unsigned int UNITS = 4;
    static const bool supports_power_up = true;
};

template<> struct Traits<I2C>: public Traits<Machine_Common>
{
    static const unsigned int UNITS = 1;

    static const unsigned int LSM330_UNIT = 0;
    static const unsigned int SI7020_UNIT = 0;

    template<unsigned int UNIT>
    struct Config {};
};
template<> struct Traits<I2C>::Config<0>
{
    static const unsigned char SDA_PORT = 'B';
    static const unsigned char SDA_PIN = 1;
    static const unsigned char SCL_PORT = 'B';
    static const unsigned char SCL_PIN = 0;
};

template<> struct Traits<SPI>: public Traits<Machine_Common>
{
    static const unsigned int UNITS = 1;
};

template<> struct Traits<USB>: public Traits<Machine_Common>
{
    // Some observed objects are created before initializing the Display, which may use the USB.
    // Enabling debug may cause trouble in some Machines
    static const bool debugged = false;

    static const bool wait_to_sync = true;
    static const unsigned int UNITS = 1;
    static const bool blocking = false;
    static const bool enabled = true;
};

template<> struct Traits<Watchdog>: public Traits<Machine_Common>
{
    static const unsigned int PERIOD = 1000; // ms
};

template<> struct Traits<Serial_Display>: public Traits<Machine_Common>
{
    static const bool enabled = true;
    static const unsigned int ENGINE = USB;
    static const unsigned int UNIT = 0;
    static const unsigned int COLUMNS = 80;
    static const unsigned int LINES = 24;
    static const unsigned int TAB_SIZE = 8;
};

template<> struct Traits<Serial_Keyboard>: public Traits<Machine_Common>
{
    static const bool enabled = true;
};

template<> struct Traits<Scratchpad>: public Traits<Machine_Common>
{
    static const bool enabled = false;
};

template<> struct Traits<IEEE802_15_4>: public Traits<Machine_Common>
{
    // NICS that don't have a network in Traits<Network>::NETWORKS will not be enabled
    typedef LIST<IEEE802_15_4_NIC> DEVICES;
    static const unsigned int UNITS = DEVICES::Length;

    static const bool enabled = (Traits<Build>::NODES > 1) && (UNITS > 0);
};

template<> struct Traits<IEEE802_15_4_NIC>: public Traits<IEEE802_15_4>
{
    static const unsigned int UNITS = Traits<IEEE802_15_4>::DEVICES::Count<IEEE802_15_4_NIC>::Result;
    static const unsigned int RECEIVE_BUFFERS = 20; // per unit
    static const bool gpio_debug = false;
    static const bool reset_backdoor = false;
    static const unsigned int DEFAULT_CHANNEL = 13;

    static const bool tstp_mac = true;
    static const bool promiscuous = false;
    static const bool enabled = (Traits<Build>::NODES > 1) && (UNITS > 0);
};

template<> struct Traits<Modem>: public Traits<Machine_Common>
{
    // NICS that don't have a network in Traits<Network>::NETWORKS will not be enabled
    typedef LIST<M95> DEVICES;
    static const unsigned int UNITS = DEVICES::Length;

    static const bool enabled = (Traits<Build>::NODES > 1) && (UNITS > 0);
};

template<> struct Traits<M95>: public Traits<Machine_Common>
{
    static const unsigned int UNITS = Traits<Modem>::DEVICES::Count<M95>::Result;

    enum {CLARO, TIM, OI};
    static const unsigned int PROVIDER = CLARO;

    static const unsigned int UART_UNIT = 1;
    static const unsigned int UART_BAUD_RATE = 9600;
    static const unsigned int UART_DATA_BITS = 8;
    static const unsigned int UART_PARITY = 0;
    static const unsigned int UART_STOP_BITS = 1;

    static const char PWRKEY_PORT = 'C';
    static const unsigned int PWRKEY_PIN = 4;
    static const char STATUS_PORT = 'C';
    static const unsigned int STATUS_PIN = 1;

    static const bool enabled = (Traits<Build>::NODES > 1) && (UNITS > 0);

    static const bool promiscuous = false;
};

__END_SYS

#endif
