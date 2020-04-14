cmake_minimum_required(VERSION 3.12)

project(canvas LANGUAGES CXX)

add_library(canvas SHARED canvas.cxx)
add_library(vertex SHARED vertex.cxx)

target_include_directories(canvas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(canvas PUBLIC cxx_std_17)
