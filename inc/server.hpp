#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include "player.hpp"
#include "socket.hpp"

namespace chisel {
const unsigned int DEFAULT_PORT = 25566;

class Server {
public:
    Server ( Config* );
    ~Server();
            
    void start();

    void broadcast( std::string ) const;
private:
    Config*             _config;
    sock::Server        _socket;
    std::vector<Player> _players;
    std::string         _salt;

    void tick();

    std::string gen_salt() const;
};
}