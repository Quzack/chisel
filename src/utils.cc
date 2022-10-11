#include <fstream>
#include <random>
#include <time.h>

#include "utils.h"

bool file_exists( const char* name ) {
    FILE* file;if(file = fopen(name, "r")) {
        fclose(file);return true;
    }

    return false;
}

std::string rand_b62_str( int len ) {
    auto& chrs = "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::mt19937 rg{std::random_device{}()};
    std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

    std::string str;
    str.reserve(len);

    while(len--) str += chrs[pick(rg)];

    return str;
}

std::string current_date_time() {
    time_t now = time(0);

    struct tm tData;
    char      buf[80];
    
    tData = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tData);
    
    return buf;
}