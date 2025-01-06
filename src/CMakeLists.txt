target_sources(${TARGET_NAME}
    PRIVATE
    #color.cpp
    #camera.cpp
	main.cpp
    window.cpp
    handy.cpp
    file.cpp
)

add_subdirectory(gpu)
add_subdirectory(shaders)

