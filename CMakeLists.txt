cmake_minimum_required(VERSION 3.10)
project(cfd_studio VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")
set(CMAKE_C_FLAGS_DEBUG "-O0")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

set(ENV{PKG_CONFIG_PATH} "/home/grep/local/home/grep/local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED raylib)

# Ensure raylib include directories and library paths are properly set
include_directories(${RAYLIB_INCLUDE_DIRS})
link_directories(${RAYLIB_LIBRARY_DIRS})

# Add include paths
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/physics)

# Add raylib library explicitly
set(RAYLIB_LIB /home/grep/local/home/grep/local/lib/libraylib.so)

# Define the cfd_studio target as an executable
set(SOURCES
    src/main.c
    src/cfd_app.c
    src/ui/gui_framework.c
    src/ui/control_panel.c
    src/viewport/viewport_3d.c
    src/physics/heat_transfer.c
    src/physics/atmosphere.c
    src/physics/forces.c
    src/physics/units.c
    src/models/model_manager.c
)

set(HEADERS
    src/cfd_app.h
    src/ui/gui_framework.h
    src/ui/control_panel.h
    src/viewport/viewport_3d.h
    src/physics/heat_transfer.h
    src/physics/units.h
    src/physics/atmosphere.h
    src/physics/forces.h
    src/models/model_manager.h
    src/models/mesh.h
    src/models/obj_loader.h
    src/models/stl_loader.h
    src/utils/logger.h
    src/utils/math_utils.h
    src/viz/colormap.h
    src/viz/contour.h
    src/viz/force_arrows.h
    src/viz/streamlines.h
    src/viz/vectors.h
)

add_executable(cfd_studio ${SOURCES} ${HEADERS})

target_link_libraries(cfd_studio ${RAYLIB_LIB} m)

target_compile_options(cfd_studio PRIVATE ${RAYLIB_CFLAGS_OTHER})

install(TARGETS cfd_studio DESTINATION bin)
