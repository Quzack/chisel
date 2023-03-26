#pragma once

#include <vector>
#include <string>

#include "block.h"
#include "location.h"

namespace chisel {
struct World {
    uint16_t           length, width, height;
    Location           spawn;
    std::vector<Block> blocks;

    static World create_new(
        uint16_t len    = 100, 
        uint16_t width  = 100, 
        uint16_t height = 10
    ) {
        World world { 
            len, width, height, 
            { len/2, width/2, height/2 },
            std::vector<chisel::Block>(len * width * height, AIR)
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
};
}