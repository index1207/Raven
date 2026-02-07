cmake_minimum_required(VERSION 4.0)
project(Raven)

set(CMAKE_CXX_STANDARD 20)

add_executable(Raven main.cpp)

find_package(Crow CONFIG REQUIRED)
target_link_libraries(Raven PRIVATE Crow::Crow asio::asio)
