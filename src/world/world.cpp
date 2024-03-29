#include "world.hpp"

#include "state.hpp"

#include <algorithm>
#include <cstring>

void World::addChunk(glm::ivec2 pos) {
    Chunk *chunk = new Chunk();
    chunk->pos = pos;
    chunk->blockPos = pos;
    chunk->blockPos.x *= Chunk::WIDTH;
    chunk->blockPos.y *= Chunk::DEPTH;

    ChunkData *data = storage.getChunkData(chunk->pos.x, chunk->pos.y);

    if(!data) {
        state.terrainGenerator->generateTerrain(chunk);
        storage.setChunk(chunk);
    } else {
        data->writeData(chunk);
    }

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

    data = storage.getChunkData(pos.x, pos.y);
    data->setData(chunk);

    chunks.push_back(chunk);

    Chunk *adjacentChunks[] = {
        getChunk(glm::ivec2(chunk->pos.x - 1, chunk->pos.y)),
        getChunk(glm::ivec2(chunk->pos.x + 1, chunk->pos.y)),
        getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y + 1)),
        getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y - 1)),
    };

    for(Chunk *c : adjacentChunks) {
        if(c && !c->mesh->generating) {
            c->mesh->shouldMesh = true;
        }
    }

    sortChunks();
}

void World::setBlock(i32 x, i32 y, i32 z, Block *block) {
    if(y > Chunk::HEIGHT - 1) {
        return;
    }

    chunkMutex.lock();

    i32 chunkPosX = (i32) floorf((f32) x / (f32) Chunk::WIDTH);
    i32 chunkPosZ = (i32) floorf((f32) z / (f32) Chunk::DEPTH);

    Chunk *chunk = getChunk(glm::ivec2(chunkPosX, chunkPosZ));

    glm::ivec3 posInChunk = glm::ivec3(x % Chunk::WIDTH, y, z % Chunk::DEPTH);

    if(!chunk) {
        // // Chunk isn't loaded, block needs to be set when this chunk loads
        pendingBlockChanges.push_back((BlockSetData) {
            .chunkPos = glm::ivec2(chunkPosX, chunkPosZ),
            .blockPos = posInChunk,
            .block = block
        });

        chunkMutex.unlock();
        return;
    }

    chunk->set(posInChunk.x, posInChunk.y, posInChunk.z, block);
    storage.setChunk(chunk);

    if(posInChunk.x == 0) {
        Chunk *left = state.world->getChunk(glm::ivec2(chunk->pos.x - 1, chunk->pos.y));
        if(left && !left->get(Chunk::WIDTH - 1, posInChunk.y, posInChunk.z)->isAir()) {
            if(!left->mesh->generating) {
                left->mesh->shouldMesh = true;
            }
        }
    } else if(posInChunk.x == Chunk::WIDTH - 1) {
        Chunk *right = state.world->getChunk(glm::ivec2(chunk->pos.x + 1, chunk->pos.y));
        if(right && !right->get(0, posInChunk.y, posInChunk.z)->isAir()) {
            if(!right->mesh->generating) {
                right->mesh->shouldMesh = true;
            }
        }
    }

    if(posInChunk.z == 0) {
        Chunk *back = state.world->getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y - 1));
        if(back && !back->get(posInChunk.x, posInChunk.y, Chunk::DEPTH - 1)->isAir()) {
            if(!back->mesh->generating) {
                back->mesh->shouldMesh = true;
            }
        }
    } else if(posInChunk.z == Chunk::DEPTH - 1) {
        Chunk *front = state.world->getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y + 1));
        if(front && !front->get(posInChunk.x, posInChunk.y, 0)->isAir()) {
            if(!front->mesh->generating) {
                front->mesh->shouldMesh = true;
            }
        }
    }

    chunkMutex.unlock();
}

void World::setBlockAndMesh(i32 x, i32 y, i32 z, Block *block) {
    setBlock(x, y, z, block);

    i32 chunkPosX = (i32) floorf((f32) x / (f32) Chunk::WIDTH);
    i32 chunkPosZ = (i32) floorf((f32) z / (f32) Chunk::DEPTH);

    chunkMutex.lock();
    
    Chunk *chunk = getChunk(glm::ivec2(chunkPosX, chunkPosZ));

    if(chunk && !chunk->mesh->generating) {
        // Called before unlocking mutex
        chunk->mesh->generating = true;

        Chunk *adjacentChunks[] = {
            getChunk(glm::ivec2(chunk->pos.x - 1, chunk->pos.y)),
            getChunk(glm::ivec2(chunk->pos.x + 1, chunk->pos.y)),
            getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y + 1)),
            getChunk(glm::ivec2(chunk->pos.x, chunk->pos.y - 1))
        };

        for(Chunk *c : adjacentChunks) {
            if(c) {
                c->isSafeToDelete = false;
            }
        }

        chunkMutex.unlock();
        chunk->mesh->mesh(adjacentChunks[0], adjacentChunks[1], adjacentChunks[2], adjacentChunks[3]);

        for(Chunk *c : adjacentChunks) {
            if(c) {
                c->isSafeToDelete = true;
            }
        }
    } else {
        chunkMutex.unlock();
    }
}

void World::setBlock(glm::ivec3 pos, Block *block) {
    setBlock(pos.x, pos.y, pos.z, block);
}

void World::setBlockAndMesh(glm::ivec3 pos, Block *block) {
    setBlockAndMesh(pos.x, pos.y, pos.z, block);
}

Block *World::getBlock(i32 x, i32 y, i32 z) {
    chunkMutex.lock();

    i32 chunkPosX = (i32) floorf((f32) x / (f32) Chunk::WIDTH);
    i32 chunkPosZ = (i32) floorf((f32) z / (f32) Chunk::DEPTH);

    Chunk *chunk = getChunk(glm::ivec2(chunkPosX, chunkPosZ));

    if(!chunk) {
        // Chunk is not loaded
        chunkMutex.unlock();
        return nullptr;
    }

    chunkMutex.unlock();
    return chunk->get(x % Chunk::WIDTH, y, z % Chunk::DEPTH);
}

Block *World::getBlock(glm::ivec3 pos) {
    return getBlock(pos.x, pos.y, pos.z);
}

void World::updateChunks() {
    ChunkMesh *m = nullptr;

    chunkMutex.lock();

    for(Chunk *chunk : chunks) {
        ChunkMesh *mesh = chunk->mesh;

        if(mesh->shouldMesh && !mesh->generating) {
            m = mesh;
            break;
        }
    }

    if(m && !m->generating) {
        Chunk *adjacentChunks[] = {
            state.world->getChunk(glm::ivec2(m->chunk->pos.x - 1, m->chunk->pos.y)),
            state.world->getChunk(glm::ivec2(m->chunk->pos.x + 1, m->chunk->pos.y)),
            state.world->getChunk(glm::ivec2(m->chunk->pos.x, m->chunk->pos.y + 1)),
            state.world->getChunk(glm::ivec2(m->chunk->pos.x, m->chunk->pos.y - 1))
        };

        for(Chunk *c : adjacentChunks) {
            if(c) {
                c->isSafeToDelete = false;
            }
        }

        if(m && !m->generating) {
            m->generating = true;
            chunkMutex.unlock();
            m->mesh(adjacentChunks[0], adjacentChunks[1], adjacentChunks[2], adjacentChunks[3]);
        } else {
            chunkMutex.unlock();
        }

        for(Chunk *c : adjacentChunks) {
            if(c) {
                c->isSafeToDelete = true;
            }
        }
    } else {
        chunkMutex.unlock();
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
    chunkMutex.lock();
    std::sort(chunks.begin(), chunks.end(), chunkDepthCmp);
    chunkMutex.unlock();
}

void World::loadChunks() {
    i32 renderDistance = state.settings.renderDistance;

    std::vector<i32> chunksToRemove;

    i32 chunkPosX = (i32) floorf(state.player->pos.x / (f32) Chunk::WIDTH);
    i32 chunkPosZ = (i32) floorf(state.player->pos.z / (f32) Chunk::DEPTH);

    chunkMutex.lock();

    for(i32 i = 0; i < chunks.size(); i++) {
        Chunk *chunk = chunks[i];

        if(abs(chunk->pos.x - chunkPosX) > renderDistance) {
            chunksToRemove.push_back(i);
        } else if(abs(chunk->pos.y - chunkPosZ) > renderDistance) {
            chunksToRemove.push_back(i);
        }
    }

    for(i32 i = chunksToRemove.size() - 1; i >= 0; i--) {
        if(chunks[chunksToRemove[i]]->isSafeToDelete) {
            chunks[chunksToRemove[i]]->destroy();
            delete chunks[chunksToRemove[i]];
            chunks.erase(chunks.begin() + chunksToRemove[i]);
        }
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
    chunkMutex.unlock();
}

void World::updateChunkStorage(Chunk *chunk) {
    ChunkData *data = storage.getChunkData(chunk->pos.x, chunk->pos.y);

    chunkMutex.lock();
    if(!data) {
        storage.setChunk(chunk);
    } else {
        data->writeData(chunk);
    }
    chunkMutex.unlock();
}

Chunk *World::getChunk(glm::ivec2 pos) {
    chunkMutex.lock();
    for(Chunk *chunk : chunks) {
        if(chunk->pos == pos) {
            chunkMutex.unlock();
            return chunk;
        }
    }

    chunkMutex.unlock();
    return nullptr;
}

glm::ivec3 World::pointToBlockPos(glm::vec3 point) {
    return glm::ivec3((i32) floorf(point.x), (i32) floorf(point.y), (i32) floorf(point.z));
}

void World::destroy() {
    chunkMutex.lock();
    for(Chunk *chunk : chunks) {
        delete chunk;
    }
    chunkMutex.unlock();
    storage.destroy();
}