list(APPEND SHADERS base_vert.vert base_frag.frag)

foreach(SHADER IN LISTS SHADERS)
    get_filename_component(SHADER_NAME ${SHADER} NAME)
    message("Output of list is: " ${SHADER})

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv
        COMMAND glslang ${CMAKE_SOURCE_DIR}/shaders/${SHADER} -o ${CMAKE_BINARY_DIR}/${SHADER}.spv
        DEPENDS ${SHADER}
        COMMENT "Compiling ${SHADER}"
    ) 
    
    target_sources(${TARGET_NAME} PRIVATE ${SHADER})
    list(APPEND SPV_SHADERS ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv)
endforeach()

#list(APPEND SHADER_SOURCE_FILES base_vert.vert base_frag.frag)
#list(APPEND SHADER_COMMAND COMMAND)
#list(APPEND SHADER_COMMAND Vulkan::glslc)
#list(APPEND SHADER_COMMAND "${SHADER_SOURCE}")
#list(APPEND SHADER_COMMAND "-o")
#list(APPEND SHADER_COMMAND "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.spv")

#list(APPEND SHADER_PRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${SHADER_NAME}.spv")

#add_custom_targets(${TARGET_NAME} ALL
#    ${SHADER_COMMANDS}
#    COMMENT "Compiling Shaders [${TARGET_NAME}]"
#    SOURCES ${SHADER_SOURCE_FILES}
#    BYPRODUCTS ${SHADER_PRODUCTS}
#  )




