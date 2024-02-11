cmake_minimum_required(VERSION 3.16)

project("minecraft-clone")

set(CMAKE_BUILD_TYPE Debug)

add_subdirectory("thirdparty/glfw")
add_subdirectory("thirdparty/glad")
add_subdirectory("thirdparty/glm")
add_subdirectory("thirdparty/stb_image")

set(SOURCES
    src/main.cpp
    src/render/window.cpp
    src/render/renderer.cpp
    src/render/shader_program.cpp
    src/render/camera.cpp
    src/render/texture.cpp
    src/block/block_manager.cpp
    src/block/block.cpp
    src/world/chunk.cpp
    src/world/chunk_mesh.cpp
    src/world/world.cpp
    src/world/generation/flat_terrain_generator.cpp
    src/player.cpp)

add_executable("minecraft-clone" ${SOURCES})

target_include_directories("minecraft-clone" PRIVATE "src")
target_link_libraries("minecraft-clone" PRIVATE glfw glad glm stb_image)