#pragma once

#include "terrain_generator.hpp"
#include "math/noise_generator.hpp"

struct OverworldTerrainGenerator : public TerrainGenerator {
    NoiseGenerator generator;

    i32 heightMap[Chunk::WIDTH][Chunk::DEPTH];

    Chunk *currentChunk = nullptr;

    OverworldTerrainGenerator();

    void generateTerrain(Chunk *chunk) override;

    void setBlocks(i32 maxHeight);
    
    void makeHeightMap();
};