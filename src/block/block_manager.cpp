#include "block_manager.hpp"

BlockManager::BlockManager() {
    blocks.push_back((Block) {
        .type = Block::AIR,
        .texCoords = {
            .negX = {0.0f, 0.0f, 0.0f, 0.0f},
            .posX = {0.0f, 0.0f, 0.0f, 0.0f},
            .negY = {0.0f, 0.0f, 0.0f, 0.0f},
            .posY = {0.0f, 0.0f, 0.0f, 0.0f},
            .negZ = {0.0f, 0.0f, 0.0f, 0.0f},
            .posZ = {0.0f, 0.0f, 0.0f, 0.0f}
        },
        .transparent = true,
        .solid = false
    });
    blocks.push_back((Block) {
        .type = Block::GRASS,
        .texCoords = {
            .negX = {3.0f / 8.0f, 0.0f, 4.0f / 8.0f, 1.0f / 8.0f},
            .posX = {3.0f / 8.0f, 0.0f, 4.0f / 8.0f, 1.0f / 8.0f},
            .negY = {2.0f / 8.0f, 0.0f, 3.0f / 8.0f, 1.0f / 8.0f},
            .posY = {0.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f},
            .negZ = {3.0f / 8.0f, 0.0f, 4.0f / 8.0f, 1.0f / 8.0f},
            .posZ = {3.0f / 8.0f, 0.0f, 4.0f / 8.0f, 1.0f / 8.0f}
        },
        .transparent = false,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::DIRT,
        .texCoords = {
            .negX = {2.0f / 8.0f, 0.0f, 3.0f / 8.0f, 1.0f / 8.0f},
            .posX = {2.0f / 8.0f, 0.0f, 3.0f / 8.0f, 1.0f / 8.0f},
            .negY = {2.0f / 8.0f, 0.0f, 3.0f / 8.0f, 1.0f / 8.0f},
            .posY = {2.0f / 8.0f, 0.0f, 3.0f / 8.0f, 1.0f / 8.0f},
            .negZ = {2.0f / 8.0f, 0.0f, 3.0f / 8.0f, 1.0f / 8.0f},
            .posZ = {2.0f / 8.0f, 0.0f, 3.0f / 8.0f, 1.0f / 8.0f}
        },
        .transparent = false,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::STONE,
        .texCoords = {
            .negX = {1.0f / 8.0f, 0.0f, 2.0f / 8.0f, 1.0f / 8.0f},
            .posX = {1.0f / 8.0f, 0.0f, 2.0f / 8.0f, 1.0f / 8.0f},
            .negY = {1.0f / 8.0f, 0.0f, 2.0f / 8.0f, 1.0f / 8.0f},
            .posY = {1.0f / 8.0f, 0.0f, 2.0f / 8.0f, 1.0f / 8.0f},
            .negZ = {1.0f / 8.0f, 0.0f, 2.0f / 8.0f, 1.0f / 8.0f},
            .posZ = {1.0f / 8.0f, 0.0f, 2.0f / 8.0f, 1.0f / 8.0f}
        },
        .transparent = false,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::GLASS,
        .texCoords = {
            .negX = {4.0f / 8.0f, 0.0f, 5.0f / 8.0f, 1.0f / 8.0f},
            .posX = {4.0f / 8.0f, 0.0f, 5.0f / 8.0f, 1.0f / 8.0f},
            .negY = {4.0f / 8.0f, 0.0f, 5.0f / 8.0f, 1.0f / 8.0f},
            .posY = {4.0f / 8.0f, 0.0f, 5.0f / 8.0f, 1.0f / 8.0f},
            .negZ = {4.0f / 8.0f, 0.0f, 5.0f / 8.0f, 1.0f / 8.0f},
            .posZ = {4.0f / 8.0f, 0.0f, 5.0f / 8.0f, 1.0f / 8.0f}
        },
        .transparent = true,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::WOOD_PLANKS,
        .texCoords = {
            .negX = {5.0f / 8.0f, 0.0f, 6.0f / 8.0f, 1.0f / 8.0f},
            .posX = {5.0f / 8.0f, 0.0f, 6.0f / 8.0f, 1.0f / 8.0f},
            .negY = {5.0f / 8.0f, 0.0f, 6.0f / 8.0f, 1.0f / 8.0f},
            .posY = {5.0f / 8.0f, 0.0f, 6.0f / 8.0f, 1.0f / 8.0f},
            .negZ = {5.0f / 8.0f, 0.0f, 6.0f / 8.0f, 1.0f / 8.0f},
            .posZ = {5.0f / 8.0f, 0.0f, 6.0f / 8.0f, 1.0f / 8.0f}
        },
        .transparent = false,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::COBBLESTONE,
        .texCoords = {
            .negX = {6.0f / 8.0f, 0.0f, 7.0f / 8.0f, 1.0f / 8.0f},
            .posX = {6.0f / 8.0f, 0.0f, 7.0f / 8.0f, 1.0f / 8.0f},
            .negY = {6.0f / 8.0f, 0.0f, 7.0f / 8.0f, 1.0f / 8.0f},
            .posY = {6.0f / 8.0f, 0.0f, 7.0f / 8.0f, 1.0f / 8.0f},
            .negZ = {6.0f / 8.0f, 0.0f, 7.0f / 8.0f, 1.0f / 8.0f},
            .posZ = {6.0f / 8.0f, 0.0f, 7.0f / 8.0f, 1.0f / 8.0f}
        },
        .transparent = false,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::LOG,
        .texCoords = {
            .negX = {7.0f / 8.0f, 0.0f, 1.0f, 1.0f / 8.0f},
            .posX = {7.0f / 8.0f, 0.0f, 1.0f, 1.0f / 8.0f},
            .negY = {0.0f, 1.0f / 8.0f, 1.0f / 8.0f, 2.0f / 8.0f},
            .posY = {0.0f, 1.0f / 8.0f, 1.0f / 8.0f, 2.0f / 8.0f},
            .negZ = {7.0f / 8.0f, 0.0f, 1.0f, 1.0f / 8.0f},
            .posZ = {7.0f / 8.0f, 0.0f, 1.0f, 1.0f / 8.0f}
        },
        .transparent = false,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::WATER,
        .texCoords = {
            .negX = {1.0f / 8.0f, 1.0f / 8.0f, 2.0f / 8.0f, 2.0f / 8.0f},
            .posX = {1.0f / 8.0f, 1.0f / 8.0f, 2.0f / 8.0f, 2.0f / 8.0f},
            .negY = {1.0f / 8.0f, 1.0f / 8.0f, 2.0f / 8.0f, 2.0f / 8.0f},
            .posY = {1.0f / 8.0f, 1.0f / 8.0f, 2.0f / 8.0f, 2.0f / 8.0f},
            .negZ = {1.0f / 8.0f, 1.0f / 8.0f, 2.0f / 8.0f, 2.0f / 8.0f},
            .posZ = {1.0f / 8.0f, 1.0f / 8.0f, 2.0f / 8.0f, 2.0f / 8.0f}
        },
        .transparent = true,
        .solid = false
    });
    blocks.push_back((Block) {
        .type = Block::SAND,
        .texCoords = {
            .negX = {2.0f / 8.0f, 1.0f / 8.0f, 3.0f / 8.0f, 2.0f / 8.0f},
            .posX = {2.0f / 8.0f, 1.0f / 8.0f, 3.0f / 8.0f, 2.0f / 8.0f},
            .negY = {2.0f / 8.0f, 1.0f / 8.0f, 3.0f / 8.0f, 2.0f / 8.0f},
            .posY = {2.0f / 8.0f, 1.0f / 8.0f, 3.0f / 8.0f, 2.0f / 8.0f},
            .negZ = {2.0f / 8.0f, 1.0f / 8.0f, 3.0f / 8.0f, 2.0f / 8.0f},
            .posZ = {2.0f / 8.0f, 1.0f / 8.0f, 3.0f / 8.0f, 2.0f / 8.0f}
        },
        .transparent = false,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::LEAVES,
        .texCoords = {
            .negX = {3.0f / 8.0f, 1.0f / 8.0f, 4.0f / 8.0f, 2.0f / 8.0f},
            .posX = {3.0f / 8.0f, 1.0f / 8.0f, 4.0f / 8.0f, 2.0f / 8.0f},
            .negY = {3.0f / 8.0f, 1.0f / 8.0f, 4.0f / 8.0f, 2.0f / 8.0f},
            .posY = {3.0f / 8.0f, 1.0f / 8.0f, 4.0f / 8.0f, 2.0f / 8.0f},
            .negZ = {3.0f / 8.0f, 1.0f / 8.0f, 4.0f / 8.0f, 2.0f / 8.0f},
            .posZ = {3.0f / 8.0f, 1.0f / 8.0f, 4.0f / 8.0f, 2.0f / 8.0f}
        },
        .transparent = true,
        .solid = true
    });
}

Block *BlockManager::getBlockByType(Block::BlockType type) {
    if(type <= BLOCK_TYPE_LAST && type > -1) {
            return &blocks[type];
    }
    return nullptr;
}