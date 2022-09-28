// EPOS Component Declarations

#ifndef __stub_mutex_h
#define __stub_mutex_h

#include <architecture.h>
#include <syscall/message.h>

__BEGIN_API

__USING_UTIL

class Stub_Mutex
{
private:
    int id;
    typedef _SYS::Message Message;

public:
    Stub_Mutex(){
        Message * msg = new Message(Message::ENTITY::MUTEX, Message::MUTEX_CREATE);
        msg->act();
        id = msg->result();
    }

    void lock() {
        Message * msg = new Message(id, Message::ENTITY::MUTEX, Message::MUTEX_LOCK);
        msg->act();
    }

    void unlock() {
        Message * msg = new Message(id, Message::ENTITY::MUTEX, Message::MUTEX_UNLOCK);
        msg->act();
    }
};

class Stub_Mutex_Handler {
private:
    int id;
    typedef _SYS::Message Message;

public:
    // handler it self
    Stub_Mutex_Handler(){
        Message * msg = new Message(Message::ENTITY::MUTEX, Message::MUTEX_HANDLER);
        msg->act();
        id = msg->result();
    }

    /*
    void operator()() {
        Message * msg = new Message(handler, Message::ENTITY::MUTEX, Message::MUTEX_OPERATOR);
        msg->act();
    }
    */
};

__END_API

#endif