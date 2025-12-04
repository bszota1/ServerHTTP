cmake_minimum_required(VERSION 3.20)
project(serverHTTP)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
add_executable(serverHTTP
        src/main.cpp
        src/server/listener.cpp
        include/listener.h
        src/server/session.cpp
        include/session.h
        src/server/router.cpp
        include/router.h)

target_include_directories(serverHTTP PRIVATE include)
target_link_libraries(serverHTTP PRIVATE Boost::headers nlohmann_json::nlohmann_json)
if(UNIX)
    target_link_libraries(serverHTTP PRIVATE pthread)
endif()

add_custom_command(TARGET serverHTTP POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/index.html
        ${CMAKE_CURRENT_BINARY_DIR}/index.html
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/db.json
        ${CMAKE_CURRENT_BINARY_DIR}/db.json
)