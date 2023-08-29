file( GLOB INIT_SOURCES *.cpp )
add_library( init ${INIT_SOURCES} )
target_include_directories(init PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})