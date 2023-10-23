#pragma once

#include <string>

#include "socket.h"
#include "location.h"

namespace chisel {
class Player {
public:
    bool        active = true;
    std::string name;

    Player (  sock::Client, int8_t );
    ~Player();

    Player& operator=( const Player& );

    void disconnect( const std::string );
    void make_op   ()               { this->set_usr_type(0x64); _op = true; }
    void deop      ()               { this->set_usr_type(0x00); _op = false; }
    void set_pos   ( Location pos ) { this->_loc = pos; }

    void         send_msg( std::string ) const;
    bool         ping    ()              const;
    bool         opped   ()              const { return this->_op; }
    int8_t       id      ()              const { return this->_id; }
    Location     loc     ()              const { return this->_loc; }
    sock::Client socket  ()              const { return this->_socket; }
private:
    bool          _op = false;
    int8_t        _id;
    sock::Client  _socket;
    Location      _loc;

    void set_usr_type( const char uid ) { _socket.send_pckt({0x0f, uid}); }
};
}