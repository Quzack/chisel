#pragma once

#include <string>

#include "config.hpp"
#include "logger.hpp"

namespace chisel {
class Heart {
public:
    Heart( 
        const std::string& configParams, 
        const std::string& salt, 
        const int          playerCount
    );
private:
    std::string _url;

    void start() const;
};
}