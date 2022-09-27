#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include "player.hpp"

const unsigned int DEFAULT_PORT   = 25566;
const unsigned int SERVER_VERSION = 0x07;

namespace chisel {
    class Server {
        public:
            Server ( Config* );
            ~Server();
            
            void start();
        private:
            Config*              m_config;
            std::vector<Player>  m_players;
            std::string          m_salt;

            std::string genSalt() const;

            void tick();
    };
}