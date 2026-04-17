cmake_minimum_required(VERSION 3.10)
project(CFD_Solver_For_Science)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3")

include(FindPkgConfig)
pkg_check_modules(RAYLIB QUIET raylib)

add_subdirectory(solver)
add_subdirectory(tui)
add_subdirectory(gui)
add_subdirectory(scramjet)

if(RAYLIB_FOUND)
    add_subdirectory(visualization)
    message(STATUS "raylib found - visualization enabled")
else()
    message(STATUS "raylib not found - visualization disabled")
endif()

add_custom_target(run_solver
    COMMAND solver/cfd_solver
    DEPENDS cfd_solver
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(run_tui
    COMMAND tui/tui_executable
    DEPENDS tui_executable
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(run_gui
    COMMAND gui/gui_executable
    DEPENDS gui_executable
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

if(RAYLIB_FOUND)
    add_custom_target(run_visualization
        COMMAND visualization/visualization
        DEPENDS visualization
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()
