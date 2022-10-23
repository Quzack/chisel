#include <fstream>
#include <iostream>

#include "server.h"
#include "utils.h"

using chisel::Config;

#define OP_FILE "op-list.txt"

void create_files() {
    if(!file_exists("config.txt")) {
        Config::create_new();
    }
    
    if(!file_exists(OP_FILE)) {
        std::ofstream(OP_FILE).close();
    }
}

std::vector<std::string> read_ops() {
    std::string              line; 
    std::ifstream            opf(OP_FILE);
    std::vector<std::string> ops;

    while(std::getline(opf, line)) {
        ops.push_back(line);
    }

    return ops;
}

int main(int argc, char** argv) {
    create_files();

    std::ifstream configFile("config.txt");
    Config config = Config::from_file(configFile);

    auto ops = read_ops();

    chisel::server::Server server(&config, &ops);
    server.start();
}