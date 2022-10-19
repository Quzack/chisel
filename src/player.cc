#include <iostream>

#include "player.h"
#include "network/packet.h"

using chisel::Player;

Player::Player( sock::Client clSock, sock::Server* srSock ):
    _clSock(clSock),
    _srSock(srSock),
    _active(true)
{

}

Player::~Player() {

}

void Player::tick( const Config* config ) {
    const unsigned int pId = _clSock.read_byte();

    using packet::Packet;

    switch(packet::packet_from_id(pId)) {
        case Packet::CS_AUTH: {
            auto data = packet::client::identify(_clSock);
        }
        case Packet::UNKNOWN:
            std::cout << "Unknown packet: " << pId << std::endl;
            break;
        default: 
            break;
    }
}

void Player::kick( std::string reason ) const {
    // TODO 27/9/22: Send 0x0e packet.
}

void Player::send_msg( std::string msg ) const {
    // TODO 28/9/22: Send 0x0d packet.
}