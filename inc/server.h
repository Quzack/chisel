#pragma once

#include <string>
#include <vector>

#include "config.h"
#include "logger.h"
#include "network/socket.h"
#include "thread/thread_pool.h"
#include "player.h"

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
    thread::ThreadPool   _threadPool;
    std::vector<Player>  _players;

    void tick       ();
    void start_heart();
};
}