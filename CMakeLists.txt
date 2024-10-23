cmake_minimum_required(VERSION 3.25)

#include(${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

project(CLox
    VERSION 1.0)
    

set(TARGET_NAME "raytracer")
#set(TARGET_NAME_LIB "CLox_Lib")
set(TEST_NAME "raytracer_test")

enable_testing()
find_package(GTest CONFIG REQUIRED) 

add_executable(${TARGET_NAME}) 
add_executable(${TEST_NAME}) 
#add_library(${TARGET_NAME_LIB} STATIC)
 
#target_link_libraries(${TARGET_NAME} ${TARGET_NAME_LIB})
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)
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

include(GoogleTest)

#target_include_directories(${TEST_NAME} PRIVATE src)
add_subdirectory(src)
add_subdirectory(test)

############
# Unit Tests
############
gtest_discover_tests(${TEST_NAME})
