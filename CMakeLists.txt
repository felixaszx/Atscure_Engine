cmake_minimum_required(VERSION 3.20.0)
project(atscure_engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS off)

file(GLOB_RECURSE SRC_CPP src/*.cpp src_app/*.cpp)
add_executable(atscure_engine ${SRC_CPP})

# includes-cpp
target_include_directories(atscure_engine PUBLIC include)

# libraries-cpp
target_link_libraries(atscure_engine vulkan-1)
target_link_libraries(atscure_engine assimp)
target_link_libraries(atscure_engine glfw3)
target_link_libraries(atscure_engine fmt)

