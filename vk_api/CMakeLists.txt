set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_C_EXTENSIONS off)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/exe)

find_package(PkgConfig REQUIRED)
pkg_check_modules(VULKAN REQUIRED vulkan)
pkg_check_modules(GLFW3 REQUIRED glfw3)

file(GLOB_RECURSE SRC *.cpp *.c)
add_library(vk_api SHARED ${SRC})

target_link_libraries(vk_api ${VULKAN_LIBRARIES})
target_link_libraries(vk_api ${GLFW3_LIBRARIES})