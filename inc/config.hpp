#pragma once

#include <string>
#include <fstream>

namespace chisel {
    struct Config {
        unsigned int port;
        unsigned int maxPlayers;
        std::string  name;
        bool         pub;
    };
    
    void createDefaultConfig     ();
    chisel::Config configFromFile( std::ifstream& );
}