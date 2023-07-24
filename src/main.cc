#include <fstream>

#include "server.h"
#include "utils.h"

using chisel::Config;
using chisel::World;

void create_server_files() {
    if(!file_exists(CFG_DFNAME)) {
        Config::create_new();
    }
    
    if(!file_exists(OP_FILE)) {
        std::ofstream(OP_FILE).close();
    }
}

std::vector<std::string> idt_server_ops() {
    std::string              line; 
    std::ifstream            opf(OP_FILE);
    std::vector<std::string> ops;

    while(std::getline(opf, line)) {
        ops.push_back(line);
    }

    return ops;
}

int main(int argc, char** argv) {
    create_server_files();

    Config config = Config::from_file();
    
    chisel::Server server(
        &config, 
        idt_server_ops(), 
        file_exists(WRLD_DFNAME) ? World::from_file() : World::create_new()
    );
    server.start();

    while(server.is_running()) {
        std::string cname;
        std::cin >> cname;

        // TODO 22/7/23: Find server command and run.
        if(cname == "stop") server.stop();
    }
}