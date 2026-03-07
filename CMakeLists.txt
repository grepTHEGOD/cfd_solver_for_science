cmake_minimum_required(VERSION 3.10)
project(cfd_studio VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -g")
set(CMAKE_C_FLAGS_DEBUG "-O0")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(EPOXY REQUIRED epoxy)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${GTK3_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${EPOXY_INCLUDE_DIRS}
)

set(SOURCES
    src/main.c
    src/cfd_app.c
    src/ui/main_window.c
    src/ui/menu_bar.c
    src/ui/toolbar.c
    src/ui/object_browser.c
    src/ui/properties_panel.c
    src/ui/control_panel.c
    src/ui/status_bar.c
    src/viewport/gl_viewport.c
    src/viewport/camera.c
    src/viewport/shader.c
    src/viewport/mesh_renderer.c
    src/models/mesh.c
    src/models/stl_loader.c
    src/models/obj_loader.c
    src/models/model_manager.c
    src/viz/colormap.c
    src/viz/contour.c
    src/viz/vectors.c
    src/viz/streamlines.c
    src/viz/force_arrows.c
    src/physics/units.c
    src/physics/atmosphere.c
    src/physics/forces.c
    src/utils/math_utils.c
    src/utils/logger.c
)

set(HEADERS
    src/cfd_app.h
    src/ui/main_window.h
    src/ui/menu_bar.h
    src/ui/toolbar.h
    src/ui/object_browser.h
    src/ui/properties_panel.h
    src/ui/control_panel.h
    src/ui/status_bar.h
    src/viewport/gl_viewport.h
    src/viewport/camera.h
    src/viewport/shader.h
    src/viewport/mesh_renderer.h
    src/models/mesh.h
    src/models/stl_loader.h
    src/models/obj_loader.h
    src/models/model_manager.h
    src/viz/colormap.h
    src/viz/contour.h
    src/viz/vectors.h
    src/viz/streamlines.h
    src/viz/force_arrows.h
    src/physics/units.h
    src/physics/atmosphere.h
    src/physics/forces.h
    src/utils/math_utils.h
    src/utils/logger.h
)

add_executable(cfd_studio ${SOURCES} ${HEADERS})

target_link_libraries(cfd_studio
    ${GTK3_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${EPOXY_LIBRARIES}
    m
    dl
)

target_compile_options(cfd_studio PRIVATE ${GTK3_CFLAGS_OTHER})

install(TARGETS cfd_studio DESTINATION bin)
