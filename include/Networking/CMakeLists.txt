cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Networking/*.cpp)

add_library(
    Networking
    SHARED
    ${SRC_FILES}
)
target_include_directories(
    Networking
    PRIVATE
)