#include "player.hpp"

#include "state.hpp"

Player::Player() {
    hotbar.blocks[0] = *state.blockManager->getBlockByType(Block::GRASS);
    hotbar.blocks[1] = *state.blockManager->getBlockByType(Block::DIRT);
    hotbar.blocks[2] = *state.blockManager->getBlockByType(Block::STONE);
    hotbar.blocks[3] = *state.blockManager->getBlockByType(Block::GLASS);
    hotbar.blocks[4] = *state.blockManager->getBlockByType(Block::WOOD_PLANKS);
    hotbar.blocks[5] = *state.blockManager->getBlockByType(Block::COBBLESTONE);
    hotbar.blocks[6] = *state.blockManager->getBlockByType(Block::LOG);
    hotbar.blocks[7] = *state.blockManager->getBlockByType(Block::WATER);
    hotbar.blocks[8] = *state.blockManager->getBlockByType(Block::SAND);

    aabb.size = glm::vec3(1.0f, 2.0f, 1.0f);
}

void Player::update(f32 timestep) {
    f32 speed = 8.0f * timestep;

    if(state.window->keyPressed(GLFW_KEY_W)) {
        pos += glm::vec3(
            glm::cos(glm::radians(state.renderer->camera.yaw)),
            0.0f,
            glm::sin(glm::radians(state.renderer->camera.yaw))) * speed;
    }

    if(state.window->keyPressed(GLFW_KEY_S)) {
        pos -= glm::vec3(
            glm::cos(glm::radians(state.renderer->camera.yaw)),
            0.0f,
            glm::sin(glm::radians(state.renderer->camera.yaw))) * speed;
    }

    if(state.window->keyPressed(GLFW_KEY_A)) {
        pos -= glm::normalize(glm::cross(state.renderer->camera.front, state.renderer->camera.up)) * speed;
    }

    if(state.window->keyPressed(GLFW_KEY_D)) {
        pos += glm::normalize(glm::cross(state.renderer->camera.front, state.renderer->camera.up)) * speed;
    }

    if(state.window->keyPressed(GLFW_KEY_SPACE)) {
        pos += state.renderer->camera.up * speed;
    };

    if(state.window->keyPressed(GLFW_KEY_LEFT_SHIFT)) {
        pos -= state.renderer->camera.up * speed;
    }

    if(pos != lastPos) {
        // Update transparent geometry
        i32 chunkPosX = (i32) floorf((f32) pos.x / (f32) Chunk::WIDTH);
        i32 chunkPosZ = (i32) floorf((f32) pos.z / (f32) Chunk::DEPTH);
        Chunk *chunk = state.world->getChunk(glm::ivec2(chunkPosX, chunkPosZ));

        if(chunk) {
            chunk->mesh->sort();
        }
    }

    i32 chunkPosX = (i32) floorf((f32) pos.x / (f32) Chunk::WIDTH);
    i32 chunkPosZ = (i32) floorf((f32) pos.z / (f32) Chunk::DEPTH);

    i32 lastChunkPosX = (i32) floorf((f32) lastPos.x / (f32) Chunk::WIDTH);
    i32 lastChunkPosZ = (i32) floorf((f32) lastPos.z / (f32) Chunk::DEPTH);

    if(!hasMoved) {
        hasMoved = true;
        state.world->chunkMutex.lock();
        state.world->sortChunks();
        state.world->chunkMutex.unlock();
    } else if(chunkPosX != lastChunkPosX || chunkPosZ != lastChunkPosZ) {
        state.world->chunkMutex.lock();
        state.world->sortChunks();
        state.world->chunkMutex.unlock();

        for(i32 x = chunkPosX - 1; x <= chunkPosX + 1; x++) {
            for(i32 z = chunkPosZ - 1; z <= chunkPosZ + 1; z++) {
                Chunk *sideChunk = state.world->getChunk(glm::ivec2(x, z));

                if(sideChunk) {
                    sideChunk->mesh->sort();
                }
            }
        }
    }

    //aabb.pos = glm::vec3(pos.x - (aabb.size.x / 2.0f), pos.y, pos.z - (aabb.size.z / 2.0f));
    collide(glm::vec3(pos.x - lastPos.x, 0.0f, 0.0f));
    aabb.pos = glm::vec3(pos.x - (aabb.size.x / 2.0f), pos.y, pos.z - (aabb.size.z / 2.0f));
    collide(glm::vec3(0.0f, pos.y - lastPos.y, 0.0f));
    aabb.pos = glm::vec3(pos.x - (aabb.size.x / 2.0f), pos.y, pos.z - (aabb.size.z / 2.0f));
    collide(glm::vec3(0.0f, 0.0f, pos.z - lastPos.z));
    aabb.pos = glm::vec3(pos.x - (aabb.size.x / 2.0f), pos.y, pos.z - (aabb.size.z / 2.0f));

    // Player is 2 blocks tall, camera pos must be 1 block above
    state.renderer->camera.pos = pos + glm::vec3(0.0f, 1.5f, 0.0f);

    lastPos = pos;
}

void Player::collide(glm::vec3 vel) {
    // Block *surroundingBlocks[3 * 3 * 3] = {nullptr};
    // AABB aabbs[3 * 3 * 3];

    // i32 a = 0;
    // for(i32 x = World::pointToBlockPos(pos).x - 1; x <= World::pointToBlockPos(pos).x + 1; x++) {
    //     for(i32 y = World::pointToBlockPos(pos).y - 1; y <= World::pointToBlockPos(pos).y + 1; y++) {
    //         for(i32 z = World::pointToBlockPos(pos).z - 1; z <= World::pointToBlockPos(pos).z + 1; z++) {
    //             if(y > 0 && y < Chunk::HEIGHT) {
    //                 surroundingBlocks[a] = state.world->getBlock(x, y, z);
    //                 AABB blockAabb = {
    //                     .pos = glm::vec3(x, y, z),
    //                     .size = glm::vec3(1.0f)
    //                 };
    //                 aabbs[a] = blockAabb;
    //             }
    //             a++;
    //         }
    //     }
    // }

    // for(i32 i = 0; i < 3 * 3 * 3; i++) {
    //     Block *block = surroundingBlocks[i];

    //     if(block && !block->isAir()) {
    //         AABB blockAabb = aabbs[i];
    //         if(aabb.collidingWith(blockAabb)) {
    //             if(vel.x < 0) {
    //                 while(aabb.collidingWith(blockAabb)) {
    //                     pos.x += 0.01f;
    //                     aabb.pos = glm::vec3(pos.x - (aabb.size.x / 2.0f), pos.y, pos.z - (aabb.size.z / 2.0f));
    //                 }
    //                 // pos.x = lastPos.x;
    //             }

    //             if(vel.x > 0) {
    //                 while(aabb.collidingWith(blockAabb)) {
    //                     pos.x -= 0.01f;
    //                     aabb.pos = glm::vec3(pos.x - (aabb.size.x / 2.0f), pos.y, pos.z - (aabb.size.z / 2.0f));
    //                 }
    //                 // pos.x = lastPos.x;
    //             }

    //             if(vel.y < 0) {
    //                 pos.y = blockAabb.pos.y + blockAabb.size.y;
    //             }

    //             if(vel.y > 0) {
    //                 pos.y = blockAabb.pos.y - aabb.size.y;
    //             }

    //             if(vel.z < 0) {
    //                 while(aabb.collidingWith(blockAabb)) {
    //                     pos.z += 0.01f;
    //                     aabb.pos = glm::vec3(pos.x - (aabb.size.x / 2.0f), pos.y, pos.z - (aabb.size.z / 2.0f));
    //                 }
    //                 // pos.z = lastPos.z;
    //             }

    //             if(vel.z > 0) {
    //                 while(aabb.collidingWith(blockAabb)) {
    //                     pos.z -= 0.01f;
    //                     aabb.pos = glm::vec3(pos.x - (aabb.size.x / 2.0f), pos.y, pos.z - (aabb.size.z / 2.0f));
    //                 }
    //                 // pos.z = lastPos.z;
    //             }
    //         }
    //     }
    // }

    // // if(surroundingBlocks[0] && !surroundingBlocks[0]->isAir()) {
    // //     AABB blockAabb = {
    // //         .pos = glm::vec3(World::pointToBlockPos(lastPos) - glm::ivec3(0.0f, 1.0f, 0.0f)),
    // //         .size = glm::vec3(1.0f)
    // //     };

    // //     //std::cout << "Yes" << std::endl;

    // //     if(aabb.collidingWith(blockAabb)) {
    // //         pos.y = blockAabb.pos.y + 1;
    // //         state.renderer->camera.pos = pos + glm::vec3(0.0f, 1.5f, 0.0f);
    // //     }
    // // }
}

void Player::tryBreakBlock() {
    glm::ivec3 blockPos;
    Block *block = rayCast(state.renderer->camera.front, 5, &blockPos, nullptr);

    if(!block) {
        return;
    }

    state.world->setBlockAndMesh(blockPos, state.blockManager->getBlockByType(Block::AIR));
}

void Player::tryPlaceBlock() {
    if(hotbar.get(hotbar.activeSlot).isAir()) {
        return;
    }

    // blockPos is irrelevant, we need placePos
    glm::ivec3 blockPos;
    glm::ivec3 placePos;
    if(rayCast(state.renderer->camera.front, 5, &blockPos, &placePos)) {
        state.world->setBlockAndMesh(placePos, &hotbar.get(hotbar.activeSlot));
    }
}

void Player::setHotbarSlot(u32 slot) {
    hotbar.activeSlot = slot;
}

Block *Player::rayCast(glm::vec3 dir, f32 distance, glm::ivec3 *outPos, glm::ivec3 *lastPos) {
    f32 deltaMagnitude = 0.01f;
    glm::vec3 delta = dir * deltaMagnitude;

    glm::vec3 currentPos = pos + glm::vec3(0.0f, 1.5f, 0.0f);

    for(f32 distanceTravelled = 0.0f; distanceTravelled < distance; distanceTravelled += deltaMagnitude) {
        glm::ivec3 blockPos = World::pointToBlockPos(currentPos);
        Block *block = state.world->getBlock(blockPos);

        if(block && !block->isAir()) {
            *outPos = blockPos;
            return block;
        }

        if(lastPos) {
            *lastPos = blockPos;
        }

        currentPos += delta;
    }

    return nullptr;
}