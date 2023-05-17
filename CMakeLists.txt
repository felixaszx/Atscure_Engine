cmake_minimum_required(VERSION 3.20.0)
project(atscure_engine VERSION 0.1.0)

add_subdirectory(render)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_compile_options(-Wno-deprecated-declarations)
add_compile_options(-Wno-deprecated-enum-enum-conversion)

# engine
file(GLOB_RECURSE SRC src/*.cpp)
add_executable(atscure_engine ${SRC})

target_include_directories(atscure_engine PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(atscure_engine atscure_engine_render)





