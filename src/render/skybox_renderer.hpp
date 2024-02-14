#pragma once

#include "texture.hpp"
#include "camera.hpp"
#include "shader_program.hpp"

struct SkyboxRenderer {
    ShaderProgram program;

    Texture cubemap;

    Camera *camera;

    u32 vao;
    u32 vbo;

    SkyboxRenderer() = default;
    SkyboxRenderer(Camera *camera);

    void renderSkybox();
};