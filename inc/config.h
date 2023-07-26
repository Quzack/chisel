#pragma once

#include <string>
#include <fstream>

#define CFG_DFNAME "config.cfg"

namespace chisel {    
struct Config {
    unsigned int port, maxPlayers;
    std::string name, motd;
    
    static void   create_new();
    static Config from_file ( std::ifstream = std::ifstream(CFG_DFNAME) );
};
}