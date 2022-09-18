#include <iostream>
#include <sstream>

#include "config.hpp"

void chisel::createDefaultConfig() {
    std::ofstream file("config.txt");
    
    file << "port: 25565"     << std::endl;
    file << "max-players: 20" << std::endl;
    file << "name: MyServer"  << std::endl;
    file << "public: false"   << std::endl; 

    file.close();
}

chisel::Config chisel::configFromFile( std::ifstream& file ) {
    chisel::Config config;
    std::string line;

    while(std::getline(file, line)) {
        std::istringstream sin(line.substr(line.find(":" ) + 1));

        if     (line.find("port")        != -1) sin >> config.port;
        else if(line.find("max-players") != -1) sin >> config.maxPlayers;
        else if(line.find("name")        != -1) sin >> config.name;
        else if(line.find("public")      != -1) sin >> config.pub; 
    }

    return config;
}