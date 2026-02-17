#pragma once
#include <functional>
#include <vector>
#include <string_view>
#include <crow.h>
#include <rfl/json.hpp>

#define RAVEN_CONTROLLER(ClassName, Prefix) \
using ControllerType = ClassName; \
ClassName() : raven::ControllerBase(Prefix) {}

#define ROUTE_INTERNAL(method, path, name) \
raven::ControllerBase::RouteRegistry _reg_##method_##name{this, [this]() { \
auto handler = this->WrapHandler(static_cast<ControllerType*>(this), &ControllerType::name); \
CROW_BP_ROUTE(this->GetBlueprint(), path).methods(crow::HTTPMethod::method) \
([handler](const crow::request& req) { return handler(req); }); \
}};

#define ROUTE_GET(path, name)    ROUTE_INTERNAL(GET, path, name)
#define ROUTE_POST(path, name)   ROUTE_INTERNAL(POST, path, name)
#define ROUTE_DELETE(path, name) ROUTE_INTERNAL(DELETE, path, name)

namespace raven {
    class ControllerBase {
    public:
        explicit ControllerBase(std::string_view prefix);
        virtual ~ControllerBase();
    public:
        void Register(crow::SimpleApp& app);
    protected:
        // 매크로에서 멤버 변수로 선언될 도우미 구조체
        struct RouteRegistry {
            RouteRegistry(ControllerBase* parent, std::function<void()>&& func);
        };
    protected:
        void AddRoute(std::function<void()>&& func);
        [[nodiscard]] crow::Blueprint& GetBlueprint();

        template<class TController, class TReq, class TRes>
        std::function<crow::response(crow::request)> WrapHandler(TController* self, TRes(TController::*handler)(TReq)) {
            return [self, handler](crow::request httpReq) -> crow::response {
                try {
                    TReq req = rfl::json::read<TReq>(httpReq.body).value();
                    TRes res = (self->*handler)(req);
                    return crow::response(crow::status::OK, "application/json", rfl::json::write<TRes>(res));
                } catch (const std::exception& exc) {
                    return crow::response(crow::status::INTERNAL_SERVER_ERROR, "text/plain", exc.what());
                }
            };
        }

        template<class TController, class TRes>
        std::function<crow::response(crow::request)> WrapHandler(TController* self, TRes(TController::*handler)()) {
            return [self, handler](crow::request) -> crow::response {
                try {
                    TRes res = (self->*handler)();
                    return crow::response(crow::status::OK, "application/json", rfl::json::write<TRes>(res));
                } catch (const std::exception& exc) {
                    return crow::response(crow::status::INTERNAL_SERVER_ERROR, "text/plain", exc.what());
                }
            };
        }
    private:
        crow::Blueprint _bp;
        std::vector<std::function<void()>> _routeFuncList;
    };
}
