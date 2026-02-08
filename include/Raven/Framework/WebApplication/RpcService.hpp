#pragma once
#include <memory>

#include <crow/app.h>

namespace raven {
    class RpcService {
    public:
        RpcService();
        ~RpcService();
    public:
        template<typename... Middlewares>
        void RegisterRpcEndpoint(crow::App<Middlewares...>& app) {
            auto& bp = *_bp;
            app.register_blueprint(bp);
        }
    private:
        std::unique_ptr<crow::Blueprint> _bp;
    };
}