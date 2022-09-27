#include <random>
#include <iostream>
#include <winsock2.h>

#include "server.hpp"
#include "heart.hpp"

using chisel::Server;

Server::Server( Config* config ):
    m_config (config),
    m_players(0),
    m_salt   (genSalt())
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
    int addrSize =  sizeof(addr);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(m_config->port); 

    const int serverFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(serverFd, (LPSOCKADDR) &addr, addrSize);

    std::cout << "Listening for incoming clients in port " << m_config->port << std::endl;
    listen(serverFd, SOMAXCONN);

    chisel::Heart(m_config->asParams(), m_salt, m_players.size());

    while(true) {
        const int clientFd = accept(serverFd, (LPSOCKADDR) &addr, &addrSize);
        Player player(clientFd, &serverFd);

        m_players.push_back(player);
        // TODO 27/9/22: Server tick clock.
    }

    closesocket(serverFd);
}

void Server::tick() {
    
}

std::string Server::genSalt() const {
    int   len  = 16;
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