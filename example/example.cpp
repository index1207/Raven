#include "Raven/Framework/WebApplication/ControllerBase.hpp"

class HomeController : public raven::ControllerBase {
public:
    // 1. 클래스 이름과 Blueprint Prefix를 한 번에 지정합니다.
    RAVEN_CONTROLLER(HomeController, "home")

public:
    // 2. 메서드 선언 바로 위에 어트리뷰트처럼 사용합니다.
    ROUTE_GET("/", Index)
    std::string Index(std::vector<int> list) {
        return "GET Success";
    }

    ROUTE_POST("/submit", SubmitData)
    std::string SubmitData(nlohmann::json data) {
        return "POST Success";
    }

    ROUTE_DELETE("/remove", Remove)
    std::string Remove() {
        return "DELETE Success";
    }
};

int main() {
    crow::SimpleApp app;
    HomeController controller;
    controller.Register(app); // 내부에서 모든 라우트가 자동 등록됨

    app.port(8989).run();
}