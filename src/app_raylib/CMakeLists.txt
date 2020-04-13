include(FetchContent)

set(OpenGL_GL_PREFERENCE GLVND)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib
    GIT_SHALLOW 1
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(raylib)

add_executable(tetris_raylib)

target_sources(
    tetris_raylib
        PRIVATE
            main.cpp
)

target_link_libraries(
    tetris_raylib
        PRIVATE
            project_options
            tetrislib
            assertpp
            raylib
)
