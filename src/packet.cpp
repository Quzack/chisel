#include <winsock2.h>
#include <iostream>

#include "packet.hpp"

namespace chisel::packet {
Packet packet_from_id( const int id ) {
    switch(id) {
        case 0x00:
            return Packet::CS_AUTH;
        case 0x05:
            return Packet::CS_SET_BLOCK;
        case 0x08:
            return Packet::CS_POSITION;
        case 0x0d:
            return Packet::CS_MESSAGE;
        default:
            return Packet::UNKNOWN;
    }
}

namespace client {
    Identify identify( const chisel::sock::Server* socket ) {
        return {
            socket->read_byte(),
            socket->read_str (),
            socket->read_str (),
            socket->read_byte()
        };
    }
}
}