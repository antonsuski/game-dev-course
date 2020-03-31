cmake_minimum_required(VERSION 3.12)

file(GLOB
            CXX "*.cxx")

add_executable(${PROJECT_NAME}-static ${CXX})
target_compile_features(${PROJECT_NAME}-static PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME}-static LINK_PUBLIC hello-lib-static)
#target_link_options(${PROJECT_NAME}-static PRIVATE -static)

add_executable(${PROJECT_NAME}-shared ${CXX})
target_compile_features(${PROJECT_NAME}-shared PUBLIC cxx_std_17)
target_link_libraries(${PROJECT_NAME}-shared LINK_PUBLIC hello-lib-shared)
