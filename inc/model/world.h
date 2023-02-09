#pragma once

#include <vector>
#include <string>

#include "block.h"
#include "location.h"

namespace chisel {
struct World {
    int16_t            length, width, height;
    Location           spawn;
    std::vector<Block> blocks;

    static World create_new(
        int16_t len, 
        int16_t width, 
        int16_t height
    ) {
        World world { 
            len, width, height, 
            { len/2, width/2, height/2 },
            std::vector((u_int)(len * width * height), AIR)
        };

        // TODO 9/2/2023: Flat map generation.

        return world;
    }
};
}