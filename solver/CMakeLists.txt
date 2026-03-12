cmake_minimum_required(VERSION 3.10)
project(CFD_Solver)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O3 -march=native")

add_executable(cfd_solver 
    main.c 
    cfd_solver.c
)

target_link_libraries(cfd_solver m)

install(TARGETS cfd_solver DESTINATION bin)
