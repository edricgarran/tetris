add_executable(tetris)

target_sources(
    tetris
        PRIVATE
            main.cpp
            matrix.hpp
            tetriminoes.hpp
            unreachable.hpp
)

target_link_libraries(
    tetris
        PRIVATE
            project_options
            assertpp
            cursespp
)
