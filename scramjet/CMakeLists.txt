add_executable(scramjet main.c scramjet_config.c)
target_link_libraries(scramjet m)
target_include_directories(scramjet PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(run_scramjet
    COMMAND scramjet
    DEPENDS scramjet
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)