cmake_minimum_required(VERSION 3.23)
project(mine_sweeper)

add_executable(MyProgram)

target_sources(MyProgram
    PRIVATE
        src/main.cpp
        src/game.cpp
        # src/ui.cpp
)

target_include_directories(MyProgram
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(MyProgram PRIVATE cxx_std_20)


# add_library(MyLibrary)
# target_sources(MyLibrary
#     PRIVATE
#     src/MyLibrary.cpp

#     PUBLIC
#     FILE_SET myHeaders
#     TYPE HEADERS
#     BASE_DIRS
#     include
#     FILES
#     include/MyLibrary.h
# )

# add_subdirectory(lib/eigen)
# add_subdirectory(lib/sdl)
# add_subdirectory(lib/**)

add_subdirectory(lib)
# add_subdirectory(lib/sdl)

# if(TARGET SDL3::SDL3)
#   add_custom_command(TARGET SDL3::SDL3 POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#       $<TARGET_FILE:SDL3::SDL3>
#       ${CMAKE_BINARY_DIR}
#   )
# endif()

target_link_libraries(MyProgram
    PRIVATE
        # imgui
        SDL3::SDL3
        SDL3_image::SDL3_image
)