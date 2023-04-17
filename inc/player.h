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

    Player ( sock::Client, int8_t );
    ~Player();

    Player& operator=(const Player& player) {
        if(this != &player) {
            this->active  = player.active;
            this->op      = player.op;
            this->name    = player.name;
            this->_id     = player._id;
            this->_socket = player._socket;
        }

        return *this;
    }

    void teleport( Location );

    void disconnect    ( std::string ) const;
    void send_msg      ( std::string ) const;
    bool ping          ()              const;
    sock::Client socket()              const { return this->_socket; }
    int8_t id          ()              const { return this->_id; }
private:
    int8_t        _id;
    sock::Client  _socket;
};
}