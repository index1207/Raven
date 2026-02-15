#pragma once
#include <functional>
#include <string_view>

#include <crow/http_response.h>
#include <crow/http_request.h>
#include <crow/routing.h>
#include <crow/app.h>
#include <nlohmann/json.hpp>

#define THIS_TYPE std::remove_pointer_t<decltype(this)>
#define ROUTE(method, name) Route_##method_##name()
#define ROUTE_GET(path, name) void ROUTE(GET, name) { CROW_BP_ROUTE(_bp, path)(WrapHandler(&THIS_TYPE::name)); }

namespace raven {
    class ControllerBase {
    public:
        explicit ControllerBase(std::string_view urlPrefix);
        virtual ~ControllerBase();
    public:
        void Register(crow::SimpleApp& app);

        ROUTE_GET("/home", Home)
        std::string Home();
    protected:
        template<class TReq, class TRes>
        [[nodiscard]] std::function<crow::response(crow::request)> WrapHandler(TRes(ControllerBase::* handler)(TReq)) {
            return [this, handler](crow::request httpReq) -> crow::response {
                try {
                    // TODO: Content-Type별 Json, MessagePack 직렬화

                    nlohmann::json serializedBody = nlohmann::json::parse(httpReq.body);
                    TReq req = serializedBody.get<TReq>();
                    TRes res = (this->*handler)(req);
                    nlohmann::json resJson = res;

                    return crow::response(crow::status::OK, "application/json", resJson.dump());
                }
                catch (const std::exception& exc) {
                    // TODO: Sentry 리포트
                    return crow::response(crow::status::INTERNAL_SERVER_ERROR, "application/json", exc.what());
                }
            };
        }
        template<class TRes>
        [[nodiscard]] std::function<crow::response(crow::request)> WrapHandler(TRes(ControllerBase::* handler)()) {
            return [this, handler](crow::request) -> crow::response {
                try {
                    // TODO: Content-Type별 Json, MessagePack 직렬화

                    TRes res = (this->*handler)();
                    nlohmann::json resJson = res;

                    return crow::response(crow::status::OK, "application/json", resJson.dump());
                }
                catch (const std::exception& exc) {
                    // TODO: Sentry 리포트
                    return crow::response(crow::status::INTERNAL_SERVER_ERROR, "application/json", exc.what());
                }
            };
        }
    private:
        crow::Blueprint _bp;
    };
}