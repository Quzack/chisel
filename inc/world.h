#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "socket.h"
#include "player.h"
#include "location.h"

#define WRLD_DFNAME "world.mcw"

namespace chisel {
class World {
public:
    World( 
        uint16_t, uint16_t, uint16_t,
        Location,
        std::vector<char>
    );

    static World from_file ( const std::ifstream = std::ifstream(WRLD_DFNAME) );
    static World create_new( 
        uint16_t = 100, 
        uint16_t = 100, 
        uint16_t = 100 
    );

    bool set_block( const Location coord, const char block );

    void spawn  ( chisel::Player& )                 const;
    void save_tf( const std::string = WRLD_DFNAME ) const; // to file.
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