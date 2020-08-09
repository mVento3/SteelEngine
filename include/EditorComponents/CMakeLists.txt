cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/EditorComponents/*.cpp)

add_library(
    EditorComponents
    STATIC
    ${SRC_FILES}
)
target_include_directories(
    EditorComponents
    PRIVATE
)