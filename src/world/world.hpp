#pragma once

#include "chunk.hpp"

#include <vector>

#include <glm/glm.hpp>

struct BlockSetData {
    glm::ivec2 chunkPos;
    glm::uvec3 blockPos;
    Block *block;
};

struct World {
    std::vector<Chunk*> chunks;
    std::vector<BlockSetData> pendingBlockChanges;

    void addChunk(glm::ivec2 pos);

    void setBlock(i32 x, i32 y, i32 z, Block *block);
    void setBlock(glm::ivec3 pos, Block *block);
    Block *getBlock(i32 x, i32 y, i32 z);
    Block *getBlock(glm::ivec3 pos);

    void updateChunks();

    void loadChunks();

    Chunk* getChunk(glm::ivec2 pos);

    static glm::ivec3 pointToBlockPos(glm::vec3 point);

    void destroy();
};