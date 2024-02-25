#include "chunk_mesh.hpp"

#include "render/gl.hpp"
#include "state.hpp"

#include <cstring>
#include <algorithm>

#define FACE_NEG_X 0
#define FACE_POS_X 1
#define FACE_NEG_Y 2
#define FACE_POS_Y 3
#define FACE_NEG_Z 4
#define FACE_POS_Z 5

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*) (3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*) (5 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &transparentVao);
    glBindVertexArray(transparentVao);

    glGenBuffers(1, &transparentVbo);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*) (3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*) (5 * sizeof(f32)));
    glEnableVertexAttribArray(2);
}

void ChunkMesh::sort() {
    if(isEmpty) {
        return;
    }

    f32 *tempData = (f32*) std::malloc(transparentData.size() * sizeof(f32));
    std::memcpy(tempData, &transparentData[0], transparentData.size() * sizeof(f32));

    std::sort(faces.begin(), faces.end(), faceDepthCmp);

    for(i32 i = 0; i < faces.size(); i++) {
        std::memcpy(&transparentData[i * 6 * 6], &tempData[faces[i].indexBase], 6 * 6 * sizeof(f32));
        faces[i].indexBase = i * 6 * 6;
    }

    std::free(tempData);

    shouldBufferTransparent = true;
}

void ChunkMesh::positiveXFace(u32 x, u32 y, u32 z, Block *block) {
    f32 positiveXFace[] {
        x + 1 , y, z + 1, block->texCoords.posX.x, block->texCoords.posX.y, 0.8f,
        x + 1, y, z, block->texCoords.posX.z, block->texCoords.posX.y, 0.8f,
        x + 1, y + 1, z, block->texCoords.posX.z, block->texCoords.posX.w, 0.8f,
        x + 1, y + 1, z, block->texCoords.posX.z, block->texCoords.posX.w, 0.8f,
        x + 1, y + 1, z + 1, block->texCoords.posX.x, block->texCoords.posX.w, 0.8f,
        x + 1, y, z + 1, block->texCoords.posX.x, block->texCoords.posX.y, 0.8f,
    };

    addFace(positiveXFace, block->transparent);
    if(block->transparent) {
        faces.push_back((Face) {
            .pos = glm::vec3(0.9f + x + chunk->blockPos.x, 0.5f + y, 0.5f + z + chunk->pos.y * 16),
            .indexBase = transparentIndex
        });
    }

    if(block->transparent) {
        transparentIndex += ADVANCE;
    } else {
        index += ADVANCE;
    }
}

void ChunkMesh::negativeXFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeXFace[] {
        x, y, z + 1, block->texCoords.negX.x, block->texCoords.negX.y, 0.8f,
        x, y, z, block->texCoords.negX.z, block->texCoords.negX.y, 0.8f,
        x, y + 1, z, block->texCoords.negX.z, block->texCoords.negX.w, 0.8f,
        x, y + 1, z, block->texCoords.negX.z, block->texCoords.negX.w, 0.8f,
        x, y + 1, z + 1, block->texCoords.negX.x, block->texCoords.negX.w, 0.8f,
        x, y, z + 1, block->texCoords.negX.x, block->texCoords.negX.y, 0.8f,
    };

    addFace(negativeXFace, block->transparent);
    if(block->transparent) {
        faces.push_back((Face) {
            .pos = glm::vec3(0.1f + x + chunk->blockPos.x, 0.5f + y, 0.5f + z + chunk->pos.y * 16),
            .indexBase = transparentIndex
        });
    }

    if(block->transparent) {
        transparentIndex += ADVANCE;
    } else {
        index += ADVANCE;
    }
}

void ChunkMesh::positiveYFace(u32 x, u32 y, u32 z, Block *block, bool topBlock) {
    // Fluids are 1 pixel shorter than normal blocks
    f32 yOffset = 1.0f;
    if(!block->solid && topBlock) {
        yOffset = 15.0f / 16.0f;
    }

    f32 positiveYFace[] {
        x + 1, y + yOffset, z + 1, block->texCoords.posY.x, block->texCoords.posY.y, 1.0f,
        x, y + yOffset, z + 1, block->texCoords.posY.z, block->texCoords.posY.y, 1.0f,
        x, y + yOffset, z, block->texCoords.posY.z, block->texCoords.posY.w, 1.0f,
        x, y + yOffset, z, block->texCoords.posY.z, block->texCoords.posY.w, 1.0f,
        x + 1, y + yOffset, z, block->texCoords.posY.x, block->texCoords.posY.w, 1.0f,
        x + 1, y + yOffset, z + 1, block->texCoords.posY.x, block->texCoords.posY.y, 1.0f,
    };

    addFace(positiveYFace, block->transparent);
    if(block->transparent) {
        faces.push_back((Face) {
            .pos = glm::vec3(0.5f + x + chunk->blockPos.x, 0.9f + y, 0.5f + z + chunk->pos.y * 16),
            .indexBase = transparentIndex
        });
    }

    if(block->transparent) {
        transparentIndex += ADVANCE;
    } else {
        index += ADVANCE;
    }
}

void ChunkMesh::negativeYFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeYFace[] {
        x + 1, y, z + 1, block->texCoords.negY.x, block->texCoords.negY.y, 0.6f,
        x, y, z + 1, block->texCoords.negY.z, block->texCoords.negY.y, 0.6f,
        x, y, z, block->texCoords.negY.z, block->texCoords.negY.w, 0.6f,
        x, y, z, block->texCoords.negY.z, block->texCoords.negY.w, 0.6f,
        x + 1, y, z, block->texCoords.negY.x, block->texCoords.negY.w, 0.6f,
        x + 1, y, z + 1, block->texCoords.negY.x, block->texCoords.negY.y, 0.6f,
    };

    addFace(negativeYFace, block->transparent);
    if(block->transparent) {
        faces.push_back((Face) {
            .pos = glm::vec3(0.5f + x + chunk->blockPos.x, 0.1f + y, 0.5f + z + chunk->pos.y * 16),
            .indexBase = transparentIndex
        });
    }

    if(block->transparent) {
        transparentIndex += ADVANCE;
    } else {
        index += ADVANCE;
    }
}

void ChunkMesh::positiveZFace(u32 x, u32 y, u32 z, Block *block) {
    f32 positiveZFace[] {
        x, y, z + 1, block->texCoords.posZ.x, block->texCoords.posZ.y, 0.85f,
        x + 1, y, z + 1, block->texCoords.posZ.z, block->texCoords.posZ.y, 0.85f,
        x + 1, y + 1, z + 1, block->texCoords.posZ.z, block->texCoords.posZ.w, 0.85f,
        x + 1, y + 1, z + 1, block->texCoords.posZ.z, block->texCoords.posZ.w, 0.85f,
        x, y + 1, z + 1, block->texCoords.posZ.x, block->texCoords.posZ.w, 0.85f,
        x, y, z + 1, block->texCoords.posZ.x, block->texCoords.posZ.y, 0.85f,
    };

    addFace(positiveZFace, block->transparent);
    if(block->transparent) {
        faces.push_back((Face) {
            .pos = glm::vec3(0.5f + x + chunk->blockPos.x, 0.5f + y, 0.9f + z + chunk->pos.y * 16),
            .indexBase = transparentIndex
        });
    }

    if(block->transparent) {
        transparentIndex += ADVANCE;
    } else {
        index += ADVANCE;
    }
}

void ChunkMesh::negativeZFace(u32 x, u32 y, u32 z, Block *block) {
    f32 negativeZFace[] {
        x, y, z, block->texCoords.negZ.x, block->texCoords.negZ.y, 0.85f,
        x + 1, y, z, block->texCoords.negZ.z, block->texCoords.negZ.y, 0.85f,
        x + 1, y + 1, z, block->texCoords.negZ.z, block->texCoords.negZ.w, 0.85f,
        x + 1, y + 1, z, block->texCoords.negZ.z, block->texCoords.negZ.w, 0.85f,
        x, y + 1, z, block->texCoords.negZ.x, block->texCoords.negZ.w, 0.85f,
        x, y, z, block->texCoords.negZ.x, block->texCoords.negZ.y, 0.85f,
    };

    addFace(negativeZFace, block->transparent);
    if(block->transparent) {
        faces.push_back((Face) {
            .pos = glm::vec3(0.5f + x + chunk->blockPos.x, 0.5f + y, 0.1f + z + chunk->pos.y * 16),
            .indexBase = transparentIndex
        });
    }

    if(block->transparent) {
        transparentIndex += ADVANCE;
    } else {
        index += ADVANCE;
    }
}

void ChunkMesh::addFace(f32 *vertices, bool transparent) {
    for(i32 i = 0; i < 6 * 6; i++) {
        if(transparent) {
            transparentData.push_back(vertices[i]);
        } else {
            data.push_back(vertices[i]);
        }
    }
}

void ChunkMesh::mesh() {
    generating = true;
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
                    if(x > 0 && (chunk->get(x - 1, y, z)->isAir() || (!block.transparent && chunk->get(x - 1, y, z)->transparent))) {
                        negativeXFace(x, y, z, &block);
                    } else if(x == 0) {
                        if(left != nullptr && (left->get(Chunk::WIDTH - 1, y, z)->isAir() || (!block.transparent && left->get(Chunk::WIDTH - 1, y, z)->transparent))) {
                            negativeXFace(x, y, z, &block);
                        }
                    }

                    if(x < Chunk::WIDTH - 1 && (chunk->get(x + 1, y, z)->isAir() || (!block.transparent && chunk->get(x + 1, y, z)->transparent))) {
                        positiveXFace(x, y, z, &block);
                    } else if(x == Chunk::WIDTH - 1) {
                        if(right != nullptr && (right->get(0, y, z)->isAir() || (!block.transparent && right->get(0, y, z)->transparent))) {
                            positiveXFace(x, y, z, &block);
                        }
                    }

                    if(y > 0 && (chunk->get(x, y - 1, z)->isAir() || (!block.transparent && chunk->get(x, y - 1, z)->transparent))) {
                        negativeYFace(x, y, z, &block);
                    }

                    if(y < Chunk::HEIGHT - 1 && (chunk->get(x, y + 1, z)->isAir() || (!block.transparent && chunk->get(x, y + 1, z)->transparent))) {
                        positiveYFace(x, y, z, &block, chunk->get(x, y + 1, z)->type != block.type);
                    } else if(y == Chunk::HEIGHT - 1) {
                        positiveYFace(x, y, z, &block, true);
                    }

                    if(z > 0 && (chunk->get(x, y, z - 1)->isAir() || (!block.transparent && chunk->get(x, y, z - 1)->transparent))) {
                        negativeZFace(x, y, z, &block);
                    } else if(z == 0) {
                        if(back != nullptr && (back->get(x, y, Chunk::DEPTH - 1)->isAir() || (!block.transparent && back->get(x, y, Chunk::DEPTH - 1)->transparent))) {
                            negativeZFace(x, y, z, &block);
                        }
                    }

                    if(z < Chunk::DEPTH - 1 && (chunk->get(x, y, z + 1)->isAir() || (!block.transparent && chunk->get(x, y, z + 1)->transparent))) {
                        positiveZFace(x, y, z, &block);
                    } else if(z == Chunk::DEPTH - 1) {
                        if(front != nullptr && (front->get(x, y, 0)->isAir() || (!block.transparent && front->get(x, y, 0)->transparent))) {
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
    shouldBufferOpaque = true;
    shouldBufferTransparent = true;
    generating = false;
}

void ChunkMesh::reset() {
    data.clear();
    transparentData.clear();
    faces.clear();
    index = 0;
    transparentIndex = 0;
    isEmpty = true;
}

void ChunkMesh::destroy() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &transparentVbo);
    glDeleteVertexArrays(1, &transparentVao);
}