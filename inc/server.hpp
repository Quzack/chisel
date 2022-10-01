#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include "network/socket.hpp"
#include "player.hpp"
#include "logger.hpp"

namespace chisel {
const unsigned int DEFAULT_PORT = 25565;

class Server {
public:
    Server ( Config* );
    ~Server();
            
    void start();

    void broadcast( std::string ) const;
private:
    std::string          _salt;
    Config*              _config;
    sock::Server         _socket;
    logger::Logger       _logger;
    std::vector<Player>  _players;

    void tick();
};
}