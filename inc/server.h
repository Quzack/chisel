#pragma once

#include <string>
#include <vector>

#include "config.h"
#include "logger.h"
#include "network/socket.h"
#include "thread/thread_pool.h"
#include "model/world.h"
#include "player.h"

namespace chisel {
class Server {
public:
    std::vector<std::string>* operators;
    chisel::Config*           config;
    World                     world;

    Server(
        chisel::Config*, 
        std::vector<std::string>*,
        World
    );
    ~Server();
            
    void                         start      ();
    std::vector<chisel::Player>& get_players() { return this->_players; }
private:
    std::string                 _salt;
    sock::Server                _socket;
    logger::Logger              _logger;
    thread::ThreadPool          _threadPool;
    std::vector<chisel::Player> _players;

    void tick();
};
}