#include <iostream>

#include "player.h"
#include "network/packet.h"

using chisel::Player;

Player::Player( sock::Client clSock, sock::Server* srSock ):
    _clSock(clSock),
    _srSock(srSock),
    _active(true)
{
    // TODO 27/9/2022: Threaded player handling.
}

Player::~Player() {

}

void Player::tick( const Config* config ) {
    const int pId = _srSock->read_byte();
    if(pId == -1) {
        printf("No data.");
        return;
    }

    using packet::Packet;

    switch(packet::packet_from_id(pId)) {
        case Packet::CS_AUTH: {
            auto data = packet::client::identify(_srSock);
            std::cout << "Identifying player " << data.protocolVer << std::endl;

            while(true) {}
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