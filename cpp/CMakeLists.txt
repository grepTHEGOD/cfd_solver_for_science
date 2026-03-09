cmake_minimum_required(VERSION 3.16)
project(cfd_studio_cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_OPENGL "Enable OpenGL rendering" ON)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)

pkg_check_modules(GLFW3 REQUIRED glfw3)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GLFW3_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
)

set(SOURCES
    src/main.cpp
    src/app/application.cpp
    src/app/scene.cpp
    src/physics/atmosphere.cpp
    src/physics/forces.cpp
    src/physics/heat_transfer.cpp
    src/physics/units.cpp
    src/rendering/renderer.cpp
    src/rendering/shader.cpp
    src/rendering/mesh.cpp
    src/rendering/camera.cpp
    src/rendering/gl_functions.cpp
    src/geometry/scramjet_geometry.cpp
    src/visualization/streamlines.cpp
    src/visualization/contours.cpp
    src/visualization/force_arrows.cpp
    src/ui/ui_manager.cpp
)

set(HEADERS
    include/app/application.hh
    include/app/scene.hh
    include/physics/atmosphere.hh
    include/physics/forces.hh
    include/physics/heat_transfer.hh
    include/physics/units.hh
    include/rendering/renderer.hh
    include/rendering/shader.hh
    include/rendering/mesh.hh
    include/rendering/camera.hh
    include/geometry/scramjet_geometry.hh
    include/visualization/streamlines.hh
    include/visualization/contours.hh
    include/visualization/force_arrows.hh
    include/ui/ui_manager.hh
    include/utils/logger.hh
    include/utils/math.hh
)

add_executable(cfd_studio_cpp ${SOURCES} ${HEADERS})

target_link_libraries(cfd_studio_cpp
    ${GLFW3_LIBRARIES}
    OpenGL::GL
    pthread
    dl
)

target_compile_options(cfd_studio_cpp PRIVATE
    -Wall -Wextra -Wpedantic
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g -O0>
)

install(TARGETS cfd_studio_cpp DESTINATION bin)
