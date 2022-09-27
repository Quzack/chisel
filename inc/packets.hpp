#pragma once

#include <string>

namespace chisel::packet {
    enum Packet {
        CS_AUTH,
        CS_SET_BLOCK,
        CS_POSITION,
        CS_MESSAGE,
        OTHER
    };

    Packet packetFromId( const int );

    namespace client {
        struct Identify {
            unsigned int protocolVer;
            std::string  username, key;
        };

        Identify identifyPlayer( const int );
    }

    char        readByte  ( const int );
    std::string readString( const int );
}