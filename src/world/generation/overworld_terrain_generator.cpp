#include "overworld_terrain_generator.hpp"

#include "state.hpp"

OverworldTerrainGenerator::OverworldTerrainGenerator() {
    random.seed(seed);
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
    std::vector<glm::ivec3> trees;

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
                std::uniform_int_distribution<i32> dist(0, 200);
                i32 num = dist(random);

                if(num == 0) {
                    trees.push_back(glm::ivec3(x, height + 1, z));
                }

                currentChunk->set(x, height, z, state.blockManager->getBlockByType(Block::GRASS));
            }

            if(height <= 20) {
                for(i32 i = height + 1; i <= 20; i++) {
                    currentChunk->set(x, i, z, state.blockManager->getBlockByType(Block::WATER));
                }
            }
        }
    }

    // Trees
    for(auto treePos : trees) {
        for(i32 y = treePos.y; y < treePos.y + 6; y++) {
            state.world->setBlock(treePos.x + currentChunk->blockPos.x, y, treePos.z + currentChunk->blockPos.y, state.blockManager->getBlockByType(Block::LOG));
        }

        for(i32 x = treePos.x - 2; x <= treePos.x + 2; x++) {
            for(i32 z = treePos.z - 2; z <= treePos.z + 2; z++) {
                if(x != treePos.x || z != treePos.z) {
                    state.world->setBlock(x + currentChunk->blockPos.x, treePos.y + 3, z + currentChunk->blockPos.y, state.blockManager->getBlockByType(Block::LEAVES));
                    state.world->setBlock(x + currentChunk->blockPos.x, treePos.y + 4, z + currentChunk->blockPos.y, state.blockManager->getBlockByType(Block::LEAVES));
                }
            }
        }

        for(i32 x = treePos.x - 1; x <= treePos.x + 1; x++) {
            for(i32 z = treePos.z - 1; z <= treePos.z + 1; z++) {
                if(x != treePos.x || z != treePos.z) {
                    state.world->setBlock(x + currentChunk->blockPos.x, treePos.y + 5, z + currentChunk->blockPos.y, state.blockManager->getBlockByType(Block::LEAVES));
                }
                state.world->setBlock(x + currentChunk->blockPos.x, treePos.y + 6, z + currentChunk->blockPos.y, state.blockManager->getBlockByType(Block::LEAVES));
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
            f64 floatHeight = perlinNoise.octave2D_01(((x + (location.x * Chunk::WIDTH)) - offset) * 0.015, (((z) + (location.y * Chunk::DEPTH)) - offset) * 0.015, 3, 0.35f);
            floatHeight *= 50;
            i32 height = (i32) floatHeight;
            heightMap[x][z] = height;
        }
    }
}