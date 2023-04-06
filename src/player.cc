#include "player.h"

using chisel::Player;

Player::Player( sock::Client socket, const int8_t id ):
    _socket(socket),
    _id    (id)
{

}

Player::~Player() {

}

void Player::teleport( Location location ) {
    
}

void Player::disconnect( std::string reason ) const {
    // TODO 27/9/22: Send 0x0e packet.
}

void Player::send_msg( std::string msg ) const {
    // TODO 28/9/22: Send 0x0d packet.
}

bool Player::ping() const {
    return _socket.send_pckt({0x01});
}