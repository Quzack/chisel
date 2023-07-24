#include <iostream>

#include "player.h"
#include "network/packet.h"

using chisel::Player;

Player::Player( sock::Client socket, const int8_t id ):
    _socket(socket),
    _id    (id)
{

}

Player& Player::operator=( const Player& player ) {
    if(this != &player) {
        this->active  = player.active;
        this->op      = player.op;
        this->name    = player.name;
        this->_id     = player._id;
        this->_socket = player._socket;
    }

    return *this;
}

void Player::disconnect( const std::string reason ) {
    packet::Packet discp(0x0e);
    discp.write_str     (reason);

    _socket.send_pckt(discp.get_data());
    this->active = false;
}

void Player::send_msg( std::string msg ) const {
    packet::Packet msgp(0x0d);
    msgp.write_sbyte   (-2);
    msgp.write_str     (msg);

    _socket.send_pckt(msgp.get_data());
}

bool Player::ping() const {
    return _socket.send_pckt({0x01});
}