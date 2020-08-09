cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/FileWatcher/*.cpp)

add_library(
    FileWatcher
    STATIC
    ${SRC_FILES}
)
target_include_directories(
    FileWatcher
    PRIVATE
)