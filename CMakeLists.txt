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
    src/sounds.cpp
)

target_include_directories(MyProgram
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(MyProgram PRIVATE cxx_std_20)


add_subdirectory(lib/SDL)

target_link_libraries(MyProgram
    PRIVATE
    SDL3::SDL3
)