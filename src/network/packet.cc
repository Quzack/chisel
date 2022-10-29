#include <winsock2.h>
#include <iostream>

#include "utils.h"
#include "network/packet.h"

namespace chisel::packet {
Packet::Packet( const unsigned char id ) {
    write_byte(id);
}

int Packet::send_to( const int fd ) const {
    return send(fd, &_buffer[0], _buffer.size(), 0);
}

void Packet::write_byte( const char byte ) {
    _buffer.push_back(byte);
}

void Packet::write_str( const std::string str ) {
    const int len      = str.length();
    const int spaceLen = STR_BF_SZ - len;

    for(int i = 0; i < len; i++) {
        write_byte(str[i]);
    }

    for(int i = 0; i < spaceLen; i++) {
        write_byte(0x20);
    }
}

namespace server {
Identify::Identify( 
    const std::string& name,
    const std::string& motd,
    bool op 
):
    Packet(0x00)
{
    write_byte(PROTOCOL_VERSION);
    write_str (name);
    write_str (motd);
    write_byte(op ? 0x64 : 0x00);
}
}

namespace client {
Identify identify( const chisel::sock::Client& socket ) {
    return {
        socket.read_byte(),
        remove_empty(socket.read_str()),
        remove_empty(socket.read_str()),
        socket.read_byte()
    };
}
}
}