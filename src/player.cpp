#include "player.hpp"

#include "state.hpp"

void Player::update() {
    f32 speed = 0.5f;

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
}

void Player::tryBreakBlock() {
    glm::ivec3 blockPos;
    Block *block = rayCast(state.renderer->camera.viewDirection, 5, &blockPos);

    if(!block) {
        return;
    }

    state.world->setBlock(blockPos, state.blockManager->getBlockByType(Block::AIR));
}

Block *Player::rayCast(glm::vec3 dir, f32 distance, glm::ivec3 *outPos) {
    f32 deltaMagnitude = 0.01f;
    glm::vec3 delta = glm::normalize(dir) * deltaMagnitude;

    glm::vec3 currentPos = pos;

    for(f32 distanceTravelled = 0.0f; distanceTravelled < distance; distanceTravelled += deltaMagnitude) {
        glm::ivec3 blockPos = World::pointToBlockPos(currentPos);
        Block *block = state.world->getBlock(blockPos);

        if(block && !block->isAir()) {
            *outPos = blockPos;
            return block;
        }

        currentPos += delta;
    }

    return nullptr;
}