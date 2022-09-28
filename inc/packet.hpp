#pragma once

#include <string>
#include <vector>

#include "socket.hpp"

namespace chisel::packet {
const unsigned int PROTOCOL_VERSION = 0x07;

enum Packet {
    PlayerIdentify,
    SetBlock,
    Position,
    Message,
    Unknown
};

Packet packet_from_id( const int );

namespace client {
struct Identify {
    unsigned int protocolVer;
    std::string  username, key;
    unsigned int unused;
};

Identify identify( const chisel::sock::Server* );
}
}