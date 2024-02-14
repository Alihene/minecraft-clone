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
}

void Player::update(f32 timestep) {
    f32 speed = 50.0f * timestep;

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

    state.renderer->camera.pos = pos;

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

    lastPos = pos;

    std::cout << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
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

    glm::vec3 currentPos = pos;

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