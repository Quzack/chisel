#include <fstream>
#include <iostream>

#include "server.h"
#include "utils.h"

using chisel::Config;

int main(int argc, char** argv) {
    if(!file_exists("config.txt")) {
        Config::create_new();
    }

    std::ifstream configFile("config.txt");
    Config config = Config::from_file(configFile);
    
    chisel::Server server(&config);
    server.start();

    // TODO 28/9/22: Console messages.
}