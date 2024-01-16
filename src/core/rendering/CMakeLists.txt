file( GLOB RENDERING_SOURCES *.cpp )

find_package(glm REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

add_library(rendering ${RENDERING_SOURCES})
target_link_libraries(rendering manager)
target_include_directories(rendering PUBLIC ${MANAGER_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE assimp::assimp)