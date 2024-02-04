#pragma once

#include <vector>

#include "block.hpp"

struct BlockManager {
    std::vector<Block> blocks;

    BlockManager();

    Block *getBlockByType(Block::BlockType type);
};