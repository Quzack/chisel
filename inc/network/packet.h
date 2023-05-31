#pragma once

#include <string>
#include <vector>

#include "socket.h"
#include "player.h"
#include "utils.h"
#include "location.h"

namespace chisel::packet {
const unsigned int PROTOCOL_VERSION = 0x07;
const unsigned int STR_BF_SZ        = 64;

class Packet {
public:
    Packet( const unsigned char );

    void write_xyz   ( const short, const short, const short );
    void write_loc   ( const Location ); // FSHORT

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

inline void send_spawn_pckt( 
    const chisel::Player& p, 
    const Location loc, 
    const chisel::sock::Client& cl
) {
    packet::Packet spawnPk(0x07);
    spawnPk.write_sbyte   ((p.socket() == cl) ? -1 : p.id());
    spawnPk.write_str     (p.name);
    spawnPk.write_loc     (loc); 

    cl.send_pckt(spawnPk.get_data());
}

struct Identify {
    char        protocolVer;
    std::string username, key;
    char        unused;
};

struct SetBlock {
    Location coord;
    char     mode; // Created or destroyed.
    char     block;
};

struct SetPos {     // 0x08
    signed char id; // -1
    Location coord;
};

inline Identify identify_cl( const chisel::sock::Client& sock ) {
    return {
        sock.read_byte(),
        rem_empty     (sock.read_str()),
        sock.read_str (),
        sock.read_byte()
    };
}

inline SetBlock id_set_blck( const chisel::sock::Client& sock ) {
    return {
        {sock.read_short(), sock.read_short(), sock.read_short()},
        sock.read_byte  (),
        sock.read_byte  ()
    };
}

inline SetPos id_set_pos( const chisel::sock::Client& sock ) {
    return {
        sock.read_sbyte(),
        { sock.read_fshort(), sock.read_fshort(), sock.read_fshort(), sock.read_byte(), sock.read_byte()}
    };
}
}