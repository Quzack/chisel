#include <string>
#include <fstream>

#include "config.hpp"
#include "server.hpp"

using chisel::Config;
using chisel::Server;

bool fileExists(const char* name) {
    FILE* file;
    if(file = fopen(name, "r")) {
        fclose(file);
        return true;
    }

    return false;
}

int main(int argc, char** argv) {
    if(!fileExists("config.txt")) {
        Config::createDefault();
    }
}