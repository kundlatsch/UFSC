// EPOS PC Keyboard Mediator Declarations

#ifndef __pc_keyboard_h
#define __pc_keyboard_h

#include <utility/observer.h>
#include <architecture/cpu.h>
#include <machine/ic.h>
#include <machine/keyboard.h>

__BEGIN_SYS

// Intel 8042 Keyboard Controller
class i8042
{
public:
    // I/O ports
    enum {
        DATA            = 0x60,
        STATUS		= 0x64,
        COMMAND         = 0x64 // same as STATUS, but for write operations
    };

    // Status register flags
    enum {
        OUT_BUF_FULL    = 0x01, // The byte in the output buffer hasn't been received yet
        IN_BUF_FULL     = 0x02, // A byte is ready in the input buffer
        IGNORE          = 0x04, // Ignored bit
        PORT            = 0x08, // 0 => PORT = 0x64, 1 => PORT = 0x60
        ACTIVE          = 0x10, // Keyboard is active
        OUT_TIME_OUT    = 0x20, // Error: time out when outputting
        IN_TIME_OUT     = 0x40, // Error: time out when inputing
        PARITY          = 0x80  // Error: parity don't match
    };

    // Commands
    typedef unsigned char Command;
    enum {
        READ_CFG        = 0x20,
        WRITE_CFG       = 0x60,
        DISABLE_C2      = 0xa7,
        ENABLE_C2       = 0xa8,
        DISABLE_C1      = 0xad,
        ENABLE_C1       = 0xae,
        SET_LEDS        = 0xed,
        SET_RATE        = 0xf3,
        ACK             = 0xfa,
        REBOOT          = 0xfe
    };

    // LEDs
    enum {
        SCROLL          = 0x1,
        NUM             = 0x2,
        CAPS            = 0x4
    };

public:
    i8042() {}

    static int status() { return(CPU::in8(STATUS)); }

    static bool command(const Command & cmd) {
        // Wait for the controller's input buffer to get empty
        for(int i = 0; (i < 300) && (status() & IN_BUF_FULL); i++);
        if((status() & IN_BUF_FULL)) return false;

        // Send command
        CPU::out8(COMMAND, cmd);

        // Wait for a keyboard controller reaction
        for(int i = 0; (i < 300) && !(status() & OUT_BUF_FULL); i++);
        if(!(status() & OUT_BUF_FULL)) return false;

        // Check for an ACK
        return (data() == ACK);
    }

    static int data() { return(CPU::in8(DATA)); }

    static int scancode() {
        while(!(status() & OUT_BUF_FULL));
        return data();
    }

    static bool leds(unsigned char leds = (SCROLL | NUM | CAPS)) {
        return (command(SET_LEDS) && command(leds));
    }

    static int rate(int delay, int rate) {
        return (command(SET_RATE) && command(((delay << 5) | rate) & 0xff));
    }

    static void enable() {
        command(ENABLE_C1);
        command(ENABLE_C2);
    }
    static void disable() {
        command(DISABLE_C1);
        command(DISABLE_C2);
    }

    static void int_enable(bool press = true, bool release = true) {
        command(READ_CFG);
        unsigned char cfg = data() | 1;
        command(WRITE_CFG) && command(cfg);
    }
    static void int_disable(bool press = true, bool release = true) {
        command(READ_CFG);
        unsigned char cfg = data() & 0xfe;
        command(WRITE_CFG) && command(cfg);
    }

    static void flush() {
        // Wait for the controller to get reeady
        while(status() & OUT_BUF_FULL)
            data(); // Discard a byte
    }
};

class PS2_Keyboard: public Keyboard_Common, private i8042
{
    friend class Machine;

private:
    typedef i8042 Engine;

    struct Scancode {
       unsigned char normal;
       unsigned char shift;
       unsigned char ctrl;
       unsigned char alt;
    };

    // Keyboard Status
    enum {
        CONTROL = 0x01,
        SHIFT   = 0x02,
        ALT     = 0x04,
        CAPS    = 0x08
    };

    // Keyboard Special Keys
    enum {
        BREAK   = 0x80,
        ENTER   = 0x1c,
        LSHIFT  = 0x2a,
        RSHIFT  = 0x36,
        LCTRL   = 0x1d,
        LALT    = 0x38,
        LCAPS   = 0x3a,
        SCROL   = 0x46,
        NUM     = 0x45
    };

public:
    typedef _UTIL::Observer Observer;
    typedef _UTIL::Observed Observed;

public:
    PS2_Keyboard() {}

    static char get();
    static bool ready_to_get() { return (status() & OUT_BUF_FULL); }

    using Engine::int_enable;
    using Engine::int_disable;

    static void attach(Observer * obs) { _observed.attach(obs); }
    static void detach(Observer * obs) { _observed.detach(obs); }

private:
    static char upper(char c) { return ((c >= 'a') && (c <= 'z')) ? (c -'a' + 'A') : c; }
    static char map(int code);

    static bool notify() { return _observed.notify(); }

    static void int_handler(IC::Interrupt_Id i);

    static void reboot();

    static void init();

private:
    static unsigned int _status;
    static Scancode _scancodes[255];

    static Observed _observed;
};

// i8042 is used even if the machine is configured without a keyboard (e.g. leds, reboot)
#ifdef __KEYBOARD_H

class Keyboard: public IF<Traits<Serial_Keyboard>::enabled, Serial_Keyboard, PS2_Keyboard>::Result {};

#endif

__END_SYS

#endif
