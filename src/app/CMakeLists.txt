add_executable(tetris)

target_sources(
    tetris
        PRIVATE
            main.cpp
)

target_link_libraries(
    tetris
        PRIVATE
            project_options
            tetrislib
            cursespp
)
