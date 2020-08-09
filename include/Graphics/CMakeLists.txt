cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Graphics/*.cpp)

add_library(
    Graphics
    SHARED
    ${SRC_FILES}
)
target_include_directories(
    Graphics
    PRIVATE
)