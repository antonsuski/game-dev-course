cmake_minimum_required(VERSION 3.17)

#project(engine-shared LANGUAGES CXX C)

#file(GLOB
#            CXX "src/*.cxx"
#            HXX "include/*.hxx"
#            C "src/*.c"
#           H "include/*.h")

add_library(engine-shared SHARED src/engine.cxx
    include/engine.hxx
    include/glad/glad.c
    include/glad/glad.h)

find_package(SDL2 REQUIRED)

target_compile_features(engine-shared PUBLIC cxx_std_17)
target_include_directories(engine-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(engine-shared PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    GL)
