add_library(util)

target_sources(
    util
        PUBLIC
            containers.hpp
            unreachable.hpp

        PRIVATE
            containers.cpp
            unreachable.cpp
)

target_include_directories(
    util
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)
