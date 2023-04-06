#include "model/world.h"
#include "model/block.h"
#include "gzip/compress.h"
#include "network/packet.h"

using chisel::World;

World::World( 
    uint16_t l, uint16_t h, uint16_t w, 
    Location spawn, 
    std::vector<char> blocks
): 
    _length(l),_height(h),_width(w),
    _spawn (spawn),
    _blocks(blocks)
{

}

World World::create_new( uint16_t l, uint16_t h, uint16_t w ) {
    World world(l, h, w, { l/2, h/2, w/2 }, std::vector<char>(l*w*h, AIR));
    world.gen_flat_world();

    return world;
} 

void World::join( const chisel::Player& player ) const {
    player.socket().send_pckt({0x02}); // LEVEL INIT.
    snd_chunk_data(player.socket());   // CHUNK DATA.

    packet::Packet finalize(0x04);
    finalize.write_short(_length);
    finalize.write_short(_height);
    finalize.write_short(_width);

    player.socket().send_pckt(finalize.get_data()); // LEVEL FINALIZE.
    // TODO 5/4/2023: Spawn player.
    packet::Packet spawnPk(0x07);
    spawnPk.write_sbyte   (player.id());
    spawnPk.write_str     (player.name);
    
}

void World::gen_flat_world() {
    for(int y = 0; y < _height/2; y++) {
        for(int x = 0; x < _length; x++) {
            for(int z = 0; z < _width; z++) {
                auto id = block_idx_fr_pos({x, y, z});

                _blocks[id] = (y < ((_height/2) -1)) ? DIRT : GRASS;
            }
        }
    }
}

void World::snd_chunk_data( const sock::Client& client ) const {
    std::vector<char> apBlocks(_blocks.size() + 1);
    apBlocks.push_back((uint32_t) _blocks.size());
    apBlocks.insert(apBlocks.begin() + 1, _blocks.begin(), _blocks.end());

    std::string gdata = gzip::compress(&apBlocks[0], apBlocks.size());

    bool dRem = gdata.size() % CHUNK_LENGTH == 0;
    int i = dRem ? dRem/CHUNK_LENGTH : (dRem - (dRem % CHUNK_LENGTH))/CHUNK_LENGTH;

    for(int j = 0; j < i; j++) {
        std::string data = gdata.substr(j * CHUNK_LENGTH, CHUNK_LENGTH + (j * CHUNK_LENGTH));

        packet::Packet chunk(0x03);
        chunk.write_short   (CHUNK_LENGTH);
        chunk.write_barray  (std::vector<char>(data.begin(), data.end()));
        chunk.write_byte    (dRem && j == i ? 100 : 0);

        client.send_pckt(chunk.get_data());
    }

    if(dRem) return;
    packet::Packet chunk(0x03);
    chunk.write_short   (dRem);
    chunk.write_barray  (std::vector<char>(gdata.begin() - dRem, gdata.end()));
    chunk.write_byte    (100);

    client.send_pckt(chunk.get_data());
}

size_t World::block_idx_fr_pos( Location pos ) const {
    return (unsigned int) (pos.x + this->_width * (pos.z + this->_length * pos.y));
}