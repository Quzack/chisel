#include <random>

#include "server.hpp"
#include "heart.hpp"

using chisel::Server;

Server::Server( Config* config ):
    _config (config),
    _salt   (gen_salt())
{

}

Server::~Server() {
    delete _config;
}

void Server::start() {
    _socket.listen_port(_config->port);

    chisel::Heart(_config->params(), _salt, _players.size());

    //while(true) {
        auto client = _socket.accept_cl();
        Player player(client, &_socket);

        _players.push_back(player);

        tick();
    //}
}

void Server::broadcast( std::string msg ) const {
    for(auto& p : _players) {
        p.send_msg(msg);
    }
}

void Server::tick() {
    _players[0].tick(_config);
}

std::string Server::gen_salt() const {
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