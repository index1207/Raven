#include <Raven/Framework/WebApplication/ControllerBase.hpp>

#include <crow/app.h>
#include <crow/routing.h>

namespace raven {
    ControllerBase::ControllerBase(std::string_view urlPrefix) : _bp(urlPrefix.data()) {
        ROUTE(GET, Home);
    }

    ControllerBase::~ControllerBase() = default;

    void ControllerBase::Register(crow::SimpleApp& app) {
        app.register_blueprint(_bp);
    }

    std::string ControllerBase::Home() {
        return "Hello";
    }
}
