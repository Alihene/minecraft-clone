#pragma once

#include "gl.hpp"
#include "shader_program.hpp"
#include "world/chunk_mesh.hpp"
#include "camera.hpp"
#include "skybox_renderer.hpp"

struct Renderer {
    ShaderProgram program;
    ShaderProgram guiProgram;

    Texture textureAtlas;
    Texture cubemap;
    Texture crosshairTexture;

    Camera camera;

    glm::mat4 orthoProj;
    u32 crosshairVao;
    u32 crosshairVbo;

    SkyboxRenderer skyboxRenderer;

    bool wireframeRender = false;

    Renderer();

    void prepareFrame();

    void renderWorld();
    
    void renderSkyBox();

    void renderCrosshair();
    
    void renderChunkMesh(ChunkMesh *mesh);
};