add_library(stb_image STATIC stb_image.h)
set_target_properties(stb_image PROPERTIES LINKER_LANGUAGE C)
add_custom_command(
        TARGET stb_image POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/lib/stb_image.h
                ${CMAKE_CURRENT_BINARY_DIR}/stb_image.h)