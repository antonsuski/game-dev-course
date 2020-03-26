cmake_minimum_required(VERSION 3.12)
project(hello_lib)
file(GLOB CXX "source/*.cxx"
            H "headers/*.h")

add_library(${PROJECT_NAME} SHARED ${CXX} ${H})
target_include_directories(${PROJECT_NAME} PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/headers)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
