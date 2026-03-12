cmake_minimum_required(VERSION 3.10)
project(TUI)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(tui_executable main.c)

install(TARGETS tui_executable DESTINATION bin)
