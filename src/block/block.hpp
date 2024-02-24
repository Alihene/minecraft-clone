#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "util/util.hpp"

struct Block {
    enum BlockType {
        AIR = 0,
        GRASS = 1,
        DIRT = 2,
        STONE = 3,
        GLASS = 4,
        WOOD_PLANKS = 5,
        COBBLESTONE = 6,
        LOG = 7,
        WATER = 8,
        SAND = 9,
        LEAVES = 10
    };

    enum Rotation {
        UP = 0,
        DOWN = 2,
        NORTH = 3,
        EAST = 4,
        SOUTH = 5,
        WEST = 6
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

    u16 pack(Rotation rotation = UP);

    static Rotation getPackedRotation(u16 packedBlock);

    static Block *getPackedBlock(u16 packedBlock);
};

#define BLOCK_TYPE_LAST Block::LEAVES