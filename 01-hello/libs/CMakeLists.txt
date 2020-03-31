cmake_minimum_required(VERSION 3.12)

project(hello-lib)

file(GLOB CXX "src/*.cxx"
            H "include/*.h")

#You should to choise compile target: static or shared
#-------------------SHARED---------------------------
add_library(hello-lib-shared SHARED ${CXX} ${H})
target_include_directories(hello-lib-shared PUBLIC
                          "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(hello-lib-shared PRIVATE cxx_std_11)

#-------------------STATIC---------------------------
add_library(hello-lib-static STATIC ${CXX} ${H})
target_include_directories(hello-lib-static PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello-lib-static PRIVATE cxx_std_17)
