cmake_minimum_required(VERSION 3.28)
project(Pathfinding LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.92.6
        GIT_SHALLOW ON)
FetchContent_MakeAvailable(imgui)

FetchContent_Declare(imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG master
        GIT_SHALLOW ON)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_STATIC ON)
FetchContent_MakeAvailable(imgui-sfml)

add_executable(main
        src/core/main.cpp
        src/core/game.cpp
        src/grid/cell.cpp
        src/grid/grid.cpp
        src/pathfinding/snapshot.cpp
        src/pathfinding/agent.cpp
)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)