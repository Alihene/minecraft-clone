#pragma once

#include "render/camera.hpp"
#include "block/block.hpp"

struct Player {
    glm::vec3 pos = glm::vec3(0.0f);

    Player() = default;

    void update();

    void tryBreakBlock();

    Block *rayCast(glm::vec3 dir, f32 distance, glm::ivec3 *pos);
};