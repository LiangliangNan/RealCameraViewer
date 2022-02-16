cmake_minimum_required(VERSION 3.12)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

# The data directory
add_compile_definitions("DATA_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/data\"")

add_subdirectory(src)
