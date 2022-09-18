#include <string>
#include <fstream>

namespace chisel {
    struct Config {
        unsigned int port;
        unsigned int maxPlayers;
        std::string  name;
        bool         pub;

        static void createDefault();
        static Config fromFile   ( std::ifstream& );
    };
}