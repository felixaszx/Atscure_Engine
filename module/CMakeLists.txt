set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_library(all_modules _all.cxx)

file(GLOB_RECURSE MODULES *.cpp)
foreach(MODULE_FILE ${MODULES})
    get_filename_component(MODULE_NAME ${MODULE_FILE} NAME_WLE)
    add_library(${MODULE_NAME} SHARED ${MODULE_FILE})
    set_target_properties(${MODULE_NAME} PROPERTIES PREFIX "m_")
    target_include_directories(${MODULE_NAME} PUBLIC include)
    target_include_directories(${MODULE_NAME} PUBLIC include_app)
    target_link_libraries(${MODULE_NAME} atscure_engine)
    target_link_libraries(all_modules ${MODULE_NAME})
endforeach()