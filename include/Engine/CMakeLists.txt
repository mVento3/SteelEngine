cmake_minimum_required(VERSION 3.17)

FILE(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/Engine/*.cpp)

add_executable(
    Engine
    ${SRC_FILES}
)
target_include_directories(
    Engine
    PRIVATE
)