#pragma once

#include <string>
#include <vector>

#include "config.h"
#include "logger.h"
#include "network/socket.h"
#include "thread/thread_pool.h"
#include "player.h"

namespace chisel::server {
// TODO 23/10/22: Implement server queue mechanics.
class Server {
public:
    Server ( Config*, std::vector<std::string>* );
    ~Server();
            
    void start();
private:
    std::string               _salt;
    Config*                   _config;
    sock::Server              _socket;
    logger::Logger            _logger;
    thread::ThreadPool        _threadPool;
    std::vector<Player>       _players;
    std::vector<std::string>* _operators;

    void tick       ();
    void start_heart();
};
}