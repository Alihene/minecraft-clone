#pragma once

#include <iostream>
#include <glm/glm.hpp>

struct Block {
    enum BlockType {
        AIR = 0,
        GRASS = 1,
        STONE = 2
    };

    BlockType type;

    glm::vec4 texCoords;

    bool transparent;

    bool solid;

    inline bool isAir() {
        return type == AIR;
    }
};