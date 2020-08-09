cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/ReflectionGenerator/*.cpp)

add_executable(
    ReflectionGenerator
    ${SRC_FILES}
)
target_include_directories(
    ReflectionGenerator
    PRIVATE
)