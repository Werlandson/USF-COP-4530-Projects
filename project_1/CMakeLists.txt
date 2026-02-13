cmake_minimum_required(VERSION 3.20)

project(SFMLExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML REQUIRED COMPONENTS Graphics Window System)

add_executable(app
    src/main.cpp
    src/Window.cpp
    src/Paths.cpp
    src/Characters.cpp
    src/CircleList.cpp
)

target_link_libraries(app
    PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

