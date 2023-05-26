#pragma once

#include <fstream>
#include <random>
#include <time.h>
#include <algorithm>

inline bool file_exists( const char* name ) {
    FILE* file;
    
    if(file = fopen(name, "r")) {
        fclose(file);
        return true;
    }

    return false;
}

inline int rand_no( const int x, const int y ) {
    static std::random_device rd; 
    static std::mt19937 eng(rd()); 
    static std::uniform_int_distribution<> distr(x, y); 

    return distr(eng);
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

inline std::string fmt_time( const char* fmt ) {
    time_t now = time(0);

    struct tm tData;
    char      buf[80];
    
    tData = *localtime(&now);
    strftime(buf, sizeof(buf), fmt, &tData);
    
    return buf;
}

inline std::string rem_empty( std::string str ) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

template<typename T>
inline bool obj_in_vec( std::vector<T>& vec, T& obj ) {
    return std::find(vec.begin(), vec.end(), obj) != vec.end();
}