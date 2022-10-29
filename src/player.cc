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

void Player::tick( 
    const Config*                   config,
    const std::vector<std::string>* ops
) {
    if(!ping()) {
        active = false;
        return;
    }

    const unsigned int pId = _clSock.read_byte();

    switch(pId) {
        case 0x00: {
            auto data = packet::client::identify(_clSock);
            this->_op = std::find(ops->begin(), ops->end(), data.username) != ops->end();
            
            /*
            packet::server::Identify(
                config->name,
                config->motd,
                _op
            ).send_to(_clSock.get_fd());
            */
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

bool Player::ping() const {
    return packet::Packet(0x01).send_to(_clSock.get_fd()) != -1;
}