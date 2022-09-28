// EPOS Component Declarations

#ifndef __stub_segment_h
#define __stub_segment_h

#include <architecture.h>
#include <syscall/message.h>
#include <memory.h>

__BEGIN_API

__USING_UTIL

class Stub_Segment
{
private:
    int id;
    typedef _SYS::Message Message;

public:
    Stub_Segment(){}
    void set_id(int _id) {id = _id;}

    Stub_Segment(unsigned int bytes, _SYS::MMU::Flags flags = _SYS::MMU::Flags::APP){
        Message * msg = new Message(Message::ENTITY::SEGMENT, Message::SEGMENT_CREATE, bytes, flags);
        msg->act();
        id = msg->result();
    }

    Stub_Segment(_SYS::Segment::Phy_Addr phy_addr, unsigned int bytes, _SYS::MMU::Flags flags = _SYS::MMU::Flags::APP){
        Message * msg = new Message(Message::ENTITY::SEGMENT, Message::SEGMENT_CREATE_PHY, phy_addr, bytes, flags);
        msg->act();
        id = msg->result();
    }

    unsigned int size () {
        Message * msg = new Message(id, Message::ENTITY::SEGMENT, Message::SEGMENT_SIZE);
        msg->act();
        int r = msg->result();
        return reinterpret_cast<int>(r);
    }

    _SYS::Segment::Phy_Addr phy_address() {
        Message * msg = new Message(id, Message::ENTITY::SEGMENT, Message::SEGMENT_PHY_ADDRESS);
        msg->act();
        int r = msg->result();
        //return reinterpret_cast<Segment::Phy_Addr>(r);
        return r;
    }

    int resize (int amount) {
        Message * msg = new Message(id, Message::ENTITY::SEGMENT, Message::SEGMENT_RESIZE, amount);
        msg->act();
        int r = msg->result();
        return r;
    }
};

__END_API

#endif