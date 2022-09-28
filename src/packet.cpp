#include <winsock2.h>

#include "packet.hpp"

namespace chisel::packet {
Packet packet_from_id( const int id ) {
    switch(id) {
        case 0x00:
            return Packet::PlayerIdentify;
            break;
        case 0x05:
            return Packet::SetBlock;
        case 0x08:
            return Packet::Position;
        case 0x0d:
            return Packet::Message;
        default:
            return Packet::Unknown;
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