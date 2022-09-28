// EPOS Component Declarations

#ifndef __stub_chronometer_h
#define __stub_chronometer_h

#include <architecture.h>
#include <syscall/message.h>

__BEGIN_API

__USING_UTIL

class Stub_Chronometer
{
private:
    int id;
    typedef _SYS::Message Message;
    // typedef Message::ENTITY::SEMAPHORE SEMAPHORE;

public:
    template<typename ... Tn>
    Stub_Chronometer(Tn ... an){
        Message * msg = new Message(Message::ENTITY::CHRONOMETER, Message::CHRONOMETER_CREATE);
        msg->act();
        id = msg->result();
    }

    Hertz frequency(){
        Message * msg = new Message(id, Message::ENTITY::CHRONOMETER, Message::CHRONOMETER_FREQUENCY);
        msg->act();
        return msg->result();
    }

    void reset() {
        Message * msg = new Message(id, Message::ENTITY::CHRONOMETER, Message::CHRONOMETER_RESET);
        msg->act();
    }

    void start() {
        Message * msg = new Message(id, Message::ENTITY::CHRONOMETER, Message::CHRONOMETER_START);
        msg->act();
    }

    void lap() {
        Message * msg = new Message(id, Message::ENTITY::CHRONOMETER, Message::CHRONOMETER_LAP);
        msg->act();
    }

    void stop() {
        Message * msg = new Message(id, Message::ENTITY::CHRONOMETER, Message::CHRONOMETER_STOP);
        msg->act();
    }

    Microsecond read() {
        Message * msg = new Message(id, Message::ENTITY::CHRONOMETER, Message::CHRONOMETER_READ);
        msg->act();
        return msg->result();
    }

};

__END_API

#endif