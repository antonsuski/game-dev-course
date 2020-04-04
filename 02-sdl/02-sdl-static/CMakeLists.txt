cmake_minimum_required(VERSION 3.12)

project(02-sdl CXX)

file(GLOB CXX "*.cxx")

#------------------Static link-----------------------------
add_executable(${PROJECT_NAME}-static ${CXX})
target_compile_features(${PROJECT_NAME}-static PUBLIC cxx_std_17)

find_package(SDL2 REQUIRED)
find_library(SDL2lib libSDL2d.a)

target_include_directories(${PROJECT_NAME}-static PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}-static
               ${SDL2lib}
#               -lm
#               -ldl
#               -lpthread
#               -lrt
                SDL2::SDL2-static
               )

