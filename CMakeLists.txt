cmake_minimum_required(VERSION 4.0)
project(Raven)

set(CMAKE_CXX_STANDARD 20)

# dependencies
find_package(Crow CONFIG REQUIRED)
find_package(reflectcpp CONFIG REQUIRED)
find_package(sentry CONFIG REQUIRED)

set(INCLUDE_ROOT ${PROJECT_SOURCE_DIR}/include/Raven)
set(SOURCE_ROOT ${PROJECT_SOURCE_DIR}/src/Raven)

add_subdirectory(src/Raven)
add_subdirectory(test)
add_subdirectory(example)