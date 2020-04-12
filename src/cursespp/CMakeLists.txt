add_library(ncurses INTERFACE)

target_link_libraries(
    ncurses
        INTERFACE
            CONAN_PKG::ncurses
            ${CMAKE_DL_LIBS}
)

add_library(
    cursespp
)

target_sources(
    cursespp
        INTERFACE
            cursespp.hpp
        PRIVATE
            cursespp.cpp
)

target_include_directories(
    cursespp
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    cursespp
        PUBLIC
            assertpp
            ncurses
        PRIVATE
            project_options
)
