#include "overworld_terrain_generator.hpp"

#include "state.hpp"

void OverworldTerrainGenerator::generateTerrain(Chunk *chunk) {
    this->currentChunk = chunk;
    makeHeightMap();

    i32 maxHeight = 0;
    for(i32 x = 0; x < Chunk::WIDTH; x++) {
        for(i32 z = 0; z < Chunk::DEPTH; z++) {
            i32 height = heightMap[x][z];
            if(height > maxHeight) {
                maxHeight = height;
            }
        }
    }

    setBlocks(maxHeight);
}

void OverworldTerrainGenerator::setBlocks(i32 maxHeight) {
    for(i32 x = 0; x < Chunk::WIDTH; x++) {
        for(i32 z = 0; z < Chunk::DEPTH; z++) {
            i32 height = heightMap[x][z];

            for(i32 y = 0; y < height; y++) {
                if(height <= 20) {
                    if(height - y > 4) {
                        currentChunk->set(x, y, z, state.blockManager->getBlockByType(Block::STONE));
                    } else {
                        currentChunk->set(x, y, z, state.blockManager->getBlockByType(Block::SAND));
                    }
                } else {
                    if(height - y > 4) {
                        currentChunk->set(x, y, z, state.blockManager->getBlockByType(Block::STONE));
                    } else {
                        currentChunk->set(x, y, z, state.blockManager->getBlockByType(Block::DIRT));
                    }
                }
            }

            if(height <= 20) {
                currentChunk->set(x, height, z, state.blockManager->getBlockByType(Block::SAND));
            } else {
                currentChunk->set(x, height, z, state.blockManager->getBlockByType(Block::GRASS));
            }

            if(height <= 20) {
                for(i32 i = height + 1; i <= 20; i++) {
                    currentChunk->set(x, i, z, state.blockManager->getBlockByType(Block::WATER));
                }
            }
        }
    }
}

void OverworldTerrainGenerator::makeHeightMap() {
    glm::ivec2 location = currentChunk->pos;

    i32 offset = std::numeric_limits<i32>::max();

    for (i32 x = 0; x < Chunk::WIDTH; x++) {
        for (i32 z = 0; z < Chunk::DEPTH; z++)
        {
            f64 floatHeight = perlinNoise.noise2D_01(((x + (location.x * Chunk::WIDTH)) - offset) * 0.02, (((z) + (location.y * Chunk::DEPTH)) - offset) * 0.02);
            floatHeight *= 50;
            i32 height = (i32) floatHeight;
            heightMap[x][z] = height;
        }
    }
}