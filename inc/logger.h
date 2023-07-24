#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "utils.h"

#define LL_INFO 0
#define LL_WARN 1

namespace chisel {
class Logger {
public:
    Logger ( std::string fileName ) {
        mkdir("logs");

        this->_file = std::ofstream("logs/" + fileName);
    }

    ~Logger() { _file.close(); }

    void log( const int l, std::string m, bool console = true ) {
        if(console) {
            std::cout << "[" << fmt_time("%d.%X")  << ":" << lv_prf(l) << "]: " << m << std::endl;
        }
        _file << "[" << fmt_time("%Y-%m-%d.%X") << "] " << "(" << lv_prf(l) << "): " << m << std::endl;
    }
private:
    std::ofstream _file;

    std::string lv_prf( const int l ) const {
        switch(l) {
            case 0:  return "INFO";
            case 1:  return "WARN";
            default: return "NONE";
        }
    }
};
}