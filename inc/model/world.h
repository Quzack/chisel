#pragma once

#include <vector>
#include <string>

#include "network/socket.h"
#include "player.h"
#include "location.h"

namespace chisel {
const uint16_t CHUNK_LENGTH = 1024;

class World {
public:
    World( 
        uint16_t, uint16_t, uint16_t,
        Location,
        std::vector<char>
    );

    static World create_new( uint16_t, uint16_t, uint16_t );

    void spawn( const chisel::Player&, const sock::Client& ) const;
private:
    uint16_t          _length, _height, _width;
    Location          _spawn;
    std::vector<char> _blocks;

    void gen_flat_world();

    void   snd_world_data  ( const sock::Client& ) const;
    void   snd_chunk_data  ( const sock::Client& ) const;
    size_t block_idx_fr_pos( const Location )      const;
};
}