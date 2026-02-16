#pragma once
#include <functional>
#include <vector>
#include <string_view>
#include <crow.h>
#include <nlohmann/json.hpp>

namespace raven {
    class ControllerBase {
    public:
        explicit ControllerBase(std::string_view prefix) : _bp(prefix.data()) {

        }
        virtual ~ControllerBase() = default;

    public:
        void Register(crow::SimpleApp& app) {
            for (const auto& routeFunc : _routeFuncList) {
                routeFunc();
            }
            app.register_blueprint(_bp);
        }
    protected:
        // 매크로에서 멤버 변수로 선언될 도우미 구조체
        struct RouteRegistry {
            RouteRegistry(ControllerBase* parent, std::function<void()>&& func) {
                parent->_routeFuncList.emplace_back(func);
            }
        };

        std::vector<std::function<void()>> _routeFuncList;
    protected:
        [[nodiscard]] crow::Blueprint& GetBlueprint() { return _bp; }

        template<class TController, class TReq, class TRes>
        std::function<crow::response(crow::request)> WrapHandler(TController* self, TRes(TController::*handler)(TReq)) {
            return [self, handler](crow::request httpReq) -> crow::response {
                try {
                    nlohmann::json serializedBody = nlohmann::json::parse(httpReq.body);
                    TReq req = serializedBody.get<TReq>();
                    TRes res = (self->*handler)(req);
                    return crow::response(crow::status::OK, "application/json", nlohmann::json(res).dump());
                } catch (const std::exception& exc) {
                    return crow::response(crow::status::INTERNAL_SERVER_ERROR, "application/json", exc.what());
                }
            };
        }

        template<class TController, class TRes>
        std::function<crow::response(crow::request)> WrapHandler(TController* self, TRes(TController::*handler)()) {
            return [self, handler](crow::request) -> crow::response {
                try {
                    TRes res = (self->*handler)();
                    return crow::response(crow::status::OK, "application/json", nlohmann::json(res).dump());
                } catch (const std::exception& exc) {
                    return crow::response(crow::status::INTERNAL_SERVER_ERROR, "application/json", exc.what());
                }
            };
        }

    private:
        crow::Blueprint _bp;
    };
}

// --- 매크로 정의 ---

// 클래스 타입 정의 및 부모 생성자 호출
#define RAVEN_CONTROLLER(ClassName, Prefix) \
    using ControllerType = ClassName; \
    ClassName() : raven::ControllerBase(Prefix) {}

// 핵심 로직: 멤버 변수 초기화 리스트를 통해 'this'가 완성된 시점에 실행됩니다.
#define ROUTE_INTERNAL(method, path, name) \
    raven::ControllerBase::RouteRegistry _reg_##name{this, [this]() { \
        auto handler = this->WrapHandler(static_cast<ControllerType*>(this), &ControllerType::name); \
        CROW_BP_ROUTE(this->GetBlueprint(), path).methods(crow::HTTPMethod::method) \
        ([handler](const crow::request& req) { return handler(req); }); \
    }};

// C# 어트리뷰트 느낌을 위해 함수 선언 위에 배치합니다.
#define ROUTE_GET(path, name)    ROUTE_INTERNAL(GET, path, name)
#define ROUTE_POST(path, name)   ROUTE_INTERNAL(POST, path, name)
#define ROUTE_DELETE(path, name) ROUTE_INTERNAL(DELETE, path, name)