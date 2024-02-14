#pragma once

#include "gl.hpp"
#include "shader_program.hpp"
#include "world/chunk_mesh.hpp"
#include "camera.hpp"
#include "skybox_renderer.hpp"

struct Renderer {
    ShaderProgram program;

    Texture textureAtlas;
    Texture cubemap;

    Camera camera;

    SkyboxRenderer skyboxRenderer;

    bool wireframeRender = false;

    Renderer();

    void prepareFrame();

    void renderWorld();
    
    void renderSkyBox();
    
    void renderChunkMesh(ChunkMesh *mesh);
};