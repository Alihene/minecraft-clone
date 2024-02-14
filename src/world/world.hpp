#pragma once

#include "chunk.hpp"
#include "storage/chunk_storage.hpp"

#include <vector>
#include <mutex>

#include <glm/glm.hpp>

struct BlockSetData {
    glm::ivec2 chunkPos;
    glm::uvec3 blockPos;
    Block *block;
};

struct World {
    std::vector<Chunk*> chunks;
    std::mutex chunkMutex;
    std::vector<BlockSetData> pendingBlockChanges;

    ChunkStorage storage;

    void addChunk(glm::ivec2 pos);

    void setBlock(i32 x, i32 y, i32 z, Block *block);
    void setBlockAndMesh(i32 x, i32 y, i32 z, Block *block);
    void setBlock(glm::ivec3 pos, Block *block);
    void setBlockAndMesh(glm::ivec3 pos, Block *block);
    Block *getBlock(i32 x, i32 y, i32 z);
    Block *getBlock(glm::ivec3 pos);

    void updateChunks();
    void sortChunks();

    void loadChunks();

    void updateChunkStorage(Chunk *chunk);

    Chunk* getChunk(glm::ivec2 pos);

    static glm::ivec3 pointToBlockPos(glm::vec3 point);

    void destroy();
};