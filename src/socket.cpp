#include <iostream>
#include <vector>

#include "socket.hpp"

using chisel::sock::Server;
using chisel::sock::Client;

Server::Server() {

}

Server::~Server() {
    closesocket(_fd);
    WSACleanup();
}

void Server::listen_port( const int port ) {
    WSADATA wData;
    WSAStartup(0x2022, &wData);

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port); 

    this->_addr = addr;

    this->_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(_fd, (LPSOCKADDR) &addr, sizeof(addr));

    std::cout << "Listening to port: " << port << std::endl;
    listen(_fd, SOMAXCONN);
}

Client Server::accept_cl() const {
    int size = sizeof(_addr);
    const int fd = accept(_fd, (LPSOCKADDR) &_addr, &size);

    return Client(fd);
}

unsigned char Server::read_byte() const {
    char buff;
    recv(_fd, &buff, 1, 0x8);

    return (unsigned char) buff;
}

std::string Server::read_str() const {
    std::vector<unsigned char> bytes;

    for(int i = 0; i < 64; ++i) {
        bytes.push_back(read_byte());
    }

    return std::string(bytes.begin(), bytes.end());
}

Client::Client( int fd ):
    _fd(fd)
{

}