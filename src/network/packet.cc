#include <winsock2.h>
#include <iostream>

#include "utils.h"
#include "network/packet.h"

namespace chisel::packet {
Packet::Packet( const unsigned char id ) {
    write_byte(id);
}

void Packet::write_byte( const unsigned char byte ) {
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

void Packet::write_short( const signed short int i ) {
    _buffer.push_back((signed short int) i);
}

void Packet::write_barray( const std::vector<char> array ) {
    for(int i = 0; i < 1024; i++) {
        write_byte(array[i]);
    }
} 

Identify identify_cl( const chisel::sock::Client& socket ) {
    return {
        socket.read_byte(),
        remove_empty(socket.read_str()),
        remove_empty(socket.read_str()),
        socket.read_byte()
    };
}
}