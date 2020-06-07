cmake_minimum_required(VERSION 3.17)

project(engine-core-shared LANGUAGES CXX C)

#file(GLOB
#            CXX "src/*.cxx"
#            HXX "include/*.hxx"
#            C "src/*.c"
#           H "include/*.h")

add_library(engine-core-shared SHARED src/engine.cxx
    include/engine.hxx
    include/glad/glad.c
    include/glad/glad.h
    src/shader.cxx
    include/shader.hxx
    include/om_gl_check.hxx
    include/picopng.hxx)

find_package(SDL2 REQUIRED)

target_compile_features(engine-core-shared PUBLIC cxx_std_17)
target_include_directories(engine-core-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(engine-core-shared PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    GL)
