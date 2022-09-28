// EPOS Component Declarations

#ifndef __stub_semaphore_h
#define __stub_semaphore_h

#include <architecture.h>
#include <syscall/message.h>

__BEGIN_API

__USING_UTIL

class Stub_Semaphore
{
private:
    int id;
    int handler;
    typedef _SYS::Message Message;

public:
    template<typename ... Tn>
    Stub_Semaphore(int v, Tn ... an){
        Message * msg = new Message(Message::ENTITY::SEMAPHORE, Message::SEMAPHORE_CREATE, v);
        msg->act();
        id = msg->result();
    }

    void p(){
        Message * msg = new Message(id, Message::ENTITY::SEMAPHORE, Message::SEMAPHORE_P);
        msg->act();
    }

    void v(){
        Message * msg = new Message(id, Message::ENTITY::SEMAPHORE, Message::SEMAPHORE_V);
        msg->act();
    }

    // handler
    void semaphore_handler(){
        Message * msg = new Message(id, Message::ENTITY::SEMAPHORE, Message::SEMAPHORE_HANDLER, id);
        msg->act();
        handler = msg->result();
    }

    void operator()(){
        Message * msg = new Message(id, Message::ENTITY::SEMAPHORE, Message::SEMAPHORE_OPERATOR, handler);
        msg->act();
    }
};

__END_API

#endif