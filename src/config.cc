#include <sstream>

#include "config.h"

using chisel::Config;

void Config::create_new() {
    std::ofstream file("config.cfg");
    
    file << "port: " << 25565           << std::endl;
    file << "max-players: 20"           << std::endl;
    file << "name: GenericName"         << std::endl;
    file << "motd: A Minecraft server." << std::endl; 

    file.close();
}

Config Config::from_file( std::ifstream& file ) {
    Config      config;
    std::string line;

    while(std::getline(file, line)) {
        std::istringstream sin(line.substr(line.find(":" ) + 1));

        if     (line.find("port")        != -1) sin >> config.port;
        else if(line.find("max-players") != -1) sin >> config.maxPlayers;
        else if(line.find("name")        != -1) sin >> config.name;
        else if(line.find("motd")        != -1) sin >> config.motd;
    }

    return config;
}