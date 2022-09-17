#include <string>

const unsigned int DEFAULT_PORT = 25566;

namespace chisel {
    class Server {
        public:
            Server(
                    unsigned int port, 
                    unsigned int maxPlayers, 
                    std::string* name,
                    bool         pub
                );
            ~Server();
            
            void start() const;
        private:
            unsigned int m_port;
            unsigned int m_maxPlayers;
            unsigned int m_version;
            unsigned int m_userCount;
            std::string* m_name;
            std::string  m_salt;
            bool         m_public;

            std::string randB62Str(std::string::size_type) const;
    };
}