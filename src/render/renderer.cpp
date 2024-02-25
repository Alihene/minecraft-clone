#include "renderer.hpp"

#include "state.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <stb_image/stb_image.h>

Renderer::Renderer() {
    glClearColor(0.1f, 0.85f, 1.0f, 1.0f);

    program = ShaderProgram("shaders/chunk.vert", "shaders/chunk.frag");
    guiProgram = ShaderProgram("shaders/gui.vert", "shaders/gui.frag");

    textureAtlas = Texture(state.resourceDir + "textures/atlas.png", Texture::FORMAT_RGBA);

    crosshairTexture = Texture(state.resourceDir + "textures/crosshair.png", Texture::FORMAT_RGBA);

    glGenVertexArrays(1, &crosshairVao);
    glBindVertexArray(crosshairVao);

    glGenBuffers(1, &crosshairVbo);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*) (2 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    skyboxRenderer = SkyboxRenderer(&camera);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::prepareFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderWorld() {
    orthoProj = glm::ortho(0.0f, (f32) state.window->dimensions.x, (f32) state.window->dimensions.y, 0.0f, -1.0f, 1.0f);

    renderSkyBox();

    World *world = state.world;

    if(world == nullptr) {
        return;
    }

    program.bind();
    textureAtlas.bind();

    program.setMat4("uView", camera.view);
    program.setMat4("uProj", camera.proj);

    program.setInt("uTexture", 0);

    world->chunkMutex.lock();

    for(Chunk *chunk : world->chunks) {
        if(!chunk->mesh->isEmpty && !chunk->mesh->generating) {
            renderChunkMesh(chunk->mesh);
        }
    }

    world->chunkMutex.unlock();

    renderCrosshair();
}

void Renderer::renderSkyBox() {
    skyboxRenderer.renderSkybox();
}

void Renderer::renderCrosshair() {
    guiProgram.bind();
    guiProgram.setMat4("uProj", orthoProj);
    guiProgram.setInt("uTexture", 0);

    u32 width = state.window->dimensions.x;
    u32 height = state.window->dimensions.y;

    f32 crosshairVertices[] = {
        (f32) width / 2.0f - 10.0f, (f32) height / 2.0f - 10.0f, 0.0f, 0.0f,
        (f32) width / 2.0f + 10.0f, (f32) height / 2.0f - 10.0f, 1.0f, 0.0f,
        (f32) width / 2.0f + 10.0f, (f32) height / 2.0f + 10.0f, 1.0f, 1.0f,
        (f32) width / 2.0f + 10.0f, (f32) height / 2.0f + 10.0f, 1.0f, 1.0f,
        (f32) width / 2.0f - 10.0f, (f32) height / 2.0f + 10.0f, 0.0f, 1.0f,
        (f32) width / 2.0f - 10.0f, (f32) height / 2.0f - 10.0f, 0.0f, 0.0f,
    };

    glBindBuffer(GL_ARRAY_BUFFER, crosshairVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    glBindVertexArray(crosshairVao);

    crosshairTexture.bind();

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::renderChunkMesh(ChunkMesh *mesh) {
    if(mesh->shouldBufferOpaque) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->data.size() * sizeof(f32), &mesh->data[0], GL_DYNAMIC_DRAW);
        mesh->shouldBufferOpaque = false;
    }

    if(mesh->shouldBufferTransparent) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh->transparentVbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->transparentData.size() * sizeof(f32), &mesh->transparentData[0], GL_DYNAMIC_DRAW);
        mesh->shouldBufferTransparent = false;
    }

    program.setMat4("uModel", glm::translate(glm::mat4(1.0f), glm::vec3(
        mesh->chunk->blockPos.x,
        0.0f,
        mesh->chunk->blockPos.y)));

    glBindVertexArray(mesh->vao);

    glDrawArrays(GL_TRIANGLES, 0, mesh->index / 5);

    glBindVertexArray(mesh->transparentVao);

    glDrawArrays(GL_TRIANGLES, 0, mesh->transparentIndex / 5);
}