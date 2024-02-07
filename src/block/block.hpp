#pragma once

#include <iostream>
#include <glm/glm.hpp>

struct Block {
    enum BlockType {
        AIR = 0,
        GRASS = 1,
        DIRT = 2,
        STONE = 3,
        GLASS = 4,
        WOOD_PLANKS = 5,
        COBBLESTONE = 6,
        LOG = 7
    };

    BlockType type;

    struct {
        glm::vec4 negX;
        glm::vec4 posX;
        glm::vec4 negY;
        glm::vec4 posY;
        glm::vec4 negZ;
        glm::vec4 posZ;
    } texCoords;

    bool transparent;

    bool solid;

    inline bool isAir() {
        return type == AIR;
    }
};