#include "block.hpp"
#include "state.hpp"

u16 Block::pack(Rotation rotation) {
    u16 packedBlock = type | ((rotation << 8) & 0xFF);
    
    return packedBlock;
}

Block::Rotation Block::getPackedRotation(u16 packedBlock) {
    u8 rotation = packedBlock >> 8;
    if(rotation == 0) {
        return UP;
    } else if(rotation == 1) {
        return DOWN;
    } else if(rotation == 2) {
        return NORTH;
    } else if(rotation == 3) {
        return EAST;
    } else if(rotation == 4) {
        return SOUTH;
    } else if(rotation == 5) {
        return WEST;
    } else {
        return UP;
    }
}

Block *Block::getPackedBlock(u16 packedBlock) {
    u8 blockType = ((u8*) &packedBlock)[0];
    return state.blockManager->getBlockByType((BlockType) blockType);
}