cmake_minimum_required(VERSION 3.17)

#project(04-opengl-shared LANGUAGES CXX C)

add_executable(04-opengl-shared game.cxx)

target_compile_features(04-opengl-shared PUBLIC cxx_std_17)
target_link_libraries(04-opengl-shared PUBLIC
    engine-shared)
