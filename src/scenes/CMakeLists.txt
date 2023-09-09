file( GLOB SCENE_SOURCES *.cpp )
add_library( scenes ${SCENE_SOURCES} )
target_link_libraries(scenes init core)
target_include_directories(scenes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})