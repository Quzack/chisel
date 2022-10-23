#pragma once

#include <string>
#include <fstream>
#include <sys/stat.h>

#include "utils.h"

#define LL_INFO 0
#define LL_WARN 1

namespace chisel::logger {
class Logger {
public:
    Logger ( std::string fileName ) {
        mkdir("logs");

        this->_file = std::ofstream("logs/" + fileName);
    }

    ~Logger() { _file.close(); }

    void log( const int l, std::string&& m ) {
        _file << "[" << current_date_time() << "] " << "(" << lv_prf(l) << "): " << m << std::endl;
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