#pragma once

#include <string>
#include <vector>

#include "socket.h"
#include "utils.h"
#include "location.h"
#include "player.h"

namespace chisel::packet {
const unsigned int PROTOCOL_VERSION = 0x07;
const unsigned int STR_BF_SZ        = 64;

class Packet {
public:
    Packet( unsigned char );

    void write_xyz   ( short, short, short );
    void write_loc   ( const Location ); // FSHORT
    void write_byte  ( unsigned char );
    void write_sbyte ( signed char );
    void write_str   ( const std::string );
    void write_short ( signed short int );
    void write_fshort( float );
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

inline Packet pos_packet( int8_t id, Location coord ) {
    packet::Packet  pSp(0x08);
    pSp.write_sbyte    (id);
    pSp.write_loc      (coord);

    return pSp;
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
    const Location coord;
};

struct Message {
    signed char pid; // -1
    std::string msg;
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
        {(float)sock.read_short(), (float)sock.read_short(), (float)sock.read_short()},
        sock.read_byte  (),
        sock.read_byte  ()
    };
}

inline SetPos id_set_pos( const chisel::sock::Client& sock ) {
    return {
        sock.read_sbyte(),
        { sock.read_fshort(), sock.read_fshort(), sock.read_fshort(), sock.read_byte(), sock.read_byte() }
    };
}

inline Message rd_msg_pck( const chisel::sock::Client& sock ) {
    return {
        sock.read_sbyte(),
        rtrim          (sock.read_str())
    };
}
}