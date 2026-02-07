cmake_minimum_required(VERSION 4.0)
project(Raven)

set(CMAKE_CXX_STANDARD 20)

# dependencies
find_package(Crow CONFIG REQUIRED)
find_package(sentry CONFIG REQUIRED)

add_subdirectory(src/Raven)
add_subdirectory(test)
add_subdirectory(example)