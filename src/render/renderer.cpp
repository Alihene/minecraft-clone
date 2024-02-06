#include "renderer.hpp"

#include "state.hpp"

#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer() {
    glClearColor(0.1f, 0.85f, 1.0f, 1.0f);

    program = ShaderProgram("shaders/chunk.vert", "shaders/chunk.frag");

    textureAtlas = Texture(state.resourceDir + "textures/atlas.png", Texture::FORMAT_RGBA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::prepareFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::renderWorld() {
    World *world = state.world;

    if(world == nullptr) {
        return;
    }

    program.bind();
    textureAtlas.bind();

    program.setMat4("uView", camera.view);
    program.setMat4("uProj", camera.proj);

    program.setInt("uTexture", 0);

    for(Chunk *chunk : world->chunks) {
        if(!chunk->mesh->isEmpty) {
            renderChunkMesh(chunk->mesh);
        }
    }
}

void Renderer::renderChunkMesh(ChunkMesh *mesh) {
    if(mesh->shouldBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->data.size() * sizeof(f32), &mesh->data[0], GL_DYNAMIC_DRAW);
        mesh->shouldBuffer = false;
    }

    program.setMat4("uModel", glm::translate(glm::mat4(1.0f), glm::vec3(
        mesh->chunk->blockPos.x,
        0.0f,
        mesh->chunk->blockPos.y)));

    glBindVertexArray(mesh->vao);

    glDrawArrays(GL_TRIANGLES, 0, mesh->index / 5);
}