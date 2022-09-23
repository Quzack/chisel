#pragma once

#include <string>

#include "config.hpp"

namespace chisel {
    class Heart {
        public:
            Heart ( 
                    const Config*      config, 
                    const std::string& salt, 
                    const int          playerCount 
                );
            ~Heart();
        private:
            const Config*      m_config;
            const std::string& m_salt;
            const int          m_playerCount;

            void start() const;
            void pulse() const;
    };
}