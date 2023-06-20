cmake_minimum_required(VERSION 3.20.0)
project(ae VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/exe)

find_package(PkgConfig REQUIRED)

file(GLOB_RECURSE SRC src/*.cpp)
add_executable(ae ${SRC})
add_subdirectory(vk_api)

target_include_directories(ae PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(ae vk_api)