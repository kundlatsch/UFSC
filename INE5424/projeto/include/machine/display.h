// EPOS Display Mediator Common Package

#ifndef __display_h
#define __display_h

#include <system/config.h>

#if defined(__UART_H) && !defined(__uart_common_only__)
#include __UART_H
#endif

#if defined(__USB_H) && !defined(__usb_common_only__)
#include __USB_H
#endif

__BEGIN_SYS

class Display_Common
{
protected:
    Display_Common() {}

public:
    static void putc(char c);
    static void puts(const char * s);

    static void clear();

    static void geometry(int * lines, int * columns);
    static void position(int * line, int * column);
    static void position(int line, int column);
};

class Serial_Display: public Display_Common
{
    friend class Setup;
    friend class Serial_Keyboard;
    friend class Machine;

private:
    typedef IF<Traits<Serial_Display>::ENGINE == Traits<Serial_Display>::UART, UART, USB>::Result Engine;
    static const int UNIT = Traits<Serial_Display>::UNIT;
    static const int LINES = Traits<Serial_Display>::LINES;
    static const int COLUMNS = Traits<Serial_Display>::COLUMNS;
    static const int TAB_SIZE = Traits<Serial_Display>::TAB_SIZE;

    // Special characters
    enum {
        ESC  = 0x1b,
        CR   = 0x0d,
        LF   = 0x0a,
        TAB  = 0x09,
    };

public:
    Serial_Display() {}

    static void clear() {
        _line = 0;
        _column = 0;
        escape();
        put('2');
        put('J');
    };

    static void putc(char c) {
        switch(c) {
        case '\n':
            scroll();
            _line++;
            break;
        case '\t':
            put(TAB);
            _column += TAB_SIZE;
            break;
        default:
            _column++;
            put(c);
            if(_column >= COLUMNS) scroll();
        }
    };

    static void puts(const char * s) {
        while(*s != '\0')
            putc(*s++);
    }

    static void geometry(int * lines, int * columns) {
        *lines = LINES;
        *columns = COLUMNS;
    }

    static void position(int * line, int * column) {
        *line = _line;
        *column = _column;
    }
    static void position(int line, int column) {
        _line = line;
        _column = column;
        escape();
        puti(_line);
        put(';');
        puti(_column);
        put('H');
    }

private:
    static void put(char c) {
        _engine.put(c);
    }

    static void escape() {
        put(ESC);
        put('[');
    }

    static void puti(unsigned char value) {
        unsigned char digit = '0';
        while(value >= 100) {
            digit++;
            value -= 100;
        }
        put(digit);

        digit = '0';
        while(value >= 10) {
            digit++;
            value -= 10;
        }
        put(digit);

        put('0' + value);
    }

    static void scroll() {
        put(CR);
        put(LF);
        _column = 0;
    }

    static void init() {
        // Display must be on very early in the boot process, so it is
        // subject to memory remappings. Renewing it cares for it.
        new (&_engine) Engine(UNIT);

        _line = 0;
        _column = 0;
    }

private:
    static Engine _engine;
    static int _line;
    static int _column;
};

__END_SYS

// If the machine does not feature a Keyboard, then use the serial console
#ifndef __DISPLAY_H
__BEGIN_SYS
class Display: public IF<Traits<Serial_Display>::enabled, Serial_Display, Dummy<>>::Result {};
__END_SYS
#endif

#endif

#if defined(__DISPLAY_H) && !defined(__display_common_only__)
#include __DISPLAY_H
#endif
