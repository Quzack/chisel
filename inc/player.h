#pragma once

#include <string>
#include <vector>

#include "config.h"
#include "network/socket.h"

namespace chisel {
class Player {
public:
    bool active = true;

    Player ( sock::Client, sock::Server* );
    ~Player();

    void tick( 
        const Config*,
        const std::vector<std::string>*
    );

    void kick    ( std::string ) const;
    void send_msg( std::string ) const;
    bool is_op   ()              const { return this->_op; }
private:
    const sock::Client  _clSock;
    const sock::Server* _srSock;
    bool                _op = false;

    bool ping() const;
};
}