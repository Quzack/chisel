#include <iostream>

#include "player.h"
#include "network/packet.h"

using chisel::Player;

Player::Player( sock::Client socket, const int8_t id ):
    _socket(socket),
    _id    (id)
{

}

Player::~Player() {

}

void Player::disconnect( std::string reason ) const {
    // TODO 27/9/22: Send 0x0e packet.
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