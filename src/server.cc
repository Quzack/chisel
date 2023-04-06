#include <random>
#include <time.h>
#include <iostream>

#include "server.h"
#include "utils.h"
#include "network/packet.h"

#define TICK_INTERVAL 1000/20

using chisel::Server;

Server::Server( 
    chisel::Config* config, 
    std::vector<std::string>* ops,
    World world
):
    _salt      (rand_b62_str(16)),
    _logger    ("LOG_" + std::to_string(time(NULL)) + ".txt"),
    _world     (world),
    _threadPool(config->maxPlayers + 5),
    _config    (config),
    _operators (ops)
{
    _threadPool.start();
}

Server::~Server() {
    _threadPool.stop();
}

void Server::start() {
    _socket.listen_port(_config->port);
    _logger.log(LL_INFO, "Listening for clients...");

    _threadPool.queue([this] { 
        while(true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICK_INTERVAL));
            this->tick();
        }
    });

    while(true) {
        int8_t pId;
        do {  pId = rand_no(-127, 127); } while(player_id_exist(pId));

        _players.push_back(Player(_socket.accept_cl(), pId));
    }
}

void Server::tick() {
    for(auto& p : _players) {
        if(p.active == false) {
            // TODO 27/10/22: Removing player from vector.
            continue;
        }

        tick_player(p);
    }
}

void Server::tick_player( chisel::Player& player ) {
    if(!player.ping()) {
        player.active = false;
        return;
    }

    unsigned int pId = player.socket().read_byte();

    switch(pId) {
        case 0x00: {
            auto data = packet::identify_cl(player.socket());

            player.op = obj_in_vec(*_operators, data.username);
            send_serv_idt(player.socket(), player.op);

            for(auto& player : _players) {
                if(player.name != data.username) continue;
                player.disconnect("Player already on the server."); 
            }

            player.name = data.username;

            _logger.log(LL_INFO, player.name + " is connecting...");
            _world.join(player);
        }
        default: 
            std::cout << "Unknown packet: " << pId << std::endl;
            break;
    }
}

void Server::send_serv_idt( const sock::Client& client, bool op ) const {
    packet::Packet idt(0x00);
    idt.write_byte    (packet::PROTOCOL_VERSION);
    idt.write_str     (_config->name);
    idt.write_str     (_config->motd);
    idt.write_byte    (op ? 0x64 : 0x00);

    client.send_pckt(idt.get_data());
}

bool Server::player_id_exist( const int8_t id ) const {
    for(auto& player : _players) {
        if(player.id() == id) return true;
    }
    return false;
}