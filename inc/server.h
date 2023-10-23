#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>
#include <optional>

#include "config.h"
#include "logger.h"
#include "socket.h"
#include "thread_pool.h"
#include "world.h"
#include "player.h"
#include "plugin.h"

#define OP_FILE "op-list.txt"

namespace chisel {
class Server {
public:
    Server( 
        chisel::Config*, 
        std::vector<std::string>, 
        World 
    );
    ~Server();
            
    void start    ();
    void stop     ();
    bool set_block( const Location, const char ); 

    void                  broadcast( const std::string, const int8_t = -2 ) const;
    bool                  is_running()                                      const { return _running; }
    std::optional<Player> get_player( const int8_t )                        const;
private:
    std::atomic<bool>                         _running;
    std::vector<std::string>                  _operators;
    chisel::Config*                           _config;
    sock::Server                              _socket;
    Logger                                    _logger;
    thread::ThreadPool                        _threadPool;
    std::vector<chisel::Player>               _players;
    std::vector<std::unique_ptr<api::Plugin>> _plugins;
    std::queue<sock::Client>                  _cQueue;
    std::mutex                                _cqMutex;
    World                                     _world;

    void tick        ();
    void tick_player ( chisel::Player& );
    void load_plugins();

    void echo_pckt      ( const std::vector<char>& )  const;
    void send_serv_idt  ( const sock::Client&, bool ) const;
};
}