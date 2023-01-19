cmake_minimum_required(VERSION 3.18)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Pong main.c pong.c pong.h server.c klient.c klient.h server.h k_s_definitions.h k_s_definitions.c)

target_link_libraries(Pong -lncurses)