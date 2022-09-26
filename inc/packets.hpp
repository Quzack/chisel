#pragma once

#include <string>

namespace chisel::packet {
    enum Packet {
        CsAuth,
        CsSetBlock,
        CsPosition,
        CsMessage,
        Other
    };

    Packet packetFromId( const int id );

    namespace client {
        struct Auth {
            unsigned int protocolVer;
            std::string  username;
            std::string  key;
        };

        Auth identifyPlayer( const int fd );
    }

    char        readByte  ( const int fd );
    std::string readString( const int fd );
}