cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Utils/*.cpp)

add_library(
    Utils
    STATIC
    ${SRC_FILES}
)
target_include_directories(
    Utils
    PRIVATE
)