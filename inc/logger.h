#pragma once

#include <string>
#include <fstream>
#include <sys/stat.h>

#include "utils.h"

namespace chisel::logger {
enum LoggerLevel {
    LL_INFO,
    LL_WARN
};

class Logger {
public:
    Logger ( std::string fileName ) {
        mkdir("logs");

        this->_file = std::ofstream("logs/" + fileName);
    }

    ~Logger() { _file.close(); }

    void log( LoggerLevel l, std::string&& m ) {
        _file << "[" << current_date_time() << "] " << "(" << lv_prf(l) << "): " << m << std::endl;
    }
private:
    std::ofstream _file;

    std::string lv_prf( LoggerLevel l ) const {
        switch(l) {
            case LL_INFO: return "INFO";
            case LL_WARN: return "WARN";
        }
    }
};
}