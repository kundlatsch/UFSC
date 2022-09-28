// EPOS Component Declarations

#ifndef __stub_clock_h
#define __stub_clock_h

#include <architecture.h>
#include <machine/rtc.h>
#include <syscall/message.h>

__BEGIN_API

__USING_UTIL

class Stub_Clock
{
private:
    int id;
    typedef _SYS::Message Message;
    typedef Message::ENTITY::CLOCK CLOCK;
    typedef RTC::Date Date;

public:
    Stub_Clock(){
        Message * msg = new Message(CLOCK, Message::CLOCK_CREATE);
        msg->act();
        id = msg->result();
    }

    Microsecond resolution() {
        Message * msg = new Message(id, CLOCK, Message::CLOCK_RESOLUTION);
        msg->act();
        return msg->result();
    }

    Second now() {
        Message * msg = new Message(id, CLOCK, Message::CLOCK_NOW);
        msg->act();
        return msg->result();
    }

    Date date() {
        Message * msg = new Message(id, CLOCK, Message::CLOCK_DATE);
        msg->act();
        return msg->result();
    }

    void date() {
        Message * msg = new Message(id, CLOCK, Message::CLOCK_DATE1);
        msg->act();
    }
};

__END_API

#endif