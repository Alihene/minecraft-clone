#pragma once

#include "terrain_generator.hpp"

struct FlatTerrainGenerator : public TerrainGenerator {
    void generateTerrain(Chunk *chunk) override;
};