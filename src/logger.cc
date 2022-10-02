#include <iostream>
#include <sys/stat.h>

#include "logger.h"
#include "utils.h"

using chisel::logger::Logger;

std::string chisel::logger::lvl_prefix( LoggerLevel level ) {
    switch(level) {
        case LL_INFO: return "INFO";
        case LL_WARN: return "WARN";
    }
}

Logger::Logger( std::string fileName ) {
    mkdir("logs");

    this->_file = std::ofstream("logs/" + fileName);
}

Logger::~Logger() {
    _file.close();
}

void Logger::log( LoggerLevel level, std::string&& msg ) {
    _file << "[" << current_date_time() << "] " << "(" << lvl_prefix(level) << "): " << msg << std::endl;
}