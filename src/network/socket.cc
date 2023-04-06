#include <iostream>

#include "network/socket.h"
#include "network/packet.h"

using chisel::sock::Server;
using chisel::sock::Client;

Client::Client( int fd ):
    _fd(fd)
{

}

int Client::send_pckt( const std::vector<char>& data ) const {
    send(_fd, &data[0], data.size(), 0);
}

char Client::read_byte() const {
    char buffer;
    return (recv(_fd, &buffer, 1, 0) == -1) ? -1 : buffer;
}

std::string Client::read_str() const {
    char buffer[chisel::packet::STR_BF_SZ];
    recv(_fd, buffer, chisel::packet::STR_BF_SZ, 0x8);
    
    return std::string(buffer);
}

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