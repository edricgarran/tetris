add_executable(main)

target_sources(
    main
        PRIVATE
            matrix.hpp
            main.cpp
)

target_link_libraries(
    main
        PRIVATE
            project_options
)
