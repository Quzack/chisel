#include <random>
#include <iostream>

#include "server.hpp"

using chisel::Server;

Server::Server( chisel::Config* config ):
    m_config (config),
    m_version(0x07),
    m_players(0),
    m_salt   (randB62Str(16))
{
    
}

Server::~Server() {
    delete m_config;
}

void Server::start() const {
    // TODO:
    // - Send heartbeat to ClassiCube serverlist
    // - Listen for player joins
}

void Server::tick() {
    // TODO: Implementation...
}

std::string Server::randB62Str( std::string::size_type len ) const {
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