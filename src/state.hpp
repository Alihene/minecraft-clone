#pragma once

#include "render/window.hpp"
#include "render/renderer.hpp"
#include "block/block_manager.hpp"
#include "world/world.hpp"
#include "player.hpp"
#include "util/settings.hpp"

#include <thread>

struct State {
    Window *window;
    Renderer *renderer;

    BlockManager *blockManager;

    World *world;
    Player *player;

    Settings settings;

    std::string resourceDir;
};

extern State state;