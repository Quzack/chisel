#pragma once

#include <string>

#include "network/socket.h"
#include "model/location.h"

namespace chisel {
class Server;
class Player {
public:
    bool        active = true;
    bool        op     = false;
    std::string name;

    Player ( sock::Client );
    ~Player();

    void teleport( Location );

    void disconnect    ( std::string ) const;
    void send_msg      ( std::string ) const;
    bool ping          ()              const;
    sock::Client socket()              const { return this->_socket; }
private:
    const sock::Client  _socket;
};
}