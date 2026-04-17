cmake_minimum_required(VERSION 3.10)
project(GUI)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3")

find_library(NCURSES_LIBRARY ncurses)
if(NOT NCURSES_LIBRARY)
    message(FATAL_ERROR "ncurses not found! Install libncurses-dev")
endif()

add_library(physics_core STATIC
    ../core/physics_engine.cpp
    ../core/physics_bridge.cpp
    ../core/model_loader.cpp
)

add_executable(gui_executable
    main.c
)

target_include_directories(gui_executable PRIVATE ${CMAKE_SOURCE_DIR}/core)
target_link_libraries(gui_executable physics_core ${NCURSES_LIBRARY} m)

install(TARGETS gui_executable DESTINATION bin)