#include <winsock2.h>

#include "packets.hpp"

namespace chisel::packet {
    Packet packetFromId( const int id ) {
        switch(id) {
            case 0x00:
                return Packet::CsAuth;
            case 0x05:
                return Packet::CsSetBlock;
            case 0x08:
                return Packet::CsPosition;
            case 0x0d:
                return Packet::CsMessage;
            default:
                return Packet::Other;
        }
    }

    char readByte( const int fd ) {
        char byte;
        recv(fd, &byte, 1, 0x8);

        return byte;
    }

    std::string readString( const int fd ) {
        
    }
}