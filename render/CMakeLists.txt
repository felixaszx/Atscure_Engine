set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/exe)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

file(GLOB_RECURSE SRC *.cpp)
add_library(render SHARED ${SRC})

target_link_libraries(render ${GLFW3_LIBRARIES})