#pragma once

#include "chunk.hpp"
#include "util/util.hpp"

#include <vector>

struct Chunk;

struct ChunkMesh {
    struct Face {
        glm::vec3 pos;
        u32 indexBase;
    };

    static constexpr usize MESH_SIZE = 6 * 6 * 6 * 16 * 128 * 16 * sizeof(f32);

    static constexpr usize ADVANCE = 6 * 6;

    Chunk *chunk;

    u32 vao;
    u32 vbo;
    u32 transparentVao;
    u32 transparentVbo;

    usize index = 0;
    usize transparentIndex = 0;

    bool shouldBufferOpaque = true;
    bool shouldBufferTransparent = true;
    bool shouldMesh = false;
    
    bool isEmpty = true;

    std::vector<f32> data;
    std::vector<f32> transparentData;
    // For sorting
    std::vector<Face> faces;

    ChunkMesh();

    void sort();

    void positiveXFace(u32 x, u32 y, u32 z, Block *block);
    void negativeXFace(u32 x, u32 y, u32 z, Block *block);
    void positiveYFace(u32 x, u32 y, u32 z, Block *block);
    void negativeYFace(u32 x, u32 y, u32 z, Block *block);
    void positiveZFace(u32 x, u32 y, u32 z, Block *block);
    void negativeZFace(u32 x, u32 y, u32 z, Block *block);

    void addFace(f32 *vertices, bool transparent);

    void mesh();

    void reset();

    void destroy();
};