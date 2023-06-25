#include <random>
#include <time.h>

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
    _threadPool(std::thread::hardware_concurrency()),
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
    _logger.log(LL_INFO, "Started the server.");

    _threadPool.queue([this] { 
        while(true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICK_INTERVAL));
            this->tick();
        }
    });

    while(true) {
        int pId;
        do {  pId = rand_no(0, 127); } while(player_id_exist(pId));

        _players.push_back(Player(_socket.accept_cl(), pId));
    }
}

void Server::broadcast( const std::string msg, const int8_t id ) const {
    packet::Packet packet(0x0d);
    packet.write_sbyte   (id);
    packet.write_str     (msg);

    for(auto& p : _players) {
        p.socket().send_pckt(packet.get_data());
    }
}

void Server::tick() {
    for(auto& p : _players) {
        if(!p.ping()) p.active = false;
    }

    int i = 0;
    while (i < _players.size()) {
        auto& player = _players[i];
        if (!player.active) {
            // TODO 9/4/23: Despawn player packet.
            _players.erase(_players.begin() + i);
            return;
        }

        tick_player(player);
        i++;
    }
}

void Server::tick_player( chisel::Player& player ) {
    char pId = player.socket().read_byte();

    switch(pId) {
        case 0x00: {
            auto data = packet::identify_cl(player.socket());

            player.op = obj_in_vec(*_operators, data.username);
            send_serv_idt(player.socket(), player.op);

            if(_players.size() + 1 > _config->maxPlayers)  {
                player.disconnect("Server is currently full.");
            }
            
            for(auto& player : _players) {
                if(player.name != data.username) continue;
                player.disconnect(player.name + " is already on the server."); 
            }

            player.name = data.username;
        
            _world.spawn(player);
            _logger.log(LL_INFO, player.name + " has joined the server.");

            for(auto& p : _players) {           
                if(p.id() == player.id()) continue;

                packet::send_spawn_pckt(player, player.loc(), p.socket());
                packet::send_spawn_pckt(p, p.loc(), player.socket());
            }
            break;
        }
        case 0x05: {
            auto data = packet::id_set_blck(player.socket());
            char block = (data.mode == 0x01) ? data.block : 0x00; 

            if(!this->_world.set_block(data.coord, block)) return;

            packet::Packet pckSb(0x06);
            pckSb.write_xyz     (data.coord.x, data.coord.y, data.coord.z);
            pckSb.write_byte    (block);

            for(auto& p : _players) { p.socket().send_pckt(pckSb.get_data()); }
            break;
        }
        case 0x08: {
            auto data = packet::id_set_pos(player.socket());

            packet::Packet pSp(0x08);
            pSp.write_sbyte   (player.id());
            pSp.write_loc     (data.coord);

            for(auto& p : _players) {
                if(p.id() == data.id) continue;
                p.socket().send_pckt(pSp.get_data());
            }
            break;
        }
        case 0x0d: {
            auto msg = packet::rd_msg_pck(player.socket());
            std::string rmsg = "<" + player.name + "> " + msg.msg;

            if(rmsg.size() > packet::STR_BF_SZ) {
                player.send_msg("&4Message exceeds 64 characters.");
                return;
            }

            broadcast  (rmsg, player.id());
            _logger.log(LL_INFO, rmsg);
            break;
        }
        default: 
            break;
    }
}

void Server::send_serv_idt( const sock::Client& client, bool op ) const {
    packet::Packet idt(0x00);
    idt.write_byte    (packet::PROTOCOL_VERSION);
    idt.write_str     (_config->name.substr(0, 63));
    idt.write_str     (_config->motd.substr(0, 63));
    idt.write_byte    (op ? 0x64 : 0x00);

    int padding = 131 - idt.get_data().size();
    for (int i = 0; i < padding; i++) {
        idt.write_byte(0x00);
    }

    client.send_pckt(idt.get_data());
}

bool Server::player_id_exist( const int8_t id ) const {
    for(auto& player : _players) {
        if(player.id() == id) return true;
    }
    return false;
}