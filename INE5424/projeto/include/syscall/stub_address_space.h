// EPOS Component Declarations

#ifndef __stub_address_space_h
#define __stub_address_space_h

#include <architecture.h>
#include <syscall/message.h>
#include <memory.h>

__BEGIN_API

__USING_UTIL

class Stub_Address_Space
{
private:
    int id;
    typedef _SYS::Message Message;
    // typedef Message::ENTITY::SEMAPHORE SEMAPHORE;


public:
    Stub_Address_Space(){}
    void set_id(int _id) {id = _id;}

    template<typename ... Tn>
    Stub_Address_Space(Tn ... an){
        Message * msg = new Message(Message::ENTITY::ADDRESS_SPACE, Message::ADDRESS_SPACE_CREATE);
        msg->act();
        id = msg->result();
    }

    template<typename ... Tn>
    Stub_Address_Space(_SYS::MMU::Page_Directory * pd, Tn ... an){
        Message * msg = new Message(Message::ENTITY::ADDRESS_SPACE, Message::ADDRESS_SPACE_CREATE_PD, pd);
        msg->act();
        id = msg->result();
    }

    _SYS::Address_Space::Log_Addr attach(_SYS::Segment * seg) {
        Message * msg = new Message(id, Message::ENTITY::ADDRESS_SPACE, Message::ADDRESS_SPACE_ATTACH1, seg);
        msg->act();
        return msg->result();
    }

    _SYS::Address_Space::Log_Addr attach(_SYS::Segment * seg, _SYS::Address_Space::Log_Addr addr) {
        Message * msg = new Message(id, Message::ENTITY::ADDRESS_SPACE, Message::ADDRESS_SPACE_ATTACH2, seg, addr);
        msg->act();
        return msg->result();
    }

    void detach(_SYS::Segment * seg) {
        Message * msg = new Message(id, Message::ENTITY::ADDRESS_SPACE, Message::ADDRESS_SPACE_DETACH1, seg);
        msg->act();
    }

    void detach(_SYS::Segment * seg, _SYS::Address_Space::Log_Addr addr) {
        Message * msg = new Message(id, Message::ENTITY::ADDRESS_SPACE, Message::ADDRESS_SPACE_DETACH2, seg, addr);
        msg->act();
    }

    _SYS::Address_Space::Phy_Addr physical(_SYS::Address_Space::Log_Addr addr) {
        Message * msg = new Message(id, Message::ENTITY::ADDRESS_SPACE, Message::ADDRESS_SPACE_PHYSICAL, addr);
        msg->act();
        return msg->result();
    }

};

__END_API

#endif