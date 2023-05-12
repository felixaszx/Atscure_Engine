set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_library(all_exts _all.cxx)

file(GLOB_RECURSE EXTS *.cpp)
foreach(EXT_FILE ${EXTS})
    get_filename_component(EXT_NAME ${EXT_FILE} NAME_WLE)
    add_library(${EXT_NAME} SHARED ${EXT_FILE})
    set_target_properties(${EXT_NAME} PROPERTIES PREFIX "e_")
    target_include_directories(${EXT_NAME} PUBLIC include)
    target_include_directories(${EXT_NAME} PUBLIC include_app)
    target_link_libraries(${EXT_NAME} atscure_engine)
    target_link_libraries(all_exts ${EXT_NAME})
endforeach()