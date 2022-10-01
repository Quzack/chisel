#pragma once

#include <string>
#include <fstream>

namespace chisel::logger {
enum LoggerLevel {
    LL_INFO,
    LL_WARN
};

std::string lvl_prefix( LoggerLevel );

class Logger {
public:
    Logger ( std::string );
    ~Logger();

    void log( LoggerLevel, std::string&& );
private:
    std::ofstream _file;
};
}