#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <shared_mutex>

#include "config.h"
#include "logger.h"
#include "network/socket.h"
#include "thread/thread_pool.h"
#include "world.h"
#include "player.h"

#define OP_FILE "op-list.txt"

namespace chisel {
class Server {
public:
    Server( 
        chisel::Config*, 
        std::vector<std::string>, 
        World 
    );
            
    void start      ();
    void stop       ();
    void broadcast  ( const std::string, const int8_t = -2 );

    bool is_running() const { return this->_running; }
private:
    std::atomic<bool>           _running;
    std::vector<std::string>    _operators;
    chisel::Config*             _config;
    std::string                 _salt;
    sock::Server                _socket;
    Logger                      _logger;
    thread::ThreadPool          _threadPool;
    std::vector<chisel::Player> _players;
    //std::shared_mutex           _pMutex;
    World                       _world;

    void tick           ();
    void tick_player    ( chisel::Player& );
    void echo_pckt      ( const std::vector<char>& );
    bool player_id_exist( const int8_t );

    void send_serv_idt  ( const sock::Client&, bool ) const;
};
}