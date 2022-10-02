#include <random>
#include <time.h>

#include "heart.h"
#include "server.h"
#include "utils.h"

using chisel::Server;

Server::Server( Config* config ):
    _salt  (rand_b62_str(16)),
    _config(config),
    _logger("LOG_" + std::to_string(time(NULL)) + ".txt")
{

}

Server::~Server() {
    delete _config;
}

void Server::start() {
    _socket.listen_port(_config->port);
    _logger.log(logger::LL_INFO, "Listening for clients...");

    chisel::Heart(_config->params(), _salt, _players.size());

    while(true) {
        auto client = _socket.accept_cl();
        Player player(client, &_socket);

        _players.push_back(player);

        // TODO 29/9/22: Create thread and run ticks periodically.
        tick();
    }
}

void Server::broadcast( std::string msg ) const {
    for(auto& p : _players) {
        p.send_msg(msg);
    }
}

void Server::tick() {
    for(auto& p : _players) {
        p.tick(_config);
    }
}