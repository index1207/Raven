#include <Raven/Framework/WebApplication/ControllerBase.hpp>

#include  <crow.h>

int main() {
    crow::App<> app;
    auto conBase = new raven::ControllerBase("base");
    conBase->Register(app);

    app.port(8989).multithreaded().run();
}