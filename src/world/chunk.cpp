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
                set(x, y, z, state.blockManager->getBlockByType(Block::STONE));
            }
        }
    }

    for(i32 x = 0; x < WIDTH; x++) {
        for(i32 y = 50; y < 52; y++) {
            for(i32 z = 0; z < DEPTH; z++) {
                set(x, y, z, state.blockManager->getBlockByType(Block::DIRT));
            }
        }
    }

    for(i32 x = 0; x < WIDTH; x++) {
        for(i32 z = 0; z < DEPTH; z++) {
            set(x, 52, z, state.blockManager->getBlockByType(Block::GRASS));
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

    return Block::getPackedBlock(blocks[x][y][z]);
}

void Chunk::set(u32 x, u32 y, u32 z, Block *block, Block::Rotation rotation) {
    blocks[x][y][z] = block->pack(rotation);
    if(!mesh->shouldMesh) {
        mesh->shouldMesh = true;
    }
}

void Chunk::destroy() {
    mesh->destroy();
    delete mesh;
}