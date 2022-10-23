#pragma once

#include <string>
#include <vector>

#include "config.h"
#include "network/socket.h"

namespace chisel {
class Player {
public:
    Player ( sock::Client, sock::Server* );
    ~Player();

    void tick( const Config* );
    void s_op( bool op ) { this->_op = op; }

    void kick    ( std::string ) const;
    void send_msg( std::string ) const;
    bool is_op   ()              const { return this->_op; }
private:
    const sock::Client  _clSock;
    const sock::Server* _srSock;
    bool                _active = true;
    bool                _op     = false;
};
}