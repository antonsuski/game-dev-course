cmake_minimum_required(VERSION 3.16)
project(hello-bin CXX)
file(GLOB CXX "*.cxx")
option(STATIC_BUILD "if not =ON project will be build dynamic" OFF)

if (NOT MINGW)
    message("try to fix MinGW64 static linking error for F31 MinGW 6.0")

    add_executable(hello-bin-static ${CXX})
    target_compile_features(hello-bin-static PUBLIC cxx_std_14)
    target_include_directories(hello-bin-static PUBLIC ~/../../usr/include)
    target_link_libraries(hello-bin-static LINK_PUBLIC hello-lib-static)
    target_link_options(hello-bin-static PUBLIC -static)
endif()

add_executable(hello-bin-dynamic ${CXX})
target_include_directories(hello-bin-dynamic PUBLIC ~/../../usr/include)
target_compile_features(hello-bin-dynamic PUBLIC cxx_std_14)
target_link_libraries(hello-bin-dynamic LINK_PUBLIC hello-lib-dynamic)

install(TARGETS hello-bin-dynamic
        RUNTIME DESTINATION ${CMAKE_CURRENT_LIST_DIR}/test-build
        LIBRARY DESTINATION ${CMAKE_CURRENT_LIST_DIR}/test-build
        ARCHIVE DESTINATION ${CMAKE_CURRENT_LIST_DIR}/test-build)
