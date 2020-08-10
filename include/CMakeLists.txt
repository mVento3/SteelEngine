cmake_minimum_required(VERSION 3.18)

FILE(GLOB children */CMakeLists.txt)

list(APPEND STATIC_LIBRARIES "")
list(APPEND LIBS
    "OpenGL32.lib"
    "${CMAKE_SOURCE_DIR}/external/Vulkan/Lib/vulkan-1.lib"
    "${CMAKE_SOURCE_DIR}/build/external/GLEW/Debug/GLEWd.lib"
    "${CMAKE_SOURCE_DIR}/build/external/Debug/imguid.lib"
    "${CMAKE_SOURCE_DIR}/external/SDL2-2.0.9/lib/x64/SDL2.lib"
    "Ws2_32.lib"
    "${CMAKE_SOURCE_DIR}/build/external/assimp/lib/Debug/assimp-vc142-mtd.lib"
    "${CMAKE_SOURCE_DIR}/external/Python/libs/python37.lib"
)
list(APPEND INCLUDES
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/__generated_reflection__"
    "${CMAKE_SOURCE_DIR}/external/SDL2-2.0.9/include"
    "${CMAKE_SOURCE_DIR}/external/Vulkan/Include"
    "${CMAKE_SOURCE_DIR}/external/glm"
    "${CMAKE_SOURCE_DIR}/external"
    "${CMAKE_SOURCE_DIR}/external/json/include"
    "${CMAKE_SOURCE_DIR}/external/Python/include"
    "${CMAKE_SOURCE_DIR}/external/GLEW/include"
    "${CMAKE_SOURCE_DIR}/external/assimp/include"
    "${CMAKE_SOURCE_DIR}/external/assimp/build/include"
    "${CMAKE_SOURCE_DIR}/external/entt/src"
    "${CMAKE_SOURCE_DIR}/external/imgui"
)

add_definitions(-DSE_WINDOWS)

FOREACH(child ${children})
    get_filename_component(PARENT_DIR ${child} DIRECTORY)
    add_subdirectory(${PARENT_DIR})
    get_filename_component(PROJECT_ID ${PARENT_DIR} NAME)
    get_target_property(
        TARGET_TYPE ${PROJECT_ID} TYPE
    )
    target_include_directories(
        ${PROJECT_ID} PRIVATE
        ${INCLUDES}
    )
    set_target_properties(${PROJECT_ID} PROPERTIES
        CXX_STANDARD 17
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
        LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
        DEBUG_POSTFIX ""
    )
    if(EXISTS ${CMAKE_SOURCE_DIR}/__generated_reflection__/${PROJECT_ID})
        FILE(GLOB_RECURSE GENERATED_SRCS ${CMAKE_SOURCE_DIR}/__generated_reflection__/${PROJECT_ID}/*.cpp)
        target_sources(
            ${PROJECT_ID} PRIVATE
            ${GENERATED_SRCS}
        )
    endif()
    if(${TARGET_TYPE} STREQUAL "STATIC_LIBRARY")
        list(APPEND STATIC_LIBRARIES ${PROJECT_ID})
    endif()
ENDFOREACH()

list(APPEND STATIC_LIBRARY_REFLECTION_SRCS "")

FOREACH(src ${STATIC_LIBRARIES})
    FILE(GLOB_RECURSE STATIC_GENERATED_SRCS ${CMAKE_SOURCE_DIR}/__generated_reflection__/${src}/*.cpp)
    list(APPEND STATIC_LIBRARY_REFLECTION_SRCS ${STATIC_GENERATED_SRCS})
ENDFOREACH()

message(STATUS "aaa " ${STATIC_LIBRARY_REFLECTION_SRCS})

add_library(
    StaticLibraryReflection
    SHARED
    ${STATIC_LIBRARY_REFLECTION_SRCS}
)
target_include_directories(
    StaticLibraryReflection PRIVATE
    ${INCLUDES}
)
set_target_properties(StaticLibraryReflection PROPERTIES
    CXX_STANDARD 17
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
    DEBUG_POSTFIX ""
)
target_link_libraries(
    StaticLibraryReflection
    ${STATIC_LIBRARIES}
    ${LIBS}
)

FOREACH(child ${children})
    get_filename_component(PARENT_DIR ${child} DIRECTORY)
    get_filename_component(PROJECT_ID ${PARENT_DIR} NAME)
    get_target_property(
        TARGET_TYPE ${PROJECT_ID} TYPE
    )
    if(${TARGET_TYPE} STREQUAL "EXECUTABLE" OR ${TARGET_TYPE} STREQUAL "SHARED_LIBRARY")
        target_link_libraries(
            ${PROJECT_ID}
            ${STATIC_LIBRARIES}
            ${LIBS}
        )
    endif()
ENDFOREACH()