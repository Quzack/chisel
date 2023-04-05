#pragma once

#include <string>
#include <vector>

#include "socket.h"

namespace chisel::packet {
const unsigned int PROTOCOL_VERSION = 0x07;
const unsigned int STR_BF_SZ        = 64;

class Packet {
public:
    Packet( const unsigned char );

    void write_byte  ( const unsigned char );
    void write_str   ( const std::string );
    void write_short ( const signed short int );
    void write_barray( const std::vector<char> );

    std::vector<char>& get_data() { return this->_buffer; }
private:
    std::vector<char> _buffer;
};

namespace client {
struct Identify {
    unsigned int protocolVer;
    std::string  username, key;
    unsigned int unused;
};

Identify identify( const chisel::sock::Client& );
}
}