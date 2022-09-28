// EPOS Component Declarations

#ifndef __stub_condition_h
#define __stub_condition_h

#include <architecture.h>
#include <syscall/message.h>

__BEGIN_API

__USING_UTIL

class Stub_Condition
{
private:
    int id;
    int handler;
    typedef _SYS::Message Message;

public:
    template<typename ... Tn>
    Stub_Condition(Tn ... an){
        Message * msg = new Message(Message::ENTITY::CONDITION, Message::CONDITION_CREATE);
        msg->act();
        id = msg->result();
    }

    void wait(){
        Message * msg = new Message(id, Message::ENTITY::CONDITION, Message::CONDITION_WAIT);
        msg->act();
    }

    void signal(){
        Message * msg = new Message(id, Message::ENTITY::CONDITION, Message::CONDITION_SIGNAL);
        msg->act();
    }

    void broadcast(){
        Message * msg = new Message(id, Message::ENTITY::CONDITION, Message::CONDITION_BROADCAST);
        msg->act();
    }

    // handler
    void condition_handler(){
        Message * msg = new Message(id, Message::ENTITY::CONDITION, Message::CONDITION_HANDLER, id);
        msg->act();
        handler = msg->result();
    }

    void operator()(){
        Message * msg = new Message(id, Message::ENTITY::CONDITION, Message::CONDITION_OPERATOR, handler);
        msg->act();
    }
};

__END_API

#endif