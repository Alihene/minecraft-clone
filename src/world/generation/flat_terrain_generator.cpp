#include "flat_terrain_generator.hpp"

#include "state.hpp"

void FlatTerrainGenerator::generateTerrain(Chunk *chunk) {
    for(u32 x = 0; x < Chunk::WIDTH; x++) {
        for(u32 z = 0; z < Chunk::DEPTH; z++) {
            chunk->set(x, 0, z, state.blockManager->getBlockByType(Block::STONE));
            for(i32 y = 1; y <= 3; y++) {
                chunk->set(x, y, z, state.blockManager->getBlockByType(Block::DIRT));
            }
            chunk->set(x, 4, z, state.blockManager->getBlockByType(Block::GRASS));
        }
    }
}