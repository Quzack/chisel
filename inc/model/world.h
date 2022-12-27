#pragma once

#include <vector>

#include "block.h"

namespace chisel {
struct World {
    std::vector<Block> blocks;
};
}