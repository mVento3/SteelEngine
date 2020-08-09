cmake_minimum_required(VERSION 3.18)

project(SteelEngine)

option(VS_CODE "VS Code project" OFF)
set(SE_BuildSystemVersion "1.1.4" CACHE STRING "SE Build System version")

set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

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
    add_subdirectory(include)
endif()

add_subdirectory(external)

set_property(TARGET GLEW PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set_property(TARGET GLEW PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set_property(TARGET GLEW PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)