#pragma once

namespace chisel {
struct Location {
    int x, y, z, yaw, pitch;

    static Location create( int x, int y, int z ) {
        return {x, y, z, 90, 0};
    }  
};
}