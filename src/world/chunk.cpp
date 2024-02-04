#include "chunk.hpp"

#include "state.hpp"

#include <cstring>

Chunk::Chunk() {
    mesh = new ChunkMesh();
    mesh->chunk = this;

    std::memset(blocks, 0, sizeof(blocks));

    for(i32 x = 0; x < WIDTH; x++) {
        for(i32 y = 0; y < 50; y++) {
            for(i32 z = 0; z < DEPTH; z++) {
                set(x, y, z, state.blockManager->getBlockByType(Block::GRASS));
            }
        }
    }

    for(i32 x = 0; x < WIDTH; x++) {
        for(i32 z = 0; z < DEPTH; z++) {
            set(x, x + 50, z, state.blockManager->getBlockByType(Block::STONE));
        }
    }
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

    return &blocks[x][y][z];
}

void Chunk::set(u32 x, u32 y, u32 z, Block *block) {
    blocks[x][y][z] = *block;
    if(!mesh->shouldMesh) {
        mesh->shouldMesh = true;
    }
}

void Chunk::destroy() {
    mesh->destroy();
    delete mesh;
}