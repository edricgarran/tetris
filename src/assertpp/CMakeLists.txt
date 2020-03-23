option(ASSERTPP_ENABLE "Enable asserts." TRUE)

add_library(assertpp)

target_sources(
    assertpp
        PUBLIC
            assert.hpp
        PRIVATE
            assert.cpp
)

target_include_directories(
    assertpp
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}
)

if (ASSERTPP_ENABLE)
    target_compile_definitions(
        assertpp
            PUBLIC
                ASSERTPP_ENABLE
    )
endif()

target_link_libraries(
    assertpp
        PRIVATE
            project_options
)
