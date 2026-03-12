cmake_minimum_required(VERSION 3.10)
project(Visualization)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include(FindPkgConfig)
pkg_check_modules(RAYLIB QUIET raylib)

if(NOT RAYLIB_FOUND)
    message(FATAL_ERROR "raylib not found")
endif()

add_executable(visualization main.c)

target_include_directories(visualization PRIVATE ${RAYLIB_INCLUDE_DIRS})
target_link_libraries(visualization ${RAYLIB_LIBRARIES} m pthread dl)

install(TARGETS visualization DESTINATION bin)
