cmake_minimum_required(VERSION 3.17)

add_subdirectory(assimp)
add_subdirectory(entt)
add_subdirectory(glm)
add_subdirectory(json)
add_subdirectory(MemoryModule)
add_subdirectory(GLEW)

add_library(
    imgui
    STATIC
    imgui/imgui.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_draw.cpp
    imgui/examples/imgui_impl_opengl3.cpp
    imgui/examples/imgui_impl_vulkan.cpp
    imgui/examples/imgui_impl_sdl.cpp
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_include_directories(
    imgui
    PRIVATE
    "imgui"
    "SDL2-2.0.9/include"
    "GLEW/include"
    "Vulkan/Include"
)