#pragma once

#include <string>
#include <fstream>

namespace chisel {
struct Config {
    unsigned int port;
    unsigned int maxPlayers;
    std::string  name;
    bool         pub;

    std::string params() const;

    static void   create_new();
    static Config from_file ( std::ifstream& );
};
}