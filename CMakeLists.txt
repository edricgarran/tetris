cmake_minimum_required(VERSION 3.15)
project(tetris CXX)

include(cmake/base.cmake)
include(cmake/project_options.cmake)
include(cmake/conan.cmake)

conan(
    PACKAGES
    ncurses/6.1@conan/stable
)

add_subdirectory(src)
