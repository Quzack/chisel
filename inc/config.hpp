#pragma once

#include <string>
#include <fstream>

namespace chisel {
    struct Config {
        unsigned int port;
        unsigned int maxPlayers;
        std::string  name;
        bool         pub;

        std::string asParams() const;
    };
    
    void createDefaultConfig     ();
    chisel::Config configFromFile( std::ifstream& );
}