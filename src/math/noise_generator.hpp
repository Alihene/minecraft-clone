#pragma once

#include "util/util.hpp"

struct NoiseParams {
    i32 octaves;
    i32 amplitude;
    i32 smoothness;
    i32 heightOffset;

    f64 roughness;
};

struct NoiseGenerator {
    i32 seed;
    NoiseParams params;

    NoiseGenerator() = default;
    NoiseGenerator(i32 seed, NoiseParams params);

    f64 getHeight(i32 x, i32 z, i32 chunkX, i32 chunkZ);

    f64 getNoise(i32 n);
    f64 getNoise(f64 x, f64 z);

    f64 lerp(f64 a, f64 b, f64 z);

    f64 noise(f64 x, f64 z);
};