#include "overworld_terrain_generator.hpp"

#include "state.hpp"

OverworldTerrainGenerator::OverworldTerrainGenerator() {
    NoiseParams params;
    params.octaves = 5;
    params.amplitude = 100;
    params.smoothness = 400;
    params.heightOffset = 0;
    params.roughness = 0.75;

    generator = NoiseGenerator(432, params);
}

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
                if(height - y > 4) {
                    currentChunk->set(x, y, z, state.blockManager->getBlockByType(Block::STONE));
                } else {
                    currentChunk->set(x, y, z, state.blockManager->getBlockByType(Block::DIRT));
                }
            }

            currentChunk->set(x, height, z, state.blockManager->getBlockByType(Block::GRASS));
        }
    }
}

void OverworldTerrainGenerator::makeHeightMap() {
    glm::ivec2 location = currentChunk->pos;

    for (i32 x = 0; x < Chunk::WIDTH; x++) {
        for (i32 z = 0; z < Chunk::DEPTH; z++)
        {
            i32 height = generator.getHeight(x, z, location.x, location.y);
            heightMap[x][z] = height;
        }
    }
}