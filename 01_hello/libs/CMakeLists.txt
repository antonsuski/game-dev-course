cmake_minimum_required(VERSION 3.14)
project(hello-lib CXX)
file(GLOB CXX "source/*.cxx"
            H "headers/*.h")

# choisw type of library
add_library(hello-lib-static STATIC ${CXX} ${H})
target_include_directories(hello-lib-static PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/headers)
target_compile_features(hello-lib-static PUBLIC cxx_std_17)

add_library(hello-lib-dynamic SHARED ${CXX} ${H})
target_include_directories(hello-lib-dynamic PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/headers)
target_compile_features(hello-lib-dynamic PUBLIC cxx_std_17)


install(TARGETS hello-lib-static hello-lib-dynamic
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/test-build
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/test-build
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/test-build)
