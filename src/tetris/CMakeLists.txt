add_executable(tetris)

target_sources(
    tetris
        PRIVATE
            main.cpp
            matrix.cpp
            matrix.hpp
            tetriminoes.cpp
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
