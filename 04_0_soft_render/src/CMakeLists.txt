cmake_minimum_required(VERSION 3.17)

add_library(canvas SHARED canvas.cxx canvas.hxx)
add_library(render SHARED render.cxx render.hxx)
target_include_directories(canvas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(canvas PUBLIC cxx_std_17)
target_compile_features(render PUBLIC cxx_std_17)
