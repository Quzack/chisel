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
    void write_sbyte ( const signed char );
    void write_str   ( const std::string );
    void write_short ( const signed short int );
    void write_fshort( const signed short );
    void write_barray( std::vector<char> );

    std::vector<char>& get_data() { return this->_buffer; }
private:
    std::vector<char> _buffer;
};

struct Identify {
    char protocolVer;
    std::string  username, key;
    char unused;
};

Identify identify_cl( const chisel::sock::Client& );
}