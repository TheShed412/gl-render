cmake_minimum_required(VERSION 3.1.0)
project(glRender VERSION 0.1.0)
set(OpenGL_GL_PREFERENCE "GLVND")


find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})

link_libraries(${GLEW_LIBRARIES})
link_libraries(${OPENGL_LIBRARIES})
link_libraries(${SDL2_LIBRARIES})

add_subdirectory(lib)
add_subdirectory(src/core)
add_subdirectory(src/init)
add_subdirectory(src/scenes)

add_executable(main src/main.cpp)
target_link_libraries(main init scenes core stb_image ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} OpenGL::OpenGL)

add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/shaders/
                ${CMAKE_CURRENT_BINARY_DIR}/src/shaders/)

add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/textures/
                ${CMAKE_CURRENT_BINARY_DIR}/src/textures/)

add_custom_command(
        TARGET main POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/src/models/
                ${CMAKE_CURRENT_BINARY_DIR}/src/models/)
