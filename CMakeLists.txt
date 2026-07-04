cmake_minimum_required(VERSION 3.16)
project(untitled6)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(untitled6 main.cpp)
target_link_libraries(untitled6 SFML::Graphics SFML::Window SFML::System)