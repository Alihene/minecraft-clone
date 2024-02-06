#pragma once

#include "render/camera.hpp"
#include "block/block.hpp"

struct Player {
    glm::vec3 pos = glm::vec3(0.0f, 53.0f, 0.0f);
    glm::vec3 lastPos = glm::vec3(0.0f);

    struct {
        Block blocks[9];
        u32 activeSlot = 1;

        inline Block &get(u32 slot) {
            return blocks[slot - 1];
        }
    } hotbar;

    bool hasMoved = false;

    Player();

    void update();

    void tryBreakBlock();

    void tryPlaceBlock();

    void setHotbarSlot(u32 slot);

    Block *rayCast(glm::vec3 dir, f32 distance, glm::ivec3 *pos, glm::ivec3 *lastPos);
};