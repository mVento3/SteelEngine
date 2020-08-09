cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Math/*.cpp)

add_library(
    Math
    STATIC
    ${SRC_FILES}
)
target_include_directories(
    Math
    PRIVATE
)