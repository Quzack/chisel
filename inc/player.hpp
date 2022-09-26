#pragma once

#include <string>

namespace chisel {
    struct Location {
        int          x, y, z;
        unsigned int yaw, pitch;
    };

    class Player {
        public:
            Player ( int clientFd, int* serverFd );
            ~Player();

            void tick();
        private:
            const int  m_clientFd; 
            const int* m_serverFd;           
    };
}