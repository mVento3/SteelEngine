cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/ConsoleSystem/*.cpp)

add_library(
    ConsoleSystem
    SHARED
    ${SRC_FILES}
)
target_include_directories(
    ConsoleSystem
    PRIVATE
)