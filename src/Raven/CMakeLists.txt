# Subdirectories
add_subdirectory(Framework/Sentry)
add_subdirectory(Framework/WebApplication)

file(GLOB_RECURSE SRC_FILES "${SOURCE_ROOT}/**/*.cpp")
file(GLOB_RECURSE INC_FILES "${INCLUDE_ROOT}/**/*.hpp")

add_library("${CMAKE_PROJECT_NAME}" STATIC "${SRC_FILES}" "${INC_FILES}")

target_include_directories("${CMAKE_PROJECT_NAME}"
        PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries("${CMAKE_PROJECT_NAME}"
        PRIVATE
            Crow::Crow
            asio::asio
            reflectcpp::reflectcpp
            sentry::sentry
)
