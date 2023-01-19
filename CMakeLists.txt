cmake_minimum_required(VERSION 3.18)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(Pong main.c pong.c pong.h)

target_link_libraries(Pong -lncurses)