cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Window/*.cpp)

add_library(
    Window
    SHARED
    ${SRC_FILES}
)
target_include_directories(
    Window
    PRIVATE
)