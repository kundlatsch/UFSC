// EPOS PC PS2_Keyboard Mediator Implementation

#include <system/config.h>

#ifdef __KEYBOARD_H

#include <machine/keyboard.h>

__BEGIN_SYS

// Class attributes
unsigned int PS2_Keyboard::_status;

PS2_Keyboard::Scancode PS2_Keyboard::_scancodes[]= {{   0,    0,   0,   0}, // 00
                                                    {  27,   27,   0,   0}, // 01
                                                    { '1',  '!',   0,   0}, // 02
                                                    { '2',  '@',   0,   0}, // 03
                                                    { '3',  '#',   0,   0}, // 04
                                                    { '4',  '$',   0,   0}, // 05
                                                    { '5',  '%',   0,   0}, // 06
                                                    { '6',  '^',   0,   0}, // 07
                                                    { '7',  '&',   0,   0}, // 08
                                                    { '8',  '*',   0,   0}, // 09
                                                    { '9',  '(',   0,   0}, // 10
                                                    { '0',  ')',   0,   0}, // 11
                                                    { '-',  '_',   0,   0}, // 12
                                                    { '=',  '+',   0,   0}, // 13
                                                    {   8,    8,   0,   0}, // 14
                                                    {   9,    9,   0,   0}, // 15
                                                    { 'q',  'Q',   0,   0}, // 16
                                                    { 'w',  'W',   0,   0}, // 17
                                                    { 'e',  'E',   0,   0}, // 18
                                                    { 'r',  'R',   0,   0}, // 19
                                                    { 't',  'T',   0,   0}, // 20
                                                    { 'y',  'Y',   0,   0}, // 21
                                                    { 'u',  'U',   0,   0}, // 22
                                                    { 'i',  'I',   0,   0}, // 23
                                                    { 'o',  'O',   0,   0}, // 24
                                                    { 'p',  'P',   0,   0}, // 25
                                                    { '[',  '{',   0,   0}, // 26
                                                    { ']',  '}',   0,   0}, // 27
                                                    {'\n', '\n',   0,   0}, // 28
                                                    { 200,  200,   0,   0}, // 29 - CTRL
                                                    { 'a',  'A',   0,   0}, // 30
                                                    { 's',  'S',   0,   0}, // 31
                                                    { 'd',  'D',   0,   0}, // 32
                                                    { 'f',  'F',   0,   0}, // 33
                                                    { 'g',  'G',   0,   0}, // 34
                                                    { 'h',  'H',   0,   0}, // 35
                                                    { 'j',  'J',   0,   0}, // 36
                                                    { 'k',  'K',   0,   0}, // 37
                                                    { 'l',  'L',   0,   0}, // 38
                                                    { ';',  ':',   0,   0}, // 39
                                                    { ' ',  '"',   0,   0}, // 40
                                                    { '`',  '~',   0,   0}, // 41
                                                    { 202,  202,   0,   0}, // 42 - SHIFT
                                                    {'\\',  '|',   0,   0}, // 43
                                                    { 'z',  'Z',   0,   0}, // 44
                                                    { 'x',  'X',   0,   0}, // 45
                                                    { 'c',  'C',   0,   0}, // 46
                                                    { 'v',  'V',   0,   0}, // 47
                                                    { 'b',  'B',   0,   0}, // 48
                                                    { 'n',  'N',   0,   0}, // 49
                                                    { 'm',  'M',   0,   0}, // 50
                                                    { ',',  '<',   0,   0}, // 51
                                                    { '.',  '>',   0,   0}, // 52
                                                    { '/',  '?',   0,   0}, // 53
                                                    { 202,  202,   0,   0}, // 54 - SHIFT
                                                    { ' ',  ' ',   0,   0}, // 55
                                                    { ALT,  ALT,   0,   0}, // 56
                                                    { ' ',  ' ',   0,   0}, // 57 - SPACE
                                                    { 203,  203,   0,   0}, // 58 - CAPS
                                                    { 204,  204,   0,   0}, // 59 - F1
                                                    { 205,  205,   0,   0}, // 60 - F2
                                                    { 206,  206,   0,   0}};// 61 - F3
Observed PS2_Keyboard::_observed;

// Methods
char PS2_Keyboard::get()
{
    char c = map(scancode());
    while(c == -1)
        c = map(scancode());
    return c;
}

void PS2_Keyboard::int_handler(IC::Interrupt_Id i)
{
    db<Keyboard>(TRC) << "Keyboard::int_handler(int=" << i << ")" << endl;

    if(!notify())
        data(); // it is necessary to clear the output buffer before a new interrupt can happen!
}

char PS2_Keyboard::map(int code)
{
    char c = 0;

    switch(code) {
    case LCTRL                 : _status |=  CONTROL; c = -1; break;
    case LCTRL  | BREAK        : _status &= ~CONTROL; c = -1; break;
    case LSHIFT                :
    case RSHIFT                : _status |=  SHIFT;   c = -1; break;
    case RSHIFT | BREAK        :
    case LSHIFT | BREAK        : _status &= ~SHIFT;   c = -1; break;
    case LALT                  : _status |=  ALT;     c = -1; break;
    case LALT   | BREAK        : _status &= ~ALT;     c = -1; break;
    case LCAPS                 : _status ^=  CAPS;    c = -1; Engine::leds((_status & CAPS) ? Engine::CAPS : 0); break;
    case LCAPS  | BREAK        :                      c = -1; break;
    }

    if(c != -1) {
        if(_status & CONTROL)    c = _scancodes[code].ctrl;
        else if(_status & SHIFT) c = _scancodes[code].shift;
        else if(_status & ALT)   c = _scancodes[code].alt;
        else                     c = _scancodes[code].normal;

        if(_status & CAPS)       c = upper(c);
    }

    db<Keyboard>(TRC) << "Keyboard::macp(code=" << hex << code << dec << ") => " << int(c) << endl;

    return c;
 }

__END_SYS

#endif
