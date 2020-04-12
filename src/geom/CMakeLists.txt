add_library(geom)

target_sources(
    geom
        PUBLIC
            matrix.hpp
            unreachable.hpp

        PRIVATE
            matrix.cpp
)

target_include_directories(
    geom
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)
