cmake_minimum_required(VERSION 3.12)

project(02-sdl CXX)

file(GLOB CXX "*.cxx")

#------------------Shared link-----------------------------
add_executable(${PROJECT_NAME}-shared ${CXX})
target_compile_features(${PROJECT_NAME}-shared PUBLIC cxx_std_17)
find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME}-shared PRIVATE SDL2::SDL2 SDL2::SDL2main)
