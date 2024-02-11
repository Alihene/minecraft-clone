#pragma once

#include "render/window.hpp"
#include "render/renderer.hpp"
#include "block/block_manager.hpp"
#include "world/world.hpp"
#include "player.hpp"
#include "util/settings.hpp"
#include "world/generation/terrain_generator.hpp"
#include "math/noise_generator.hpp"

#include <thread>

struct State {
    Window *window;
    Renderer *renderer;

    BlockManager *blockManager;

    World *world;
    Player *player;
    TerrainGenerator *terrainGenerator;

    Settings settings;

    std::string resourceDir;
};

extern State state;