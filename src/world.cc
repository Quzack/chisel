#include <cstring>
#include <fstream>
#include <sstream>

#include "world.h"
#include "gzip/compress.h"
#include "gzip/decompress.h"
#include "network/packet.h"

using chisel::World;

const uint16_t CHUNK_LENGTH = 1024;

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

World World::from_file( const std::ifstream file ) {
    std::stringstream fdstream;
    fdstream << file.rdbuf();

    std::string wdata = gzip::decompress(&fdstream.str()[0], fdstream.str().size());
    if(wdata.empty()) return World::create_new();

    uint16_t l, h, w;
    std::memcpy(&l, &wdata[0], 2);
    std::memcpy(&h, &wdata[2], 2);
    std::memcpy(&w, &wdata[4], 2);

    return World(
        l, h, w,
        Location::create(l/2, (h/2)+1, w/2),
        std::vector<char>(wdata.begin() + 6, wdata.end())
    );
}

World World::create_new( uint16_t l, uint16_t h, uint16_t w ) {
    World world(
        l, h, w, 
        Location::create(l/2, (h/2)+1, w/2), 
        std::vector<char>(l*w*h, 0x00)
    );
    world.gen_flat_world();

    return world;
} 

bool World::set_block( const Location coord, const char block ) {
    if(coord.x < 0 || coord.y < 0 || coord.z < 0) return false;
    this->_blocks[block_idx_fr_pos({coord.x, coord.y, coord.z})] = block;
    return true;
}

void World::spawn( chisel::Player& player ) const {
    snd_world_data(player.socket());
    packet::send_spawn_pckt(player, _spawn, player.socket());

    player.set_pos(_spawn);
}

void World::save_tf( const std::string fname ) const {
    std::vector<char> data(_blocks.size() + 6); // 2*3 bytes -> length height breadth
    std::memcpy(data.data()    , &_length, 2);
    std::memcpy(data.data() + 2, &_height, 2);
    std::memcpy(data.data() + 4, &_width , 2);
    std::memcpy(data.data() + 6, _blocks.data(), _blocks.size());

    std::string cdata = gzip::compress(&data[0], data.size());

    std::ofstream file(fname);
    file << cdata << std::endl;

    file.close();
}

void World::gen_flat_world() {
    for(int y = 0; y < _height/2; y++) {
        for(int x = 0; x < _length; x++) {
            for(int z = 0; z < _width; z++) {
                set_block({x, y, z}, (y < ((_height/2) -1)) ? 0x03 : 0x02);
            }
        }
    }
}

void World::snd_world_data( const sock::Client& client ) const {
    client.send_pckt({0x02}); // LEVEL INIT.
    snd_chunk_data(client);   // CHUNK DATA.

    packet::Packet finalize(0x04);
    finalize.write_xyz(_length, _height, _width);

    client.send_pckt(finalize.get_data()); // LEVEL FINALIZE.
}

// plus sized disgusting function.
void World::snd_chunk_data( const sock::Client& client ) const {
    uint32_t blockLength = htonl(static_cast<uint32_t>(_blocks.size()));
    std::vector<char> apBlocks(_blocks.size() + 4);
    std::memcpy(apBlocks.data(), &blockLength, 4);
    std::memcpy(apBlocks.data() + 4, _blocks.data(), _blocks.size());

    std::string gData = gzip::compress(&apBlocks[0], apBlocks.size());

    int dRem = gData.size() % CHUNK_LENGTH;
    int i = (dRem == 0) ? gData.size()/CHUNK_LENGTH : (gData.size() - dRem)/CHUNK_LENGTH;
    short finalBlocks = gData.size();

    for(int j = 0; j < i; j++) {
        std::string data = gData.substr(j * CHUNK_LENGTH, CHUNK_LENGTH + (j * CHUNK_LENGTH));

        packet::Packet chunk(0x03);
        chunk.write_short   (CHUNK_LENGTH);
        chunk.write_barray  (std::vector<char>(data.begin(), data.end()));
        chunk.write_byte    (dRem == 0 && j == i ? 100 : 0);

        client.send_pckt(chunk.get_data());
        finalBlocks -= (j+1) * CHUNK_LENGTH;
    }

    if(dRem == 0) return;
    packet::Packet chunk(0x03);
    chunk.write_short   ((i == 0) ? gData.size() : finalBlocks);
    std::vector<char> chunkData;
    chunkData.insert(chunkData.begin(), (i == 0) ? gData.begin() : gData.end() - finalBlocks, gData.end());

    chunk.write_barray(chunkData);
    chunk.write_byte  (100);

    client.send_pckt(chunk.get_data());
}

size_t World::block_idx_fr_pos( Location pos ) const {
    return (unsigned int) (pos.x + this->_width * (pos.z + this->_length * pos.y));
}