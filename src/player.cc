#include <iostream>

#include "utils.h"
#include "player.h"
#include "server.h"
#include "network/packet.h"
#include "gzip/compress.h"

using chisel::Player;

Player::Player( 
    sock::Client  clSock, 
    sock::Server* srSock,
    Server*       server 
):
    _clSock(clSock),
    _srSock(srSock),
    _server(server)
{

}

Player::~Player() {

}

void Player::tick() {
    if(!ping()) {
        active = false;
        return;
    }

    const unsigned int pId = _clSock.read_byte();

    switch(pId) {
        case 0x00: {
            auto data = packet::client::identify(_clSock);
            
            this->_op = obj_in_vec(*_server->operators, data.username);

            send_serv_idt(_server->config->name, _server->config->motd);

            for(auto& player : _server->get_players()) {
                if(player.name != data.username) continue;
                disconnect("Player already on the server."); 
            }

            this->name = data.username;

            send_wrld_data(_server->world);
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

    _clSock.send_pckt(sIdentify.get_data());
}

void Player::send_wrld_data( const World& world ) const {
    _clSock.send_pckt({0x02}); // LEVEL INIT.
    std::string data = gzip::compress(&world.blocks[0], world.blocks.size());

    int dsize = data.size();
    int i = (dsize % CHUNK_LENGTH == 0) ? dsize/CHUNK_LENGTH : (dsize - (dsize % CHUNK_LENGTH))/CHUNK_LENGTH;

    for(int j = 0; j < i; j++) {
        // ........................................
    }
}

bool Player::ping() const {
    return _clSock.send_pckt({0x01});
}