cmake_minimum_required(VERSION 3.17)

project(05-engine-refactor LANGUAGES CXX C)

add_executable(05-engine-refactor game.cxx)

target_compile_features(05-engine-refactor PUBLIC cxx_std_17)
target_link_libraries(05-engine-refactor PUBLIC
    engine-shared)
