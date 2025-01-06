list(APPEND SHADERS base_vert.vert base_frag.frag)

foreach(SHADER IN LISTS SHADERS)
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    message("Output of list is: " ${SHADER})

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv
        COMMAND glslang ${CMAKE_SOURCE_DIR}/shaders/${SHADER} -o ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv
        DEPENDS ${CMAKE_SOURCE_DIR}/shaders/${SHADER}
        COMMENT "Compiling ${SHADER}"
    ) 

    list(APPEND SPV_SHADERS ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv)
endforeach()




