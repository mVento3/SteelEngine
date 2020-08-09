cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Input/*.cpp)

add_library(
    Input
    SHARED
    ${SRC_FILES}
)
target_include_directories(
    Input
    PRIVATE
)