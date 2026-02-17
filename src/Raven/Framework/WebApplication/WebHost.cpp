#include "Raven/Framework/WebApplication/WebHost.hpp"

namespace raven {
    void WebHost::Run(ushort port) {
        _app.port(port).multithreaded().run();
    }
}