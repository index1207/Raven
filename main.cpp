#include <crow.h>

int main() {
    crow::SimpleApp app;
    app.port(5000).multithreaded().run();
    return 0;
}