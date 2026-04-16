cmake_minimum_required(VERSION 3.23)
project(mine_sweeper)

add_executable(MineSweeper)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_sources(MineSweeper
    PRIVATE
    src/main.cpp
    src/game.cpp
    src/graphics.cpp
    src/sounds.cpp
)

target_include_directories(MineSweeper
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(MineSweeper PRIVATE cxx_std_20)


add_subdirectory(lib/sdl
)

target_link_libraries(MineSweeper
    PRIVATE
    SDL3::SDL3
)