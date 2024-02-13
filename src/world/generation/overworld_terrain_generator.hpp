#pragma once

#include <noise/PerlinNoise.hpp>

#include "terrain_generator.hpp"

struct OverworldTerrainGenerator : public TerrainGenerator {
    siv::PerlinNoise::seed_type seed = 0;
    siv::PerlinNoise perlinNoise {seed};

    i32 heightMap[Chunk::WIDTH][Chunk::DEPTH];

    Chunk *currentChunk = nullptr;

    OverworldTerrainGenerator() = default;

    void generateTerrain(Chunk *chunk) override;

    void setBlocks(i32 maxHeight);
    
    void makeHeightMap();
};