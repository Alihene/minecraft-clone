#pragma once

#include <noise/PerlinNoise.hpp>

#include <random>

#include "terrain_generator.hpp"

struct OverworldTerrainGenerator : public TerrainGenerator {
    siv::PerlinNoise::seed_type seed = 0;
    siv::PerlinNoise perlinNoise {seed};

    std::mt19937 random;

    i32 heightMap[Chunk::WIDTH][Chunk::DEPTH];

    Chunk *currentChunk = nullptr;

    OverworldTerrainGenerator();

    void generateTerrain(Chunk *chunk) override;

    void setBlocks(i32 maxHeight);
    
    void makeHeightMap();
};