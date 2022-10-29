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

    int send_to( const int ) const;
protected:
    void write_byte( const char );
    void write_str ( const std::string );
private:
    std::vector<char> _buffer;
};

namespace server {
struct Identify : public Packet {
    Identify( 
        const std::string&, 
        const std::string&,
        bool 
    );
};
}

namespace client {
struct Identify {
    unsigned int protocolVer;
    std::string  username, key;
    unsigned int unused;
};

Identify identify( const chisel::sock::Client& );
}
}