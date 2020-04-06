cmake_minimum_required(VERSION 3.12)

project(engine-shared LANGUAGES CXX)

file(GLOB
            CXX "src/*.cxx"
            HXX "include/*.hxx")

add_library(engine-shared SHARED ${CXX})

find_package(SDL2 REQUIRED)

target_include_directories(engine-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(engine-shared PUBLIC cxx_std_17)
target_link_libraries(engine-shared LINK_PRIVATE
    SDL2::SDL2
    SDL2::SDL2main)
