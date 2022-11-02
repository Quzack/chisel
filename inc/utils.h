#pragma once

#include <fstream>
#include <random>
#include <time.h>
#include <algorithm>

inline bool file_exists( const char* name ) {
    FILE* file;if(file = fopen(name, "r")) {
        fclose(file);return true;
    }

    return false;
}

inline std::string rand_b62_str( int len ) {
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

inline std::string current_date_time() {
    time_t now = time(0);

    struct tm tData;
    char      buf[80];
    
    tData = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tData);
    
    return buf;
}

inline std::string remove_empty( std::string str ) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

template<typename T>
inline bool itm_in_vec( std::vector<T>& vec, T& itm ) {
    return std::find(vec.begin(), vec.end(), itm) != vec.end();
}