#include <Raven/Framework/WebApplication/RpcService.hpp>
#include <crow/app.h>

namespace raven {
    RpcService::RpcService() : _bp(std::make_unique<crow::Blueprint>("rpc")) {
        auto& bp = *_bp;
        CROW_BP_ROUTE(bp, "/")
        .methods(crow::HTTPMethod::POST)
        ([this](crow::request httpReq) {
            return crow::response("text/plain", "This is RPC Method");
        });
    }
    RpcService::~RpcService() = default;
} // namespace raven