#include <random>
#include <time.h>

#include "server.h"
#include "utils.h"
#include "network/packet.h"
#include "network/http.h"

using chisel::Server;

const std::string HEARTBEAT_URL = "http://www.classicube.net/server/heartbeat";

Server::Server( Config* config ):
    _salt      (rand_b62_str(16)),
    _config    (config),
    _logger    ("LOG_" + std::to_string(time(NULL)) + ".txt"),
    _threadPool(_config->maxPlayers + 5)
{

}

Server::~Server() {
    delete _config;
}

void Server::start() {
    _socket.listen_port(_config->port);
    _logger.log(logger::LL_INFO, "Listening for clients...");

    _threadPool.push(start_heart, this);

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

void Server::start_heart() {
    using std::to_string;

    auto url = 
        HEARTBEAT_URL + _config->params() + "&version=" + to_string(packet::PROTOCOL_VERSION) +"&salt" + _salt + "&users=" + to_string(_players.size());
    
    http::Request req { url };
    
    const auto res = req.send();
    _logger.log(logger::LL_INFO, "Server URL: " + std::string{ res.body.begin(), res.body.end() });

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(45));
        req.send();
    }
}