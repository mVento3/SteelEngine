cmake_minimum_required(VERSION 3.18)

project(SteelEngine)

option(VS_CODE "VS Code project")
set(SE_BuildSystemVersion "1.0.0" CACHE STRING "SE Build System version")

if(VS_CODE)
    set(build_system SteelEngineBuildSystem-${SE_BuildSystemVersion})
    set(SE_BuildSystemVersionFinal https://github.com/mVento3/SteelEngineBuildSystem/archive/v${SE_BuildSystemVersion}.zip)

    file(
        DOWNLOAD
        ${SE_BuildSystemVersionFinal}
        ${CMAKE_BINARY_DIR}/SteelEngineBuildSystem.zip
    )
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/SteelEngineBuildSystem.zip)
    file(REMOVE ${CMAKE_BINARY_DIR}/SteelEngineBuildSystem.zip)
    file(COPY ${CMAKE_BINARY_DIR}/${build_system}/build_linux.py DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_BINARY_DIR}/${build_system}/build_windows.py DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_BINARY_DIR}/${build_system}/SubprocessWrapper.py DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_BINARY_DIR}/${build_system}/Module.py DESTINATION ${CMAKE_BINARY_DIR})
    file(COPY ${CMAKE_BINARY_DIR}/${build_system}/DependenciesBuilder.py DESTINATION ${CMAKE_BINARY_DIR})
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/${build_system})
else()
    message(FATAL_ERROR "Currently not supported! Need to find a way to iterate over .lib modules and link them")

    file(GLOB Application_SRCS src/Application/*.cpp)
    add_library(
        Application
        STATIC
        ${Application_SRCS}
    )
    target_include_directories(Application PRIVATE include)

    file(GLOB StaticHelper_SRCS src/StaticHelper/*.cpp)
    add_library(
        StaticHelper
        STATIC
        ${StaticHelper_SRCS}
    )
    target_include_directories(StaticHelper PRIVATE include)

    file(GLOB FileWatcher_SRCS src/FileWatcher/*.cpp)
    add_library(
        FileWatcher
        STATIC
        ${FileWatcher_SRCS}
    )
    target_include_directories(FileWatcher PRIVATE include)

    file(GLOB Memory_SRCS src/Memory/*.cpp)
    add_library(
        Memory
        STATIC
        ${Memory_SRCS}
    )
    target_include_directories(Memory PRIVATE include)

    file(GLOB InformationTracker_SRCS src/InformationTracker/*.cpp)
    add_library(
        InformationTracker
        STATIC
        ${InformationTracker_SRCS}
    )
    target_include_directories(InformationTracker PRIVATE include)

    file(GLOB Module_SRCS src/Module/*.cpp)
    add_library(
        Module
        STATIC
        ${Module_SRCS}
    )
    target_include_directories(Module PRIVATE include)

    file(GLOB Math_SRCS src/Math/*.cpp)
    add_library(
        Math
        STATIC
        ${Math_SRCS}
    )
    target_include_directories(Math PRIVATE include)

    file(GLOB Platform_SRCS src/Platform/*.cpp)
    add_library(
        Platform
        STATIC
        ${Platform_SRCS}
    )
    target_include_directories(Platform PRIVATE include)

    file(GLOB Utils_SRCS src/Utils/*.cpp)
    add_library(
        Utils
        STATIC
        ${Utils_SRCS}
    )
    target_include_directories(Utils PRIVATE include)

    file(GLOB Event_SRCS src/Event/*.cpp)
    add_library(
        Event
        STATIC
        ${Event_SRCS}
    )
    target_include_directories(Event PRIVATE include)

    file(GLOB Profiler_SRCS src/Profiler/*.cpp)
    add_library(
        Profiler
        STATIC
        ${Profiler_SRCS}
    )
    target_include_directories(Profiler PRIVATE include)

    file(GLOB Serialization_SRCS src/Serialization/*.cpp)
    add_library(
        Serialization
        STATIC
        ${Serialization_SRCS}
    )
    target_include_directories(Serialization PRIVATE include)

    file(GLOB EditorComponents_SRCS src/EditorComponents/*.cpp)
    add_library(
        EditorComponents
        STATIC
        ${EditorComponents_SRCS}
    )
    target_include_directories(EditorComponents PRIVATE include)

    file(GLOB RuntimeReflection_SRCS src/RuntimeReflection/*.cpp)
    add_library(
        RuntimeReflection
        SHARED
        ${RuntimeReflection_SRCS}
    )
    target_include_directories(RuntimeReflection PRIVATE include)
endif()

add_subdirectory(external)