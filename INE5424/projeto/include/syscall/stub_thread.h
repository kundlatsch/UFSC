// EPOS Component Declarations

#ifndef __stub_thread_h
#define __stub_thread_h

#include <architecture.h>
#include <syscall/message.h>
#include <syscall/stub_task.h>

__BEGIN_API

__USING_UTIL

class Stub_Thread
{
private:
    int id;
    typedef _SYS::Message Message;

public:
    Stub_Thread(){}

    template<typename ... Tn>
    Stub_Thread(int (* entry)(Tn ...), Tn ... an){
        Message * msg = new Message(Message::ENTITY::THREAD, Message::THREAD_CREATE, entry);
        msg->act();
        id = msg->result();
    }

    /*
    template<typename ... Tn>
    Stub_Thread(const Configuration & conf, int (* entry)(Tn ...), Tn ... an){
        Message * msg = new Message(THREAD, Message::THREAD_CREATE, conf, entry);
        msg->act();
        id = msg->result();
    }
    */

    void set_id(int _id){id = _id;};

    /*
    void priority(const Criterion & p) {
        Message * msg = new Message(id, Message::ENTITY::THREAD, Message::THREAD_PRIORITY1, p);
        msg->act();
    }
    */

    Stub_Task * task() {
        Message * msg = new Message(id, Message::ENTITY::THREAD, Message::THREAD_TASK);
        msg->act();
        int id = msg->result();
        Stub_Task * new_stub_task = new Stub_Task();
        new_stub_task->set_id(id);
        return new_stub_task;
    }

    int join() {
        Message * msg = new Message(id, Message::ENTITY::THREAD, Message::THREAD_JOIN);
        msg->act();
        return msg->result();
    }

    void pass() {
        Message * msg = new Message(id, Message::ENTITY::THREAD, Message::THREAD_PASS);
        msg->act();
    }

    void suspend() {
        Message * msg = new Message(id, Message::ENTITY::THREAD, Message::THREAD_SUSPEND);
        msg->act();
    }

    void resume() {
        Message * msg = new Message(id, Message::ENTITY::THREAD, Message::THREAD_RESUME);
        msg->act();
    }

    static Stub_Thread * self() {
        Message * msg = new Message(Message::ENTITY::THREAD, Message::THREAD_SELF);
        msg->act();
        int id = msg->result();
        Stub_Thread * new_stub_thread = new Stub_Thread();
        new_stub_thread->set_id(id);
        return new_stub_thread;
    }

    static void yield() {
        Message * msg = new Message(Message::ENTITY::THREAD, Message::THREAD_YIELD);
        msg->act();
    }

    static void exit(int status = 0) {
        Message * msg = new Message(Message::ENTITY::THREAD, Message::THREAD_EXIT, status);
        msg->act();
    }
};

__END_API

#endif