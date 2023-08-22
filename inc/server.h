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
            
    void start    ();
    void stop     ();

    void broadcast( const std::string, const int8_t = -2 ) const;
    bool is_running()                                      const { return this->_running; }
private:
    std::atomic<bool>           _running;
    std::vector<std::string>    _operators;
    chisel::Config*             _config;
    sock::Server                _socket;
    Logger                      _logger;
    thread::ThreadPool          _threadPool;
    std::vector<chisel::Player> _players;
    std::queue<sock::Client>    _cQueue;
    std::mutex                  _cqMutex;
    World                       _world;

    void tick           ();
    void tick_player    ( chisel::Player& );

    void echo_pckt      ( const std::vector<char>& )  const;
    bool player_id_exist( const int8_t )              const;
    void send_serv_idt  ( const sock::Client&, bool ) const;
};
}