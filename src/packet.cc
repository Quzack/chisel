#include <winsock2.h>

#include "utils.h"
#include "packet.h"

namespace chisel::packet {
Packet::Packet( unsigned char id ) {
    write_byte(id);
}

void Packet::write_xyz( short x, short y, short z ) {
    write_short(x);
    write_short(y);
    write_short(z);
}

void Packet::write_loc( const Location loc ) {
    write_fshort(loc.x);
    write_fshort(loc.y);
    write_fshort(loc.z);
    write_byte  (loc.yaw);
    write_byte  (loc.pitch);
}

void Packet::write_byte( unsigned char byte ) {
    _buffer.push_back(byte);
}

void Packet::write_sbyte( signed char byte ) {
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

void Packet::write_short( signed short int i ) {
    signed short int value = htons(i);
    _buffer.insert(_buffer.end(), (char*)&value, (char*)&value + sizeof(signed short int));
}

void Packet::write_fshort( float value ) {
    write_short(value * 32.0);
}

void Packet::write_barray( std::vector<char> data ) {
    for(auto& byte : data) {
        write_byte(byte);
    }

    for(int i = 0; i < 1024 - data.size(); i++) {
        write_byte(0x00);
    }
} 
}
