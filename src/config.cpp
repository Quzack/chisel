#include <iostream>
#include <sstream>

#include "config.hpp"
#include "server.hpp"

using chisel::Config;

using std::to_string;

std::string Config::params() const {
    std::string pubString = (pub) ? "True" : "False";

    return "?port="    + to_string(port)       + 
            "&max="    + to_string(maxPlayers) + 
            "&name="   + name                  + 
            "&public=" + pubString;
}

void Config::create_new() {
    std::ofstream file("config.txt");
    
    file << "port: " << to_string(chisel::DEFAULT_PORT) << std::endl;
    file << "max-players: 20"                           << std::endl;
    file << "name: MyServer"                            << std::endl;
    file << "public: false"                             << std::endl; 

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
        else if(line.find("public")      != -1) sin >> config.pub; 
    }

    return config;
}