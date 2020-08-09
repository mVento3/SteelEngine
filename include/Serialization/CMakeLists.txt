cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Serialization/*.cpp)

add_library(
    Serialization
    STATIC
    ${SRC_FILES}
)
target_include_directories(
    Serialization
    PRIVATE
)