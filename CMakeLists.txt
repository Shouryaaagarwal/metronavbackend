
cmake_minimum_required(VERSION 3.10)
project(metro_api)

set(CMAKE_CXX_STANDARD 17)

include_directories(crow)
include_directories(${PROJECT_SOURCE_DIR})

add_executable(metro_api main.cpp metro_graph.cpp)

find_package(Boost REQUIRED)
target_link_libraries(metro_api PRIVATE Boost::boost pthread)
