#pragma once

#include <string>

#include "config.hpp"

namespace chisel {
    class Heart {
        public:
            Heart ( 
                    const std::string& configParams, 
                    const std::string& salt, 
                    const int          playerCount 
                );
        private:
            std::string m_url;

            void start() const;
    };
}