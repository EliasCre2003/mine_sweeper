cmake_minimum_required(VERSION 3.23)
project(mine_sweeper)

add_executable(MyProgram)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_sources(MyProgram
    PRIVATE
    src/main.cpp
    src/game.cpp
    src/graphics.cpp

    # src/ui.cpp
)

target_include_directories(MyProgram
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(MyProgram PRIVATE cxx_std_20)

# add_library(MyLibrary)
# target_sources(MyLibrary
# PRIVATE
# src/MyLibrary.cpp

# PUBLIC
# FILE_SET myHeaders
# TYPE HEADERS
# BASE_DIRS
# include
# FILES
# include/MyLibrary.h
# )

# add_subdirectory(lib/eigen)
# add_subdirectory(lib/sdl)
# add_subdirectory(lib/**)

# set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# set(SDL_SHARED OFF CACHE BOOL "" FORCE)
# set(SDL_STATIC ON  CACHE BOOL "" FORCE)

# set(SDLIMAGE_DEPS_SHARED OFF CACHE BOOL "" FORCE)
# set(SDLIMAGE_VENDORED ON CACHE BOOL "" FORCE)
add_subdirectory(lib)

# add_subdirectory(lib/sdl)

# if(TARGET SDL3::SDL3)
# add_custom_command(TARGET SDL3::SDL3 POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy_if_different
# $<TARGET_FILE:SDL3::SDL3>
# ${CMAKE_BINARY_DIR}
# )
# endif()
target_link_libraries(MyProgram
    PRIVATE

    # imgui
    SDL3::SDL3
    SDL3_image::SDL3_image
)