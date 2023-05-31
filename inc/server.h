#pragma once

#include <string>
#include <vector>
#include <memory>

#include "config.h"
#include "logger.h"
#include "network/socket.h"
#include "thread/thread_pool.h"
#include "world.h"
#include "player.h"

namespace chisel {
class Server {
public:
    Server(
        chisel::Config*, 
        std::vector<std::string>*,
        World
    );
    ~Server();
            
    void start();
private:
    std::vector<std::string>*   _operators;
    chisel::Config*             _config;
    std::string                 _salt;
    sock::Server                _socket;
    logger::Logger              _logger;
    thread::ThreadPool          _threadPool;
    std::vector<chisel::Player> _players;
    World                       _world;

    void tick          ();
    void tick_player   ( chisel::Player& );

    void send_serv_idt  ( const sock::Client&, bool ) const;
    bool player_id_exist( const int8_t )              const;
};
}