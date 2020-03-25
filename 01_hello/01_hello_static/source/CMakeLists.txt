cmake_minimum_required(VERSION 3.16)
file(GLOB CXX "*.cxx")

add_executable(${PROJECT_NAME} ${CXX})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC hello_lib)
