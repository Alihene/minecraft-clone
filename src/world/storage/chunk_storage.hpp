#pragma once

#include "block/block.hpp"
#include "util/util.hpp"
#include "world/chunk.hpp"

// Chunk storage data
struct ChunkData {
    u16 data[Chunk::WIDTH][Chunk::HEIGHT][Chunk::DEPTH];

    ChunkData();

    Block *get(i32 x, i32 y, i32 z);
    void setData(Chunk *chunk);
    void writeData(Chunk *chunk);

    void destroy();
};

// 50 by 50 region of chunks
struct ChunkRegion {
    static constexpr u32 SIZE = 50;

    // Chunkpos of region
    glm::ivec2 pos;

    ChunkData *chunks[SIZE][SIZE];
    std::vector<glm::ivec2> addedPositions;

    ChunkData *get(i32 x, i32 y);

    void destroy();

    static glm::ivec2 chunkPosToRegionPos(i32 x, i32 y);
};

struct ChunkStorage {
    std::vector<ChunkRegion> regions;

    ChunkData *getChunkData(i32 x, i32 y);

    void setChunk(Chunk *chunk);

    void addData(ChunkData *data, i32 x, i32 y);

    void destroy();
};