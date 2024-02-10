#include "world.hpp"

#include "state.hpp"

#include  <algorithm>

void World::addChunk(glm::ivec2 pos) {
    Chunk *chunk = new Chunk();
    chunk->pos = pos;
    chunk->blockPos = pos;
    chunk->blockPos.x *= Chunk::WIDTH;
    chunk->blockPos.y *= Chunk::DEPTH;

    std::vector<i32> blockSetIndices;

    for(i32 i = 0; i < pendingBlockChanges.size(); i++) {
        BlockSetData &data = pendingBlockChanges[i];
        if(data.chunkPos == pos) {
            chunk->set(data.blockPos.x, data.blockPos.y, data.blockPos.z, data.block);
            blockSetIndices.push_back(i);
        }
    }

    for(i32 i = blockSetIndices.size() - 1; i >= 0; i--) {
        pendingBlockChanges.erase(pendingBlockChanges.begin() + blockSetIndices[i]);
    }

    chunks.push_back(chunk);
}

void World::setBlock(i32 x, i32 y, i32 z, Block *block) {
    if(y > Chunk::HEIGHT - 1) {
        return;
    }

    i32 chunkPosX = (i32) floorf((f32) x / (f32) Chunk::WIDTH);
    i32 chunkPosZ = (i32) floorf((f32) z / (f32) Chunk::DEPTH);

    Chunk *chunk = nullptr;
    for(Chunk *c : chunks) {
        if(c->pos.x == chunkPosX && c->pos.y == chunkPosZ) {
            chunk = c;
            break;
        }
    }

    glm::ivec3 posInChunk = glm::ivec3(x % Chunk::WIDTH, y, z % Chunk::DEPTH);

    if(!chunk) {
        // Chunk isn't loaded, block needs to be set when this chunk loads
        pendingBlockChanges.push_back((BlockSetData) {
            .chunkPos = glm::ivec2(chunkPosX, chunkPosZ),
            .blockPos = posInChunk,
            .block = block
        });
        return;
    }

    chunk->set(posInChunk.x, posInChunk.y, posInChunk.z, block);

    if(posInChunk.x == 0) {
        Chunk *left = state.world->getChunk(glm::ivec2(chunk->pos.x - 1, chunk->pos.y));
        if(left && !left->get(Chunk::WIDTH - 1, posInChunk.y, posInChunk.z)->isAir()) {
            left->mesh->mesh();
        }
    } else if(posInChunk.x == Chunk::WIDTH - 1) {
        Chunk *right = state.world->getChunk(glm::ivec2(chunk->pos.x + 1, chunk->pos.y));
        if(right && !right->get(0, posInChunk.y, posInChunk.z)->isAir()) {
            right->mesh->mesh();
        }
    }

    if(posInChunk.z == 0) {
        Chunk *back = state.world->getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y - 1));
        if(back && !back->get(posInChunk.x, posInChunk.y, Chunk::DEPTH - 1)->isAir()) {
            back->mesh->mesh();
        }
    } else if(posInChunk.z == Chunk::DEPTH - 1) {
        Chunk *front = state.world->getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y + 1));
        if(front && !front->get(posInChunk.x, posInChunk.y, 0)->isAir()) {
            front->mesh->mesh();
        }
    }
}

void World::setBlock(glm::ivec3 pos, Block *block) {
    setBlock(pos.x, pos.y, pos.z, block);
}

Block *World::getBlock(i32 x, i32 y, i32 z) {
    i32 chunkPosX = (i32) floorf((f32) x / (f32) Chunk::WIDTH);
    i32 chunkPosZ = (i32) floorf((f32) z / (f32) Chunk::DEPTH);

    Chunk *chunk = nullptr;
    for(Chunk *c : chunks) {
        if(c->pos.x == chunkPosX && c->pos.y == chunkPosZ) {
            chunk = c;
            break;
        }
    }

    if(!chunk) {
        // Chunk is not loaded
        return nullptr;
    }

    return chunk->get(x % Chunk::WIDTH, y, z % Chunk::DEPTH);
}

Block *World::getBlock(glm::ivec3 pos) {
    return getBlock(pos.x, pos.y, pos.z);
}

void World::updateChunks() {
    for(Chunk *chunk : chunks) {
        ChunkMesh *mesh = chunk->mesh;

        if(mesh->shouldMesh) {
            mesh->mesh();
            return;
        }
    }
}

static bool chunkDepthCmp(Chunk *chunk1, Chunk *chunk2) {
    f32 distance1 = glm::distance(
        glm::vec2(chunk1->blockPos.x + Chunk::WIDTH / 2.0f, chunk1->blockPos.y + Chunk::DEPTH / 2.0f),
        glm::vec2(
            state.renderer->camera.pos.x,
            state.renderer->camera.pos.z));
    f32 distance2 = glm::distance(
        glm::vec2(chunk2->blockPos.x + Chunk::WIDTH / 2.0f, chunk2->blockPos.y + Chunk::DEPTH / 2.0f),
        glm::vec2(
            state.renderer->camera.pos.x,
            state.renderer->camera.pos.z));
    return distance1 > distance2;
}

void World::sortChunks() {
    std::sort(chunks.begin(), chunks.end(), chunkDepthCmp);
}

void World::loadChunks() {
    i32 renderDistance = state.settings.renderDistance;

    std::vector<i32> chunksToRemove;

    i32 chunkPosX = (i32) floorf(state.player->pos.x / (f32) Chunk::WIDTH);
    i32 chunkPosZ = (i32) floorf(state.player->pos.z / (f32) Chunk::DEPTH);

    for(i32 i = 0; i < chunks.size(); i++) {
        Chunk *chunk = chunks[i];

        if(abs(chunk->pos.x - chunkPosX) > renderDistance) {
            chunksToRemove.push_back(i);
        } else if(abs(chunk->pos.y - chunkPosZ) > renderDistance) {
            chunksToRemove.push_back(i);
        }
    }

    for(i32 i = chunksToRemove.size() - 1; i >= 0; i--) {
        chunks[chunksToRemove[i]]->destroy();
        delete chunks[chunksToRemove[i]];
        chunks.erase(chunks.begin() + chunksToRemove[i]);
    }

    for(i32 x = -renderDistance; x <= renderDistance; x++) {
        for(i32 z = -renderDistance; z <= renderDistance; z++) {
            bool chunkExists = false;

            for(Chunk *chunk : chunks) {
                if(chunk->pos.x == x + chunkPosX && chunk->pos.y == z + chunkPosZ) {
                    chunkExists = true;
                }
            }

            if(!chunkExists) {
                addChunk({x + chunkPosX, z + chunkPosZ});
            }
        }
    }
}

Chunk *World::getChunk(glm::ivec2 pos) {
    for(Chunk *chunk : chunks) {
        if(chunk->pos == pos) {
            return chunk;
        }
    }

    return nullptr;
}

glm::ivec3 World::pointToBlockPos(glm::vec3 point) {
    return glm::ivec3((i32) floorf(point.x), (i32) floorf(point.y), (i32) floorf(point.z));
}

void World::destroy() {
    for(Chunk *chunk : chunks) {
        delete chunk;
    }
}