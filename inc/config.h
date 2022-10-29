#pragma once

#include <string>
#include <fstream>

namespace chisel {
struct Config {
    unsigned int port, maxPlayers;
    std::string name, motd;
    
    static void   create_new();
    static Config from_file ( std::ifstream& );
};
}