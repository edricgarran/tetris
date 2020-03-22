add_executable(tetris)

target_sources(
    tetris
        PRIVATE
            matrix.hpp
            main.cpp
            unreachable.hpp
)

target_link_libraries(
    tetris
        PRIVATE
            project_options
            cursespp
)
