#include <random>

#include "server.hpp"

using namespace chisel;

Server::Server(
    unsigned int port,
    unsigned int maxPlayers,
    std::string* name,
    bool         pub
):
    m_port      (port),
    m_maxPlayers(maxPlayers),
    m_version   (0x07),
    m_userCount (0),
    m_name      (name),
    m_salt      (randB62Str(16)),
    m_public    (pub)
{
    
}

Server::~Server() {
    delete m_name;
}

void Server::start() const {
    // TODO: Implementation...
}

std::string Server::randB62Str(std::string::size_type len) const {
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