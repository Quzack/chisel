#pragma once

#include <string>

#include "network/socket.h"
#include "location.h"

namespace chisel {
class Server;
class Player {
public:
    bool        active = true;
    bool        op     = false;
    std::string name;

    Player (  sock::Client, int8_t );

    Player& operator=( const Player& );

    void disconnect( const std::string );
    void make_op   ()               { this->switch_usr_type(0x64); op = true; }
    void deop      ()               { this->switch_usr_type(0x00); op = false; }
    void set_pos   ( Location pos ) { this->_loc = pos; }

    void         send_msg( std::string ) const;
    bool         ping    ()              const;
    int8_t       id      ()              const { return this->_id; }
    Location     loc     ()              const { return this->_loc; }
    sock::Client socket  ()              const { return this->_socket; }
private:
    int8_t        _id;
    sock::Client  _socket;
    Location      _loc;

    void switch_usr_type( const char uid ) { _socket.send_pckt({0x0f, uid}); }
};
}