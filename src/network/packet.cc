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

void Packet::write_sbyte( const signed char byte ) {
    _buffer.push_back(static_cast<char>(byte));
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
    signed short int value = htons(i);
    _buffer.insert(_buffer.end(), (char*)&value, (char*)&value + sizeof(signed short int));
}

void Packet::write_fshort( const signed short value ) {
    signed short fvalue = htons(value);
    fvalue *= 32;
    _buffer.insert(_buffer.end(), (char*)&fvalue, (char*)&fvalue + sizeof(signed short));
}

void Packet::write_barray( std::vector<char> data ) {
    for(auto& byte : data) {
        write_byte(byte);
    }

    for(int i = 0; i < 1024 - data.size(); i++) {
        write_byte(0x00);
    }
} 

Identify identify_cl( const chisel::sock::Client& socket ) {
    return {
        socket.read_byte(),
        socket.read_str (),
        socket.read_str (),
        socket.read_byte()
    };
}
}