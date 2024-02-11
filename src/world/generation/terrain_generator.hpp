#pragma once

#include "world/chunk.hpp"

struct TerrainGenerator {
    virtual void generateTerrain(Chunk *chunk) = 0;
};