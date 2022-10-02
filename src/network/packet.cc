#include <winsock2.h>
#include <iostream>

#include "network/packet.h"

namespace chisel::packet {
Packet packet_from_id( const int id ) {
    switch(id) {
        case 0x00: return CS_AUTH;
        case 0x05: return CS_SET_BLOCK;
        case 0x08: return CS_POSITION;
        case 0x0d: return CS_MESSAGE;
        default:   return UNKNOWN;
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