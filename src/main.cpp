#include <string>
#include <fstream>

#include "server.hpp"

bool fileExists(const char* name) {
    FILE* file;
    if(file = fopen(name, "r")) {
        fclose(file);
        return true;
    }

    return false;
}

int main(int argc, char** argv) {
    if(!fileExists("config.txt")) chisel::createDefaultConfig();

    std::ifstream configFile("config.txt");
    chisel::Config config = chisel::configFromFile(configFile);

    chisel::Server server(&config);
    server.start();
}