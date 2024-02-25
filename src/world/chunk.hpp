#pragma once

#include "block/block.hpp"
#include "chunk_mesh.hpp"
#include "util/util.hpp"

struct ChunkMesh;

struct Chunk {
    static constexpr u32 WIDTH = 16;
    static constexpr u32 HEIGHT = 128;
    static constexpr u32 DEPTH = 16;

    u16 blocks[WIDTH][HEIGHT][DEPTH];

    glm::ivec2 pos;
    glm::ivec2 blockPos;

    ChunkMesh *mesh;

    Chunk();

    Block *get(u32 x, u32 y, u32 z);
    void set(u32 x, u32 y, u32 z, Block *block, Block::Rotation rotation = Block::UP, bool shouldMesh = true);

    void destroy();
};