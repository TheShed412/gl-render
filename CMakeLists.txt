cmake_minimum_required(VERSION 3.1.0)
project(Tetris VERSION 0.1.0)
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

add_subdirectory(src/init)
add_subdirectory(src/scenes)

add_executable(main src/main.cpp)
target_link_libraries(main init scenes ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} OpenGL::OpenGL)
