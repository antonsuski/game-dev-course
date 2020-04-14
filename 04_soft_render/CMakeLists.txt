cmake_minimum_required(VERSION 3.12)

project(04-soft-render LANGUAGES CXX)

add_subdirectory(render)

file(GLOB
            CXX "*.cxx")

add_executable(04-soft-render ${CXX})

target_link_libraries(04-soft-render LINK_PRIVATE
                        canvas
                        vertex)
