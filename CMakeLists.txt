cmake_minimum_required(VERSION 3.15)
project(csapp)

set(CMAKE_CXX_STANDARD 14)


include_directories(include)
add_executable(main main.cpp)
add_library(common common/csapp.c)

link_libraries(common pthread)

add_subdirectory(10_io)

