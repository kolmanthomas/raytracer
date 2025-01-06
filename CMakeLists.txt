cmake_minimum_required(VERSION 3.26)

#include(${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

project(Raytracer VERSION 0.0.1 LANGUAGES CXX)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
    

set(TARGET_NAME "raytracer")
set(SHADER_TARGET_NAME "raytracer_shaders")
#set(TARGET_NAME_LIB "CLox_Lib")
set(TEST_NAME "raytracer_test")
set(TEST_NAME "raytracer_test")
set(COMPILE_FEATURES cxx_std_20)

enable_testing()
find_package(Vulkan REQUIRED)
find_package(GTest CONFIG REQUIRED) 
find_package(spdlog CONFIG REQUIRED)
find_package(eigen3 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glslang CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

message(${Vulkan_LIBRARY})

add_executable(${TARGET_NAME}) 
add_executable(${TEST_NAME}) 
#add_library(${TARGET_NAME_LIB} STATIC)
 
#target_link_libraries(${TARGET_NAME} ${TARGET_NAME_LIB})
target_compile_features(${TARGET_NAME} PUBLIC ${COMPILE_FEATURES})
target_compile_features(${TEST_NAME} PUBLIC ${COMPILE_FEATURES})

set_target_properties(${TARGET_NAME} PROPERTIES 
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(
    ${TEST_NAME} PRIVATE 
    GTest::gtest 
    GTest::gtest_main 
    GTest::gmock 
    GTest::gmock_main
)
#
target_link_libraries(
    ${TARGET_NAME} PRIVATE
    spdlog::spdlog
    glfw
    Vulkan::Vulkan
    glslang::glslang glslang::glslang-default-resource-limits glslang::SPIRV glslang::SPVRemapper
    glm::glm
)

target_include_directories(
    ${TARGET_NAME}
    PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_include_directories(
    ${TEST_NAME} 
    PRIVATE
    ${CMAKE_SOURCE_DIR}
)


include(GoogleTest)

#target_include_directories(${TEST_NAME} PRIVATE src)
add_subdirectory(src)
add_subdirectory(test)

############
# Unit Tests
############
gtest_discover_tests(${TEST_NAME})
