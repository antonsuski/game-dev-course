cmake_minimum_required(VERSION 3.12)

project(03-game-loop-shared LANGUAGES CXX)

add_executable(03-game-loop-shared game.cxx)

target_link_libraries(03-game-loop-shared LINK_PUBLIC
    engine-shared-0)
