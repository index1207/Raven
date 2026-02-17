#include "Raven/Framework/WebApplication/ControllerBase.hpp"
#include "Raven/Framework/WebApplication/WebHost.hpp"

class HomeController : public raven::ControllerBase {
public:
    RAVEN_CONTROLLER(HomeController, "home")
public:
    ROUTE_GET("/", IndexGet)
    std::string IndexGet(std::vector<int> list) {
        return "GET";
    }

    ROUTE_POST("/", IndexPost)
    std::string IndexPost() {
        return "POST";
    }
};

int main() {
    raven::WebHost webHost;

    webHost.AddController<HomeController>();

    webHost.Run(18080);
}