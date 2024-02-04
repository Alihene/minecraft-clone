#pragma once

#include "gl.hpp"
#include "shader_program.hpp"
#include "world/chunk_mesh.hpp"
#include "camera.hpp"
#include "texture.hpp"

struct Renderer {
    ShaderProgram program;

    Texture textureAtlas;

    Camera camera;

    bool wireframeRender = false;

    Renderer();

    void prepareFrame();

    void renderWorld();
    
    void renderChunkMesh(ChunkMesh *mesh);
};