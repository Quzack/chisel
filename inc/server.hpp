#include <string>

#include "config.hpp"

const unsigned int DEFAULT_PORT = 25566;

namespace chisel {
    class Server {
        public:
            Server ( chisel::Config* );
            ~Server();
            
            void start() const;
        private:
            chisel::Config* m_config;
            unsigned int    m_version;
            unsigned int    m_players; // TODO: Change into vector of players
            std::string     m_salt;

            std::string randB62Str( std::string::size_type ) const;
    };
}