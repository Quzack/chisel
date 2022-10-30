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
            
            if(data.protocolVer != packet::PROTOCOL_VERSION) {
                disconnect("Unsupported protocol version.");
                return;
            }

            if(data.username.length() > 16 || data.username.length() < 4) {
                disconnect("Invalid username.");
                return;
            }

            send_serv_idt(config->name, config->motd);
        }
        default: 
            std::cout << "Unknown packet: " << pId << std::endl;
            break;
    }
}

void Player::disconnect( std::string reason ) const {
    // TODO 27/9/22: Send 0x0e packet.
}

void Player::send_msg( std::string msg ) const {
    // TODO 28/9/22: Send 0x0d packet.
}

void Player::send_serv_idt( const std::string& name, const std::string& motd ) const {
    packet::Packet sIdentify(0x00);
    sIdentify.write_byte(packet::PROTOCOL_VERSION);
    sIdentify.write_str (name);
    sIdentify.write_str (motd);
    sIdentify.write_byte(_op ? 0x64 : 0x00);

    _clSock.send_pckt(sIdentify);
}

bool Player::ping() const {
    return _clSock.send_pckt(packet::Packet(0x01));
}