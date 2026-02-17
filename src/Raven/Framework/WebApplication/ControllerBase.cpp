#include <Raven/Framework/WebApplication/ControllerBase.hpp>

namespace raven {
    ControllerBase::ControllerBase(std::string_view prefix) : _bp(prefix.data()) {}
    ControllerBase::~ControllerBase() = default;

    void ControllerBase::Register(crow::SimpleApp& app) {
        for (const auto& routeFunc : _routeFuncList) routeFunc();
        app.register_blueprint(GetBlueprint());
    }

    ControllerBase::RouteRegistry::RouteRegistry(ControllerBase* parent, std::function<void()>&& func) {
        parent->AddRoute(std::move(func));
    }

    void ControllerBase::AddRoute(std::function<void()>&& func) {
        _routeFuncList.emplace_back(func);
    }

    crow::Blueprint& ControllerBase::GetBlueprint() {
        return _bp;
    }
}
