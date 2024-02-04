#pragma once

#include "chunk.hpp"
#include "util/util.hpp"

#include <vector>

struct Chunk;

struct ChunkMesh {
    static constexpr usize MESH_SIZE = 6 * 6 * 5 * 16 * 128 * 16 * sizeof(f32);

    static constexpr usize ADVANCE = 6 * 5;

    Chunk *chunk;

    u32 vao;
    u32 vbo;

    usize index = 0;

    bool shouldBuffer = true;
    bool shouldMesh = false;
    
    bool isEmpty = true;

    std::vector<f32> data;

    ChunkMesh();

    void positiveXFace(u32 x, u32 y, u32 z, Block *block);
    void negativeXFace(u32 x, u32 y, u32 z, Block *block);
    void positiveYFace(u32 x, u32 y, u32 z, Block *block);
    void negativeYFace(u32 x, u32 y, u32 z, Block *block);
    void positiveZFace(u32 x, u32 y, u32 z, Block *block);
    void negativeZFace(u32 x, u32 y, u32 z, Block *block);

    void addFace(f32 *vertices);

    void mesh();

    void reset();

    void destroy();
};