cmake_minimum_required(VERSION 2.6)
project(test_slam)

add_executable(test_slam main.cpp)

install(TARGETS test_slam RUNTIME DESTINATION bin)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-O3")

include_directories("/usr/include/eigen3")
