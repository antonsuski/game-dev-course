cmake_minimum_required(VERSION 3.17)

#project(engine-shared LANGUAGES CXX C)

#file(GLOB
#            CXX "src/*.cxx"
#            HXX "include/*.hxx"
#            C "src/*.c"
#           H "include/*.h")

add_library(engine-shared SHARED
    src/engine.cxx
    src/shader.cxx
    src/texture2d.cxx
    src/stuff.cxx
    src/stb_image.cxx
    include/engine.hxx
    include/glad/glad.c
    include/glad/glad.h
    include/shader.hxx
    include/om_gl_check.hxx
    include/picopng.hxx
    include/stb_image.h
    include/texture2d.hxx
    include/stuff.hxx)

find_package(SDL2 REQUIRED)

target_compile_features(engine-shared PUBLIC cxx_std_17)
target_include_directories(engine-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(engine-shared PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    GL)
