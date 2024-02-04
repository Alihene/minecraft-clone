#include "block_manager.hpp"

BlockManager::BlockManager() {
    blocks.push_back((Block) {
        .type = Block::AIR,
        .texCoords = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
        .transparent = true,
        .solid = false
    });
    blocks.push_back((Block) {
        .type = Block::GRASS,
        .texCoords = glm::vec4(0.0f, 0.0f, 1.0f / 8.0f, 1.0f / 8.0f),
        .transparent = false,
        .solid = true
    });
    blocks.push_back((Block) {
        .type = Block::STONE,
        .texCoords = glm::vec4(1.0f / 8.0f, 0.0f, 2.0f / 8.0f, 1.0f / 8.0f),
        .transparent = false,
        .solid = true
    });
}

Block *BlockManager::getBlockByType(Block::BlockType type) {
    for(Block &block : blocks) {
        if(block.type == type) {
            return &block;
        }
    }
    return nullptr;
}