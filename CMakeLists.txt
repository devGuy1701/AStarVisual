cmake_minimum_required(VERSION 3.14)
project(AStarVisual)

set(CMAKE_CXX_STANDARD 11)
add_definitions(-std=gnu++0x)

add_subdirectory(test)
set(SOURCE_FILES main.cpp)
add_executable(AStarVisual main.cpp)
add_library(core ${SOURCE_FILES})
target_link_libraries(AStarVisual core)


find_package(SFML 2 COMPONENTS network audio graphics window system REQUIRED)
if(SFML_FOUND)
    target_link_libraries(AStarVisual sfml-graphics sfml-window sfml-system)
endif()