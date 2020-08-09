cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Core/*.cpp)

add_library(
    Core
    SHARED
    ${SRC_FILES}
)
target_include_directories(
    Core
    PRIVATE
)