#pragma once

#include <string>
#include <memory>

#include "server.h"

#ifdef BUILD_DLL
    #define DLLAPI __declspec(dllexport)
#else
    #define DLLAPI
#endif

namespace chisel{
class Server;
namespace api {
class Plugin {
public:
    Plugin( const std::string name ): 
        _name(name) 
    {

    }
    virtual ~Plugin() = default;

    virtual void on_start( const chisel::Server& ) = 0;
protected:
    std::string _name;
};

DLLAPI std::unique_ptr<Plugin> get_plugin(void);
}
}