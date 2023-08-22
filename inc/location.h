#pragma once

namespace chisel {
struct Location {
    float x, y, z;
    int yaw, pitch;

    static Location create( float x, float y, float z, int yaw = 90, int pitch = 0 ) {
        return {x, y, z, yaw, pitch};
    }  
};
}