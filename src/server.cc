#include <random>
#include <time.h>

#include "server.h"
#include "utils.h"
#include "network/packet.h"
#include "network/http.h"

#define TICK_INTERVAL 1000/20

using chisel::server::Server;

const std::string HEARTBEAT_URL = "http://www.classicube.net/server/heartbeat";

Server::Server( Config* config, std::vector<std::string>* ops ):
    _salt      (rand_b62_str(16)),
    _config    (config),
    _logger    ("LOG_" + std::to_string(time(NULL)) + ".txt"),
    _threadPool(_config->maxPlayers + 5),
    _operators (ops)
{
    _threadPool.start();
}

Server::~Server() {
    _threadPool.stop();

    delete _config;
}

void Server::start() {
    _socket.listen_port(_config->port);
    _logger.log(LL_INFO, "Listening for clients...");

    _threadPool.queue([this] { this->start_heart(); });

    _threadPool.queue([this] { 
        while(true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICK_INTERVAL));
            this->tick();
        }
    });

    while(true) {
        auto client = _socket.accept_cl();
        _players.push_back(Player(client, &_socket));
    }
}

void Server::tick() {
    for(auto& p : _players) {
        p.tick(_config);
    }
}

void Server::start_heart() {
    auto url = HEARTBEAT_URL + _config->params()               + 
        "&version=" + std::to_string(packet::PROTOCOL_VERSION) +
        "&salt="    + _salt                                    + 
        "&users="   + std::to_string(_players.size());
    
    http::Request req { url };
    
    const auto res = req.send();
    _logger.log(LL_INFO, "Server URL: " + std::string{ res.body.begin(), res.body.end() });

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(45));
        req.send();
    }
}