#pragma once
#include <crow/app.h>

namespace raven {
    class ControllerBase;

    class WebHost {
    public:
        WebHost() = default;
    public:
        void Run(ushort port);

        template<class TController, class... TArgs>
        requires std::is_base_of_v<ControllerBase, TController>
        void AddController(TArgs&&... args) {
            auto controller = std::make_shared<TController>(std::forward<TArgs>(args)...);
            controller->Register(_app);

            _controllerList.emplace_back(controller);
        }
    private:
        crow::SimpleApp _app;
        std::vector<std::shared_ptr<ControllerBase>> _controllerList;
    };
}
