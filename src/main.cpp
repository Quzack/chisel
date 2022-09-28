#include <fstream>
#include <iostream>

#include "server.hpp"

using chisel::Config;

bool file_exists(const char* name) {
    FILE* file;
    if(file = fopen(name, "r")) {
        fclose(file);
        return true;
    }

    return false;
}

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