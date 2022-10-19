#pragma once

#include <string>

#include "config.h"
#include "network/socket.h"

namespace chisel {
class Player {
public:
    Player ( sock::Client, sock::Server* );
    ~Player();

    void tick( const Config* );

    void kick    ( std::string ) const;
    void send_msg( std::string ) const;
private:
    const sock::Client  _clSock;
    const sock::Server* _srSock;
    const bool          _active;
};
}