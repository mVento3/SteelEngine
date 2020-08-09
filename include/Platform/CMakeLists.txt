cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Platform/*.cpp)

add_library(
    Platform
    STATIC
    ${SRC_FILES}
)
target_include_directories(
    Platform
    PRIVATE
)