// EPOS PC Machine Metainfo and Configuration

#ifndef __legacy_pc_traits_h
#define __legacy_pc_traits_h

#include <system/config.h>

__BEGIN_SYS

class Machine_Common;
template<> struct Traits<Machine_Common>: public Traits<Build> {};

template<> struct Traits<Machine>: public Traits<Machine_Common>
{
    static const bool cpus_use_local_timer      = false;

    static const unsigned int NOT_USED          = 0xffffffff;
    static const unsigned int CPUS              = Traits<Build>::CPUS;

    // Physical Memory
    static const unsigned int RAM_BASE          = 0x00000000;
    static const unsigned int RAM_TOP           = 0x10000000; 	// 256 MB (max 1792 MB)
    static const unsigned int MIO_BASE          = NOT_USED;	// defined by SETUP during PCI initialization (max 244 MB)
    static const unsigned int MIO_TOP           = NOT_USED;	// defined by SETUP

    // Physical Memory at Boot
    static const unsigned int BOOT              = 0x00007c00;
    static const unsigned int BOOT_STACK        = NOT_USED;     // defined by BOOT and by SETUP
    static const unsigned int IMAGE             = 0x00008000;
    static const unsigned int SETUP             = 0x00100000;   // 1 MB
    static const unsigned int RAMDISK           = 0x0fa28000;   // MEMDISK-dependent
    static const unsigned int RAMDISK_SIZE      = 0x003c0000;

    // Logical Memory Map
    static const unsigned int APP_LOW           = 0x00000000;
    static const unsigned int APP_HIGH          = 0xefffffff;   // IO - 1

    static const unsigned int APP_CODE          = APP_LOW;
    static const unsigned int APP_DATA          = APP_CODE + 4 * 1024 * 1024;

    static const unsigned int INIT              = 0x00200000;   // 2 MB (only used during boot)
    static const unsigned int PHY_MEM           = 0x80000000; 	// 2 GB (max 1792 MB)
    static const unsigned int IO                = 0xf0000000; 	// 4 GB - 256 MB  (max 244 MB)
    static const unsigned int SYS               = 0xff800000;   // 4 GB - 8 MB

    // Default Sizes and Quantities
    static const unsigned int STACK_SIZE        = 16 * 1024;
    static const unsigned int HEAP_SIZE         = 4 * 1024 * 1024;
    static const unsigned int MAX_THREADS       = 16;
};

template<> struct Traits<PCI>: public Traits<Machine_Common>
{
    static const int MAX_BUS = 16;
    static const int MAX_DEV_FN = 0xff;
    static const unsigned int MAX_REGION_SIZE = 0x04000000; // 64 MB
};

template<> struct Traits<IC>: public Traits<Machine_Common>
{
    static const bool debugged = hysterically_debugged;
};

template<> struct Traits<Timer>: public Traits<Machine_Common>
{
    static const bool debugged = hysterically_debugged;

    // Meaningful values for the PC's timer frequency range from 100 to
    // 10000 Hz. The choice must respect the scheduler time-slice, i. e.,
    // it must be higher than the scheduler invocation frequency.
    static const int FREQUENCY = 1000; // Hz
};

template<> struct Traits<RTC>: public Traits<Machine_Common>
{
    static const unsigned int EPOCH_DAY = 1;
    static const unsigned int EPOCH_MONTH = 1;
    static const unsigned int EPOCH_YEAR = 1970;
    static const unsigned int EPOCH_DAYS = 719499;
};

template<> struct Traits<EEPROM>: public Traits<Machine_Common>
{
};

template<> struct Traits<UART>: public Traits<Machine_Common>
{
    static const unsigned int UNITS = 2;

    static const unsigned int CLOCK = 1843200; // 1.8432 MHz

    static const unsigned int DEF_BAUD_RATE = 115200;
    static const unsigned int DEF_DATA_BITS = 8;
    static const unsigned int DEF_PARITY = 0; // none
    static const unsigned int DEF_STOP_BITS = 1;

    static const unsigned int COM1 = 0x3f8; // to 0x3ff, IRQ4
    static const unsigned int COM2 = 0x2f8; // to 0x2ff, IRQ3
    static const unsigned int COM3 = 0x3e8; // to 0x3ef, no IRQ
    static const unsigned int COM4 = 0x2e8; // to 0x2ef, no IRQ
};

template<> struct Traits<Serial_Display>: public Traits<Machine_Common>
{
    static const bool enabled = (Traits<Build>::EXPECTED_SIMULATION_TIME != 0);
    static const int ENGINE = UART;
    static const int UNIT = 0; // COM1
    static const int COLUMNS = 80;
    static const int LINES = 24;
    static const int TAB_SIZE = 8;
};

template<> struct Traits<Serial_Keyboard>: public Traits<Machine_Common>
{
    static const bool enabled = (Traits<Build>::EXPECTED_SIMULATION_TIME != 0);
};

template<> struct Traits<Display>: public Traits<Machine_Common>
{
    static const bool enabled = !Traits<Serial_Display>::enabled;
    static const int COLUMNS = 80;
    static const int LINES = 25;
    static const int TAB_SIZE = 8;
};

template<> struct Traits<Keyboard>: public Traits<Machine_Common>
{
    static const bool enabled = !Traits<Serial_Keyboard>::enabled;
};

template<> struct Traits<Scratchpad>: public Traits<Machine_Common>
{
    static const bool enabled = true;
    static const unsigned int ADDRESS = 0xa0000; // VGA Graphic mode frame buffer
    static const unsigned int SIZE = 96 * 1024;
};

template<> struct Traits<FPGA>: public Traits<Machine_Common>
{
    static const bool enabled = false;

    static const unsigned int DMA_BUFFER_SIZE = 64 * 1024; // 64 KB
};

__END_SYS

#endif
