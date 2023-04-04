#pragma once

#include <string>
#include <vector>

#include "network/socket.h"
#include "model/world.h"

namespace chisel {
class Server;
class Player {
public:
    bool        active = true;
    std::string name;

    Player ( sock::Client, sock::Server*, Server* );
    ~Player();

    void tick();

    void disconnect( std::string ) const;
    void send_msg  ( std::string ) const;
    bool is_op     ()              const { return this->_op; }
private:
    const sock::Client  _clSock;
    const sock::Server* _srSock;
    Server*             _server;
    bool                _op = false;

    void send_serv_idt ( const std::string&, const std::string& ) const;
    void send_wrld_data( const World& )                           const;
    bool ping          ()                                         const;
};
}