#pragma once

#include <string>
#include <vector>

#include "config.h"
#include "logger.h"
#include "network/socket.h"
#include "thread/thread_pool.h"
#include "player.h"

namespace chisel::server {
class Server {
public:
    std::vector<std::string>* operators;
    Config*                   config;

    Server ( Config*, std::vector<std::string>* );
    ~Server();
            
    void                      start      ();
    std::vector<Player>&      get_players() { return this->_players; }
private:
    std::string               _salt;
    sock::Server              _socket;
    logger::Logger            _logger;
    thread::ThreadPool        _threadPool;
    std::vector<Player>       _players;

    void tick();
};
}