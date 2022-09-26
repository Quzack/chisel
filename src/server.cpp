#include <random>
#include <iostream>
#include <winsock2.h>

#include "server.hpp"
#include "heart.hpp"

using chisel::Server;

Server::Server( Config* config ):
    m_config (config),
    m_version(7),
    m_players(0),
    m_salt   (randB62Str(16))
{
    
}

Server::~Server() {
    WSACleanup();

    delete m_config;
}

void Server::start() {
    WSADATA wData;
    WSAStartup(0x2022, &wData);

    SOCKADDR_IN addr;
    const size_t addrSize =  sizeof(addr);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(m_config->port); 

    const int serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(serverFd, (LPSOCKADDR) &addr, addrSize);

    std::cout << "Listening for incoming clients in port " << m_config->port << std::endl;
    listen(serverFd, SOMAXCONN);

    chisel::Heart(m_config->asParams(), m_salt, m_players.size());

    while(true) {
        
    }

    closesocket(serverFd);
}

void Server::tick() {
    
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