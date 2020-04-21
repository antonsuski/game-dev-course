cmake_minimum_required(VERSION 3.17)

add_executable(04-soft-render main.cxx)
target_link_libraries(04-soft-render PUBLIC canvas render)
