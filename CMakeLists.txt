cmake_minimum_required(VERSION 3.10)
project(thesis)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SIMULATOR_SRC "*.hpp" "*.cpp")

include_directories(.)
add_subdirectory(scenarios)
