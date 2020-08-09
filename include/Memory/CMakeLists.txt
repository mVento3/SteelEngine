cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Memory/*.cpp)

add_library(
    Memory
    STATIC
    ${SRC_FILES}
)
target_include_directories(
    Memory
    PRIVATE
)