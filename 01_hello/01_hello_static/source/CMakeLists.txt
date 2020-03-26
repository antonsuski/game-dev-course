cmake_minimum_required(VERSION 3.12)
file(GLOB CXX "*.cxx")

add_executable(${PROJECT_NAME} ${CXX})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC hello_lib)
