#include <Raven/Framework/WebApplication/RpcService.hpp>

#include  <crow.h>

int main() {
    crow::App<> app;
    raven::RpcService rpcSvc;
    rpcSvc.RegisterRpcEndpoint(app);

    app.port(5000).multithreaded().run();
}