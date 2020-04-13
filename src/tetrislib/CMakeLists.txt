add_library(tetrislib)

target_sources(
    tetrislib
        PUBLIC
            board.hpp
            block_type.hpp
            tetriminoes.hpp
            tetris.hpp

        PRIVATE
            board.cpp
            block_type.cpp
            tetriminoes.cpp
            tetris.cpp
)

target_include_directories(
    tetrislib
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    tetrislib
        PUBLIC
            geom
            util

        PRIVATE
            project_options
)
