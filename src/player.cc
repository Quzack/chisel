#include <iostream>
#include <algorithm>

#include "player.h"
#include "network/packet.h"

using chisel::Player;

Player::Player( sock::Client clSock, sock::Server* srSock ):
    _clSock(clSock),
    _srSock(srSock)
{

}

Player::~Player() {

}

void Player::tick( const Config* config ) {
    const unsigned int pId = _clSock.read_byte();

    switch(pId) {
        case 0x00: {
            auto data = packet::client::identify(_clSock);
            // TODO 23/10/22: Send Identify queue action to server.
            std::cout << data.username << std::endl;
        }
        default: 
            std::cout << "Unknown packet: " << pId << std::endl;
            break;
    }
}

void Player::kick( std::string reason ) const {
    // TODO 27/9/22: Send 0x0e packet.
}

void Player::send_msg( std::string msg ) const {
    // TODO 28/9/22: Send 0x0d packet.
}