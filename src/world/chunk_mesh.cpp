#include "chunk_mesh.hpp"

#include "render/gl.hpp"
#include "state.hpp"

#include <cstring>
#include <algorithm>

static bool faceDepthCmp(const ChunkMesh::Face &face1, const ChunkMesh::Face &face2) {
    f32 distance1 = glm::distance(face1.pos, state.renderer->camera.pos);
    f32 distance2 = glm::distance(face2.pos, state.renderer->camera.pos);
    return distance1 > distance2;
}

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

void ChunkMesh::sort() {
    if(isEmpty) {
        return;
    }

    f32 *tempData = (f32*) std::malloc(data.size() * sizeof(f32));
    std::memcpy(tempData, &data[0], data.size() * sizeof(f32));

    std::sort(faces.begin(), faces.end(), faceDepthCmp);

    for(i32 i = 0; i < faces.size(); i++) {
        std::memcpy(&data[i * 6 * 5], &tempData[faces[i].indexBase], 6 * 5 * sizeof(f32));
        faces[i].indexBase = i * 6 * 5;
    }

    std::free(tempData);

    shouldBuffer = true;
}

void ChunkMesh::positiveXFace(u32 x, u32 y, u32 z, Block *block) {
    f32 positiveXFace[] {
        x + 1 , y, z + 1, block->texCoords.posX.x, block->texCoords.posX.y,
        x + 1, y, z, block->texCoords.posX.z, block->texCoords.posX.y,
        x + 1, y + 1, z, block->texCoords.posX.z, block->texCoords.posX.w,
        x + 1, y + 1, z, block->texCoords.posX.z, block->texCoords.posX.w,
        x + 1, y + 1, z + 1, block->texCoords.posX.x, block->texCoords.posX.w,
        x + 1, y, z + 1, block->texCoords.posX.x, block->texCoords.posX.y,
    };

    addFace(positiveXFace);
    faces.push_back((Face) {
        .pos = glm::vec3(0.9f + x + chunk->pos.x * 16, 0.5f + y, 0.5f + z + chunk->pos.y * 16),
        .indexBase = index
    });

    index += ADVANCE;
}

void ChunkMesh::negativeXFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeXFace[] {
        x, y, z + 1, block->texCoords.negX.x, block->texCoords.negX.y,
        x, y, z, block->texCoords.negX.z, block->texCoords.negX.y,
        x, y + 1, z, block->texCoords.negX.z, block->texCoords.negX.w,
        x, y + 1, z, block->texCoords.negX.z, block->texCoords.negX.w,
        x, y + 1, z + 1, block->texCoords.negX.x, block->texCoords.negX.w,
        x, y, z + 1, block->texCoords.negX.x, block->texCoords.negX.y,
    };

    addFace(negativeXFace);
    faces.push_back((Face) {
        .pos = glm::vec3(0.1f + x + chunk->pos.x * 16, 0.5f + y, 0.5f + z + chunk->pos.y * 16),
        .indexBase = index
    });

    index += ADVANCE;
}

void ChunkMesh::positiveYFace(u32 x, u32 y, u32 z, Block *block) {
    f32 positiveYFace[] {
        x + 1, y + 1, z + 1, block->texCoords.posY.x, block->texCoords.posY.y,
        x, y + 1, z + 1, block->texCoords.posY.z, block->texCoords.posY.y,
        x, y + 1, z, block->texCoords.posY.z, block->texCoords.posY.w,
        x, y + 1, z, block->texCoords.posY.z, block->texCoords.posY.w,
        x + 1, y + 1, z, block->texCoords.posY.x, block->texCoords.posY.w,
        x + 1, y + 1, z + 1, block->texCoords.posY.x, block->texCoords.posY.y,
    };

    addFace(positiveYFace);
    faces.push_back((Face) {
        .pos = glm::vec3(0.5f + x + chunk->pos.x * 16, 0.9f + y, 0.5f + z + chunk->pos.y * 16),
        .indexBase = index
    });

    index += ADVANCE;
}

void ChunkMesh::negativeYFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeYFace[] {
        x + 1, y, z + 1, block->texCoords.negY.x, block->texCoords.negY.y,
        x, y, z + 1, block->texCoords.negY.z, block->texCoords.negY.y,
        x, y, z, block->texCoords.negY.z, block->texCoords.negY.w,
        x, y, z, block->texCoords.negY.z, block->texCoords.negY.w,
        x + 1, y, z, block->texCoords.negY.x, block->texCoords.negY.w,
        x + 1, y, z + 1, block->texCoords.negY.x, block->texCoords.negY.y,
    };

    addFace(negativeYFace);
    faces.push_back((Face) {
        .pos = glm::vec3(0.5f + x + chunk->pos.x * 16, 0.1f + y, 0.5f + z + chunk->pos.y * 16),
        .indexBase = index
    });

    index += ADVANCE;
}

void ChunkMesh::positiveZFace(u32 x, u32 y, u32 z, Block *block) {
    f32 positiveZFace[] {
        x, y, z + 1, block->texCoords.posZ.x, block->texCoords.posZ.y,
        x + 1, y, z + 1, block->texCoords.posZ.z, block->texCoords.posZ.y,
        x + 1, y + 1, z + 1, block->texCoords.posZ.z, block->texCoords.posZ.w,
        x + 1, y + 1, z + 1, block->texCoords.posZ.z, block->texCoords.posZ.w,
        x, y + 1, z + 1, block->texCoords.posZ.x, block->texCoords.posZ.w,
        x, y, z + 1, block->texCoords.posZ.x, block->texCoords.posZ.y,
    };

    addFace(positiveZFace);
    faces.push_back((Face) {
        .pos = glm::vec3(0.5f + x + chunk->pos.x * 16, 0.5f + y, 0.9f + z + chunk->pos.y * 16),
        .indexBase = index
    });

    index += ADVANCE;
}

void ChunkMesh::negativeZFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeZFace[] {
        x, y, z, block->texCoords.negZ.x, block->texCoords.negZ.y,
        x + 1, y, z, block->texCoords.negZ.z, block->texCoords.negZ.y,
        x + 1, y + 1, z, block->texCoords.negZ.z, block->texCoords.negZ.w,
        x + 1, y + 1, z, block->texCoords.negZ.z, block->texCoords.negZ.w,
        x, y + 1, z, block->texCoords.negZ.x, block->texCoords.negZ.w,
        x, y, z, block->texCoords.negZ.x, block->texCoords.negZ.y,
    };

    addFace(negativeZFace);
    faces.push_back((Face) {
        .pos = glm::vec3(0.5f + x + chunk->pos.x * 16, 0.5f + y, 0.1f + z + chunk->pos.y * 16),
        .indexBase = index
    });

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
                    if(x > 0 && (chunk->get(x - 1, y, z)->isAir() || chunk->get(x - 1, y, z)->transparent)) {
                        negativeXFace(x, y, z, &block);
                    } else if(x == 0) {
                        if(left != nullptr && (left->get(Chunk::WIDTH - 1, y, z)->isAir() || left->get(Chunk::WIDTH - 1, y, z)->transparent)) {
                            negativeXFace(x, y, z, &block);
                        }
                    }

                    if(x < Chunk::WIDTH - 1 && (chunk->get(x + 1, y, z)->isAir() || chunk->get(x + 1, y, z)->transparent)) {
                        positiveXFace(x, y, z, &block);
                    } else if(x == Chunk::WIDTH - 1) {
                        if(right != nullptr && (right->get(0, y, z)->isAir() || right->get(0, y, z)->transparent)) {
                            positiveXFace(x, y, z, &block);
                        }
                    }

                    if(y > 0 && (chunk->get(x, y - 1, z)->isAir() || chunk->get(x, y - 1, z)->transparent)) {
                        negativeYFace(x, y, z, &block);
                    } else if(y == 0) {
                        negativeYFace(x, y, z, &block);
                    }

                    if(y < Chunk::HEIGHT - 1 && (chunk->get(x, y + 1, z)->isAir() || chunk->get(x, y + 1, z)->transparent)) {
                        positiveYFace(x, y, z, &block);
                    } else if(y == Chunk::HEIGHT - 1) {
                        positiveYFace(x, y, z, &block);
                    }

                    if(z > 0 && (chunk->get(x, y, z - 1)->isAir() || chunk->get(x, y, z - 1)->transparent)) {
                        negativeZFace(x, y, z, &block);
                    } else if(z == 0) {
                        if(back != nullptr && (back->get(x, y, Chunk::DEPTH - 1)->isAir() || back->get(x, y, Chunk::DEPTH - 1)->transparent)) {
                            negativeZFace(x, y, z, &block);
                        }
                    }

                    if(z < Chunk::DEPTH - 1 && (chunk->get(x, y, z + 1)->isAir() || chunk->get(x, y, z + 1)->transparent)) {
                        positiveZFace(x, y, z, &block);
                    } else if(z == Chunk::DEPTH - 1) {
                        if(front != nullptr && (front->get(x, y, 0)->isAir() || front->get(x, y, 0)->transparent)) {
                            positiveZFace(x, y, z, &block);
                        }
                    }
                }
            }
        }
    }

    isEmpty = false;

    sort();

    shouldMesh = false;
    shouldBuffer = true;
}

void ChunkMesh::reset() {
    data.clear();
    faces.clear();
    index = 0;
    isEmpty = true;
}

void ChunkMesh::destroy() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}