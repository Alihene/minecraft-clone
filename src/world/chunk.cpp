#include "chunk.hpp"

#include "state.hpp"

#include <cstring>

Chunk::Chunk() {
    mesh = new ChunkMesh();
    mesh->chunk = this;

    std::memset(blocks, 0, sizeof(blocks));
}

Block *Chunk::get(u32 x, u32 y, u32 z) {
    if(x < 0 || x >= WIDTH) {
        return nullptr;
    }

    if(y < 0 || y >= HEIGHT) {
        return nullptr;
    }

    if(z < 0 || z >= DEPTH) {
        return nullptr;
    }

    return Block::getPackedBlock(blocks[x][y][z]);
}

void Chunk::set(u32 x, u32 y, u32 z, Block *block, Block::Rotation rotation) {
    blocks[x][y][z] = block->pack(rotation);
    if(!mesh->shouldMesh && !mesh->generating) {
        mesh->shouldMesh = true;
    }
}

void Chunk::destroy() {
    mesh->destroy();
    delete mesh;
}