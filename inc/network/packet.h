#pragma once

#include <string>
#include <vector>

#include "socket.h"

namespace chisel::packet {
const unsigned int PROTOCOL_VERSION = 0x07;

enum Packet {
    CS_AUTH,
    CS_SET_BLOCK,
    CS_POSITION,
    CS_MESSAGE,
    UNKNOWN
};

Packet packet_from_id( const int );

namespace client {
struct Identify {
    unsigned int protocolVer;
    std::string  username, key;
    unsigned int unused;
};

Identify identify( const chisel::sock::Client& );
}
}