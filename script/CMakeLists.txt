# script
# script
# script
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/script/bin) 
file(GLOB_RECURSE SCRIPTS *.cpp)
foreach(SCRIPT_FILE ${SCRIPTS})
    get_filename_component(SCRIPT_NAME ${SCRIPT_FILE} NAME_WLE)
    add_library(${SCRIPT_NAME} SHARED ${SCRIPT_FILE})
    target_include_directories(${SCRIPT_NAME} PUBLIC include)
    target_include_directories(${SCRIPT_NAME} PUBLIC include_app)
    target_link_libraries(${SCRIPT_NAME} atscure_engine)
endforeach()