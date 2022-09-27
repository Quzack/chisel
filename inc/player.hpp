#pragma once

#include <string>

namespace chisel {
    struct Location {
        int          x, y, z;
        unsigned int yaw, pitch;
    };

    class Player {
        public:
            Player ( const int, const int* );
            ~Player();

            void tick();

            void disconnect( std::string );
        private:
            const int  m_clientFd; 
            const int* m_serverFd;
            const bool m_active;
    };
}