set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS off)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
add_compile_options(-Wno-deprecated-declarations)
add_compile_options(-Wno-deprecated-enum-enum-conversion)

find_package(PkgConfig REQUIRED)
pkg_check_modules(VULKAN REQUIRED vulkan)
pkg_check_modules(ASSIMP REQUIRED assimp)
pkg_check_modules(GLFW3 REQUIRED glfw3)

# engine
# engine
# engine
file(GLOB_RECURSE SRC *.cpp)
add_library(atscure_engine_render SHARED ${SRC})

# includes-cpp
target_include_directories(atscure_engine_render PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(atscure_engine_render PUBLIC ${VULKAN_INCLUDE_DIRS})
target_include_directories(atscure_engine_render PUBLIC ${ASSIMP_INCLUDE_DIRS})
target_include_directories(atscure_engine_render PUBLIC ${GLFW3_INCLUDE_DIRS})

# libraries-cpp
target_link_libraries(atscure_engine_render ${VULKAN_LIBRARIES})
target_link_libraries(atscure_engine_render ${ASSIMP_LIBRARIES})
target_link_libraries(atscure_engine_render ${GLFW3_LIBRARIES})





