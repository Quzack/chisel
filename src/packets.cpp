#include <winsock2.h>

#include "packets.hpp"

namespace chisel::packet {
    Packet packetFromId( const int id ) {
        switch(id) {
            case 0x00:
                return CS_AUTH;
            case 0x05:
                return CS_SET_BLOCK;
            case 0x08:
                return CS_POSITION;
            case 0x0d:
                return CS_MESSAGE;
            default:
                return OTHER;
        }
    }

    namespace client {
        
    }

    char readByte( const int fd ) {
        char byte;
        recv(fd, &byte, 1, 0x8);

        return byte;
    }
}