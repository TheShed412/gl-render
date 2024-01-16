cmake_minimum_required(VERSION 3.1.0)

project(manager)

file(GLOB MANAGER_SOURCES *.cpp)
add_library(manager ${MANAGER_SOURCES})

target_link_libraries(manager PRIVATE stb_image)
include_directories("${CMAKE_BINARY_DIR}/lib/")

target_include_directories(manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} INTERFACE "${CMAKE_BINARY_DIR}/lib/")

set(${MANAGER_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)