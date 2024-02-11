#include "noise_generator.hpp"
#include "world/chunk.hpp"

#include <cmath>

NoiseGenerator::NoiseGenerator(i32 seed, NoiseParams params) : seed(seed), params(params) {
    
}

f64 NoiseGenerator::getNoise(i32 n) {
    n += seed;
    n = (n << 13) ^ n;
    auto newN = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;

    return 1.0 - ((f64) newN / 1073741824.0);
}

f64 NoiseGenerator::getNoise(f64 x, f64 z) {
    return getNoise(x + z * 57.0);
}

f64 NoiseGenerator::lerp(f64 a, f64 b, f64 z) {
    f64 mu2 = (1 - std::cos(z * 3.14)) / 2;
    return (a * (1 - mu2) + b * mu2);
}

f64 NoiseGenerator::noise(f64 x, f64 z) {
    auto floorX = (f64) ((i32) x);
    auto floorZ = (f64) ((i32) z);

    auto s = 0.0, t = 0.0, u = 0.0,
         v = 0.0;

    s = getNoise(floorX, floorZ);
    t = getNoise(floorX + 1, floorZ);
    u = getNoise(
        floorX,
        floorZ + 1);
    v = getNoise(floorX + 1, floorZ + 1);

    auto rec1 = lerp(s, t, x - floorX);
    auto rec2 = lerp(
        u, v,
        x - floorX);
    auto rec3 =
        lerp(rec1, rec2,
             z - floorZ);
    return rec3;
}

f64 NoiseGenerator::getHeight(i32 x, i32 z, i32 chunkX, i32 chunkZ) {
    auto newX = (x + (chunkX * Chunk::WIDTH));
    auto newZ = (z + (chunkZ * Chunk::DEPTH));

    if (newX < 0 || newZ < 0) {
        // Water level 50
        return 50 - 1;
    }

    auto totalValue = 0.0;

    for (auto a = 0; a < params.octaves - 1; a++) {
        auto frequency = pow(2.0, a);
        auto amplitude = pow(params.roughness, a);
        totalValue +=
            noise(((f64) newX) * frequency / params.smoothness,
                ((f64) newZ) * frequency / params.smoothness) *
                amplitude;
    }

    auto val = (((totalValue / 2.1) + 1.2) * params.amplitude) +
               params.heightOffset;

    return val > 0 ? val : 1;
}