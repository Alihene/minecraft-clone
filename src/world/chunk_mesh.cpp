#include "chunk_mesh.hpp"

#include "render/gl.hpp"
#include "state.hpp"

#include <cstring>

ChunkMesh::ChunkMesh() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void*) (3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
}

void ChunkMesh::positiveXFace(u32 x, u32 y, u32 z, Block *block) {
    f32 positiveXFace[] {
        x + 1 , y, z + 1, block->texCoords.x, block->texCoords.y,
        x + 1, y, z, block->texCoords.z, block->texCoords.y,
        x + 1, y + 1, z, block->texCoords.z, block->texCoords.w,
        x + 1, y + 1, z, block->texCoords.z, block->texCoords.w,
        x + 1, y + 1, z + 1, block->texCoords.x, block->texCoords.w,
        x + 1, y, z + 1, block->texCoords.x, block->texCoords.y,
    };

    addFace(positiveXFace);

    index += ADVANCE;
}

void ChunkMesh::negativeXFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeXFace[] {
        x, y, z + 1, block->texCoords.x, block->texCoords.y,
        x, y, z, block->texCoords.z, block->texCoords.y,
        x, y + 1, z, block->texCoords.z, block->texCoords.w,
        x, y + 1, z, block->texCoords.z, block->texCoords.w,
        x, y + 1, z + 1, block->texCoords.x, block->texCoords.w,
        x, y, z + 1, block->texCoords.x, block->texCoords.y,
    };

    addFace(negativeXFace);

    index += ADVANCE;
}

void ChunkMesh::positiveYFace(u32 x, u32 y, u32 z, Block *block) {
    f32 positiveYFace[] {
        x + 1, y + 1, z + 1, block->texCoords.x, block->texCoords.y,
        x, y + 1, z + 1, block->texCoords.z, block->texCoords.y,
        x, y + 1, z, block->texCoords.z, block->texCoords.w,
        x, y + 1, z, block->texCoords.z, block->texCoords.w,
        x + 1, y + 1, z, block->texCoords.x, block->texCoords.w,
        x + 1, y + 1, z + 1, block->texCoords.x, block->texCoords.y,
    };

    addFace(positiveYFace);

    index += ADVANCE;
}

void ChunkMesh::negativeYFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeYFace[] {
        x + 1, y, z + 1, block->texCoords.x, block->texCoords.y,
        x, y, z + 1, block->texCoords.z, block->texCoords.y,
        x, y, z, block->texCoords.z, block->texCoords.w,
        x, y, z, block->texCoords.z, block->texCoords.w,
        x + 1, y, z, block->texCoords.x, block->texCoords.w,
        x + 1, y, z + 1, block->texCoords.x, block->texCoords.y,
    };

    addFace(negativeYFace);

    index += ADVANCE;
}

void ChunkMesh::positiveZFace(u32 x, u32 y, u32 z, Block *block) {
    f32 positiveZFace[] {
        x, y, z + 1, block->texCoords.x, block->texCoords.y,
        x + 1, y, z + 1, block->texCoords.z, block->texCoords.y,
        x + 1, y + 1, z + 1, block->texCoords.z, block->texCoords.w,
        x + 1, y + 1, z + 1, block->texCoords.z, block->texCoords.w,
        x, y + 1, z + 1, block->texCoords.x, block->texCoords.w,
        x, y, z + 1, block->texCoords.x, block->texCoords.y,
    };

    addFace(positiveZFace);

    index += ADVANCE;
}

void ChunkMesh::negativeZFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeZFace[] {
        x, y, z, block->texCoords.x, block->texCoords.y,
        x + 1, y, z, block->texCoords.z, block->texCoords.y,
        x + 1, y + 1, z, block->texCoords.z, block->texCoords.w,
        x + 1, y + 1, z, block->texCoords.z, block->texCoords.w,
        x, y + 1, z, block->texCoords.x, block->texCoords.w,
        x, y, z, block->texCoords.x, block->texCoords.y,
    };

    addFace(negativeZFace);

    index += ADVANCE;
}

void ChunkMesh::addFace(f32 *vertices) {
    for(i32 i = 0; i < 6 * 5; i++) {
        data.push_back(vertices[i]);
    }
}

void ChunkMesh::mesh() {
    reset();

    Chunk *left = state.world->getChunk(glm::ivec2(chunk->pos.x - 1, chunk->pos.y));
    Chunk *right = state.world->getChunk(glm::ivec2(chunk->pos.x + 1, chunk->pos.y));
    Chunk *front = state.world->getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y + 1));
    Chunk *back = state.world->getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y - 1));

    for(u32 x = 0; x < Chunk::WIDTH; x++) {
        for(u32 y = 0; y < Chunk::HEIGHT; y++) {
            for(u32 z = 0; z < Chunk::DEPTH; z++) {
                Block &block = *chunk->get(x, y, z);

                if(!block.isAir()) {
                    if(x > 0 && chunk->get(x - 1, y, z)->isAir()) {
                        negativeXFace(x, y, z, &block);
                    } else if(x == 0) {
                        if(left != nullptr && left->get(Chunk::WIDTH - 1, y, z)->isAir()) {
                            negativeXFace(x, y, z, &block);
                        }
                    }

                    if(x < Chunk::WIDTH - 1 && chunk->get(x + 1, y, z)->isAir()) {
                        positiveXFace(x, y, z, &block);
                    } else if(x == Chunk::WIDTH - 1) {
                        if(right != nullptr && right->get(0, y, z)->isAir()) {
                            positiveXFace(x, y, z, &block);
                        }
                    }

                    if(y > 0 && chunk->get(x, y - 1, z)->isAir()) {
                        negativeYFace(x, y, z, &block);
                    } else if(y == 0) {
                        negativeYFace(x, y, z, &block);
                    }

                    if(y < Chunk::HEIGHT - 1 && chunk->get(x, y + 1, z)->isAir()) {
                        positiveYFace(x, y, z, &block);
                    } else if(y == Chunk::HEIGHT - 1) {
                        positiveYFace(x, y, z, &block);
                    }

                    if(z > 0 && chunk->get(x, y, z - 1)->isAir()) {
                        negativeZFace(x, y, z, &block);
                    } else if(z == 0) {
                        if(back != nullptr && back->get(x, y, Chunk::DEPTH - 1)->isAir()) {
                            negativeZFace(x, y, z, &block);
                        }
                    }

                    if(z < Chunk::DEPTH - 1 && chunk->get(x, y, z + 1)->isAir()) {
                        positiveZFace(x, y, z, &block);
                    } else if(z == Chunk::DEPTH - 1) {
                        if(front != nullptr && front->get(x, y, 0)->isAir()) {
                            positiveZFace(x, y, z, &block);
                        }
                    }
                }
            }
        }
    }

    shouldMesh = false;
    shouldBuffer = true;
    isEmpty = false;
}

void ChunkMesh::reset() {
    data.clear();
    index = 0;
    isEmpty = true;
}

void ChunkMesh::destroy() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}