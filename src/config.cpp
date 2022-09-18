#include "config.hpp"

using chisel::Config;

void Config::createDefault() {
    std::ofstream file("config.txt");
    
    file << "port: 25565"     << std::endl;
    file << "max-players: 20" << std::endl;
    file << "name: MyServer"  << std::endl;
    file << "public: false"   << std::endl; 

    file.close();
}

Config Config::fromFile( std::ifstream& file ) {

}