#include <random>
#include <time.h>

#include "server.h"
#include "utils.h"
#include "packet.h"

using chisel::Server;

Server::Server( 
    chisel::Config* config,
    std::vector<std::string> ops,
    World world
):
    _running   (false),
    _logger    ("LOG_" + std::to_string(time(NULL)) + ".txt"),
    _world     (world),
    _threadPool(std::thread::hardware_concurrency()),
    _config    (config),
    _operators (ops)
{
    _threadPool.start();
}

Server::~Server() {

}

void Server::start() {
    _socket.listen_port(_config->port);
    load_plugins();
    _logger.log(LL_INFO, "Started the server.");

    this->_running = true;

    _threadPool.queue([this] { 
        while(this->_running) this->tick();
    });
    
    _threadPool.queue([this] {
        while(_running) {
            auto client = _socket.accept_cl(); // No thread safety: single use.
            if(!client.has_value()) continue;
            
            std::lock_guard<std::mutex> lock(_cqMutex);
            _cQueue.push(client.value());
        }
    });
}

void Server::stop() {
    for(auto& p : _players) {
        // TODO 16/7/23: User type checking.
        p.disconnect("Server has been stopped.");
    }

    _world.save_tf();
    _logger.log(LL_INFO, "Completed saving world.");

    this->_running = false;
    _threadPool.stop();
}

bool Server::set_block( const Location coord, const char block) {
    if(!_world.set_block(coord, block)) return false;

    packet::Packet pckSb(0x06);
    pckSb.write_xyz     (coord.x, coord.y, coord.z);
    pckSb.write_byte    (block);

    echo_pckt(pckSb.get_data());
    return true;
}

void Server::broadcast( const std::string msg, const int8_t id ) const {
    packet::Packet packet(0x0d);
    packet.write_sbyte   (id);
    packet.write_str     (msg);

    echo_pckt(packet.get_data());
}

std::optional<chisel::Player> Server::get_player( const int8_t id ) const {
    for(auto& player : _players) {
        if(player.id() == id) return player;
    }

    return {};
}

void Server::tick() {
    {
        std::lock_guard<std::mutex> lock(_cqMutex);
        while(!_cQueue.empty()) {
            int8_t id;
            do { id = rand_no(0, 127); } while(get_player(id).has_value());

            _players.push_back({_cQueue.front(), id});
            _cQueue.pop();
        }
    }

    for(auto& p : _players) {
        if(!p.ping()) p.active = false;
    }

    int i = 0;
    while (i < _players.size()) {
        auto& player = _players[i];

        if (!player.active) {
            echo_pckt({0x0c, player.id()});

            if(!player.name.empty()) {
                _logger.log(LL_INFO, player.name + " has left the server.");
                broadcast("&e" + player.name + " has left the server.");
            }
            
            _players.erase(_players.begin() + i);
            return;
        }

        tick_player(player);
        i++;
    }
}

// TODO 17/7/23: _world mutex?
void Server::tick_player( chisel::Player& player ) {
    switch((char) player.socket().read_byte()) {
        case 0x00: {
            auto data = packet::identify_cl(player.socket());

            if(_players.size() + 1 > _config->maxPlayers)  {
                player.disconnect("Server is currently full.");
                return;
            }
            
            for(auto& p : _players) {
                if(p.name != data.username) continue;
                player.disconnect(player.name + " is already on the server.");
                return;
            }

            player.name = data.username;
            
            if(obj_in_vec(_operators, data.username)) { player.make_op(); };
            send_serv_idt(player.socket(), player.opped());
        
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

            set_block(data.coord, block);
            break;
        }
        case 0x08: {
            auto data = packet::id_set_pos(player.socket());

            echo_pckt(packet::pos_packet(player.id(), data.coord).get_data());
            break;
        }
        case 0x0d: {
            auto msgp = packet::rd_msg_pck(player.socket());
            std::string msg = "<" + player.name + "> " + msgp.msg;

            if(msg.size() > packet::STR_BF_SZ) {
                player.send_msg("&4Message exceeds 64 characters.");
                return;
            }

            broadcast  (msg, player.id());
            _logger.log(LL_INFO, msg);
            break;
        }
        default: 
            break;
    }
}

void Server::load_plugins() {
    _logger.log(LL_INFO, "Loading plugins.");    
    
    WIN32_FIND_DATAW fd;
    HANDLE fh = FindFirstFileW(L".\\plugins\\*.dll", &fd);

    if (fh == INVALID_HANDLE_VALUE) {
        _logger.log(LL_INFO, "No plugins found.");
        mkdir("plugins");
        return;
    }

    do {
        std::wstring path = std::wstring(L".\\plugins\\") + std::wstring(fd.cFileName);
        HINSTANCE temp = LoadLibraryW(path.c_str());

        if(!temp) {
            _logger.log(LL_WARN, "Failed to load plugin: ");
            continue;
        }

        typedef std::unique_ptr<api::Plugin> (__cdecl *PProc)(void);
        PProc getP = (PProc) GetProcAddress(temp, "get_plugin");

        _plugins.push_back(std::move(getP()));
        FreeLibrary(temp);
    } while(FindNextFileW(fh, &fd));
    FindClose(fh);
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

void Server::echo_pckt( const std::vector<char>& data ) const {
    for(auto& p : _players) {
        p.socket().send_pckt(data);
    }
}