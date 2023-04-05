#pragma once

#include <vector>
#include <string>

#include "block.h"
#include "location.h"
#include "network/packet.h"
#include "gzip/compress.h"

namespace chisel {
const uint16_t CHUNK_LENGTH = 1024;

struct World {
    uint16_t           length, width, height;
    Location           spawn;
    std::vector<char>  blocks;

    static World create_new(
        uint16_t len    = 100, 
        uint16_t width  = 100, 
        uint16_t height = 100
    ) {
        World world { 
            len, width, height, 
            { len/2, width/2, height/2 },
            std::vector<char>(len * width * height, AIR)
        };

        world.gen_flat_wrld();

        return world;
    }

    static World fr_file( const char* fname ) {
        // TODO 26/3/2023: Load from file.
    }

    void gen_flat_wrld() {
        for(int y = 0; y < height/2; y++) {
            for(int x = 0; x < width; x++) {
                for(int z = 0; z < length; z++) {
                    auto id = block_idx_fr_pos({x, y, z});

                    if(y < ((height/2) - 1)) {
                        blocks[id] = DIRT;
                        continue;
                    }

                    blocks[id] = GRASS;
                }
            }
        }
    }

    size_t block_idx_fr_pos( Location coord ) {
        return (unsigned int) (coord.x + this->width * (coord.z + this->length * coord.y));
    }

    void join( const sock::Client& client ) const {
        client.send_pckt({0x02}); // LEVEL INIT.
        send_chunk_d(client);     // CHUNK DATA.

        packet::Packet finalize(0x04);
        finalize.write_short(length);
        finalize.write_short(height);
        finalize.write_short(width);

        client.send_pckt(finalize.get_data()); // LEVEL FINALIZE.
        // TODO 5/4/2023: Spawn player.
    }

    void send_chunk_d( const sock::Client& client ) const {
        std::vector<char> apBlocks(blocks.size() + 1);
        apBlocks.push_back((uint32_t) blocks.size());
        apBlocks.insert(apBlocks.begin() + 1, blocks.begin(), blocks.end());

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
};
}