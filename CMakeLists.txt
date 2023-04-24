cmake_minimum_required(VERSION 3.20.0)
project(atscure_engine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(VULKAN REQUIRED vulkan)
pkg_check_modules(ASSIMP REQUIRED assimp)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(FMT REQUIRED fmt)
pkg_check_modules(LUA REQUIRED lua)

# engine
# engine
# engine
file(GLOB_RECURSE SRC src/*.cpp)
add_library(atscure_engine SHARED ${SRC})

# includes-cpp
target_include_directories(atscure_engine PUBLIC include)
target_include_directories(atscure_engine PUBLIC ${VULKAN_INCLUDE_DIRS})
target_include_directories(atscure_engine PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_include_directories(atscure_engine PUBLIC ${GLFW3_INCLUDE_DIRS})
target_include_directories(atscure_engine PUBLIC ${FMT_INCLUDE_DIRS})
target_include_directories(atscure_engine PUBLIC ${LUA_INCLUDE_DIRS})

# libraries-cpp
target_link_libraries(atscure_engine ${VULKAN_LIBRARIES})
target_link_libraries(atscure_engine ${ASSIMP_LIBRARIES})
target_link_libraries(atscure_engine ${GLFW3_LIBRARIES})
target_link_libraries(atscure_engine ${FMT_LIBRARIES})
target_link_libraries(atscure_engine ${LUA_LIBRARIES})
target_link_libraries(atscure_engine ${LUA_LIBRARIES})
target_link_libraries(atscure_engine ode_double)

# example app
# example app
# example app
file(GLOB_RECURSE SRC_APP src_app/*.cpp)
add_executable(atscure_testing ${SRC_APP})

# includes-cpp
target_include_directories(atscure_testing PUBLIC include)
target_include_directories(atscure_testing PUBLIC include_app)

# libraries-cpp
target_link_libraries(atscure_testing atscure_engine)

# script
# script
# script
add_subdirectory(module)

file(GLOB_RECURSE SRC_TEST src_test/*.cpp)
add_executable(test2 ${SRC_TEST})
target_include_directories(test2 PUBLIC include)
target_include_directories(test2 PUBLIC include_app)
target_link_libraries(test2 atscure_engine)




